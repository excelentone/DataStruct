#pragma once
#include "STLAlloc.h"
#include "STLConstruct.h"
#include <string>
#include <iostream>
using namespace std;

// 没有任何意义
template<class T, class Ref, class Pointer>
class VectorIterator
{
public:
	typedef VectorIterator<T, Ref, Pointer> Self;
	typedef T ValueType;
	VectorIterator()
		: _pData(NULL)
	{}

	VectorIterator(T* pData)
		: _pData(pData)
	{}

	VectorIterator(const Self& s)
		: _pData(s._pData)
	{}

	Ref operator*()
	{
		return *_pData;
	}

	Pointer operator->()
	{
		return _pData;
	}

	Self& operator++()
	{
		++_pData;
		return *this;
	}

	Self operator++(int)
	{
		Self temp(*This);
		++_pData;
		return temp;
	}

	Self& operator--()
	{
		--_pData;
		return *this;
	}

	Self operator--(int)
	{
		Self temp(*this);
		--_pData;
		return temp;
	}

	bool operator=(const Self& s)
	{
		return _pData == s._pData;
	}

	bool operator!=(const Self& s)
	{
		return _pData != s._pData;
	}
private:
	T* _pData;
};

template<class T, class Alloc = _Alloc>
class Vector
{
	typedef SimpleAlloc<T, Alloc> dataAlloc;
public:
	typedef T* Iterator;
	typedef ReverseIterator<Iterator> ReverseIterator;

	//typedef VectorIterator<T, T&, T*> Iterator;
	typedef T ValueType;

public:
	Vector()
		: _start(NULL)
		, _finish(NULL)
		, _endOfStorage(NULL)
	{}

	Vector(size_t n, const T& data)
	{
		_start = dataAlloc::Allocate(n);
		for (size_t idx = 0; idx < n; ++idx)
			Construct(_start+idx, data);

		_finish = _start + n;
		_endOfStorage = _finish;
	}

	~Vector()
	{
		Clear();
		dataAlloc::DeAllocate(_start, Size());
	}

	////////////////////////////////////////////////////
	void PushBack(const T& data)
	{
		_CheckCapacity();
		//*_finish = data;
		Construct(_finish, data);
		++_finish;
	}

	void PopBack()
	{
		assert(_start == _finish);
		Destroy(_finish-1);
		--_finish;
	}

	void Insert(Iterator pos, const T& data)
	{
		assert(!(pos >= Begin() && pos <= End()));
		_CheckCapacity();

		for (Iterator it = _finish; it > pos; --it)
			*it = *(it-1);

		*pos = data
		_finish++;
		return pos;
	}

	Iterator Erase(Iterator pos)
	{
		assert(!(pos >= Begin() && pos < End()));

		for (Iterator it = pos; it < _finish-1; ++it)
			*it = *(it+1);
		Destroy(_finish-1);
		_finish--;
		return pos;
	}

	void Clear()
	{
		Destroy(_start, _finish);
		_finish = _start;
	}

	/////////////////////////////////////////////
	size_t Size()const
	{
		return _finish - _start;
	}

	size_t Capacity()const
	{
		return _endOfStorage - _start;
	}

	bool Empty()const
	{
		return NULL == _start;
	}

	void ReSize(size_t newSize, const T& data = T())
	{
		if(newSize < Size())
		{
			Destroy(_start+newSize, _finish);
			_finish = _start + newSize;
		}
		else if(_start+newSize < _endOfStorage)
		{
			for(size_t idx = Size(); idx < newSize; ++idx)
				Construct(_start+idx, data);
		}
		else
		{
			Iterator temp = dataAlloc::Allocate(newSize);
			size_t oldSize = Size();
			// 旧空间中的元素-->UnInitCopy
			for(size_t idx = 0; idx < oldSize; ++idx)
				Construct(temp+idx, _start+idx);

			// 销毁旧空间
			Destroy(_start, _finish);
			dataAlloc::DeAllocate(_start, Capacity());

			for(size_t idx = oldSize; idx < newSize; ++idx)
				Construct(temp+idx, data);

			_start = temp;
			_finish = _start+newSize;
			_endOfStorage = _finish;
		}
	}

	void Reserve(size_t n)
	{
		size_t oldSize = Size();
		Iterator temp = dataAlloc::Allocate(n);
		if(n < oldSize)
		{

		}

		// 旧空间释放

		_start = temp;
		_finish = _start + ((oldSize < n)? oldSize:n);
		_endOfStorage = _start + n;
	}

	////////////////////////////////////////////////////////
	Iterator Begin()
	{
		return _start;
	}

	Iterator End()
	{
		return _finish;
	}

	ReverseIterator RBegin()
	{
		return ReverseIterator(End());
	}

	ReverseIterator REnd()
	{
		return ReverseIterator(Begin());
	}

private:
	void _CheckCapacity()
	{
		if(_finish == _endOfStorage)
		{
			size_t oldSize = Size();
			size_t newSize = oldSize*2 + 3;
			Iterator temp = dataAlloc::Allocate(newSize);

			for(size_t idx = 0; idx < oldSize; ++idx)
				Construct(temp+idx, _start[idx]);

			if(_start)
			{
				Destroy(_start, _finish);
				dataAlloc::DeAllocate(_start, oldSize);
			}

			_start = temp;
			_finish = _start+oldSize;
			_endOfStorage = _start+newSize;
		}
	}

private:
	Iterator _start;
	Iterator _finish;
	Iterator _endOfStorage;
};

void TestVector1()
{
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);

	Vector<int>::Iterator it = v.Begin();
	while(it != v.End())
	{
		cout<<*it<<" ";
		++it;
	}

	cout<<endl;

	Distance(v.Begin(), v.End());
}

void TestVector2()
{
	Vector<string> v;
	v.PushBack("1111");
	v.PushBack("2222");
	v.PushBack("3333");
	v.PushBack("4444");

	Vector<string>::Iterator it = v.Begin();
	while(it != v.End())
	{
		cout<<*it<<" ";
		++it;
	}

	cout<<endl;
}