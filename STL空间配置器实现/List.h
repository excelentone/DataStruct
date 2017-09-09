#pragma once
#include "STLAlloc.h"
#include "STLConstruct.h"

#include <iostream>
using namespace std;

#include <string>
template<class T>
struct ListNode
{
	ListNode(const T& data = T())
		: _pNext(NULL)
		, _pPre(NULL)
		, _data(data)
	{}

	ListNode<T>* _pNext;
	ListNode<T>* _pPre;
	T            _data;
};

template<class T, class Ref, class Pointer>
class _ListIterator_
{
	typedef _ListIterator_<T, Ref, Pointer> Self;
public:
	typedef T ValueType;
	typedef T& Reference;
	typedef T* Pointer;
	typedef int DifferenceType;
	typedef BidirectionalIteratorTag IteratorCategory;
public:
	_ListIterator_()
		: _pNode(NULL)
	{}

	_ListIterator_(ListNode<T>* pNode)
		: _pNode(pNode)
	{}

	_ListIterator_(const Self& s)
		: _pNode(s._pNode)
	{}

	Ref operator*()
	{
		return _pNode->_data;
	}

	Pointer operator->()
	{
		return (&operator*());
	}

	Self& operator++()
	{
		_pNode = _pNode->_pNext;
		return *this;
	}

	Self operator++(int)
	{
		Self temp(*this);
		_pNode = _pNode->_pNext;
		return temp;
	}

	Self& operator--()
	{
		_pNode = _pNode->_pPre;
		return *this;
	}

	Self operator--(int)
	{
		Self temp(*this);
		_pNode = _pNode->_pPre;
		return temp;
	}

	bool operator!=(const Self& s)
	{
		return _pNode != s._pNode;
	}

	bool operator==(const Self& s)
	{
		return _pNode == s._pNode;
	}
private:
	ListNode<T>* _pNode;
};

template<class T, class Alloc = _Alloc>
class List
{
	typedef SimpleAlloc<ListNode<T>, Alloc> NodeAlloc;
public:
	typedef _ListIterator_<T, T&, T*> Iterator;
	typedef _ListIterator_<T, const T&, const T*> ConstIterator;
	
	typedef ReverseIterator<ConstIterator> ConstReverseIterator;
	typedef ReverseIterator<Iterator> ReverseIterator;
	
	//      ReverseIterator<Iterator><ConstIterator>
public:
	List()
	{
		_pHead = CreateNode();
		_pHead->_pPre = _pHead;
		_pHead->_pNext = _pHead;
	}

	void Clear()
	{
		Destroy(Begin(), End());
	}

	~List()
	{
		Clear();
		Destroy(&(_pHead->_data));
	}

	void PushBack(const T& data)
	{
		ListNode<T>* pNewNode = CreateNode(data);
		ListNode<T>* pTailNode = _pHead->_pPre;

		pTailNode->_pNext = pNewNode;
		pNewNode->_pPre = pTailNode;
		pNewNode->_pNext = _pHead;
		_pHead->_pPre = pNewNode;
	}

	void PopBack()
	{
		ListNode<T>* pTailNode = _pHead->_pPre;
		if(_pHead->_pNext != _pHead)
		{
			pTailNode->_pPre->_pNext = _pHead;
			_pHead->_pPre = pTailNode->_pPre;
		}

		DestroyNode(pTailNode);
	}

	void PushFront(const T& data)
	{
		ListNode<T>* pNewNode = CreateNode(data);
		pNewNode->_pNext = _pHead->_pNext;
		_pHead->_pNext = pNewNode;
		pNewNode->_pPre = _pHead;
		pNewNode->_pNext->_pPre = pNewNode;
	}

	void PopFront()
	{
		ListNode<T>* pDel = _pHead->_pNext;
		if(pDel != _pHead)
		{
			_pHead->_pNext = pDel->_pNext;
			pDel->_pNext->_pPre = _pHead;
			DestroyNode(pDel);
		}
	}

	Iterator Insert(Iterator pos, const T& data);
	Iterator Erase(Iterator pos);

	//////////////////////////////////////////////////////
	Iterator Begin()
	{
		return Iterator(_pHead->_pNext);
	}

	Iterator End()
	{
		return Iterator(_pHead);
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
	ListNode<T>* CreateNode(const T& data = T())
	{
		ListNode<T>* p = NodeAlloc::Allocate();
		Construct(p, data);
		return p;
	}

	void DestroyNode(ListNode<T>* p)
	{
		Destroy(&(p->_data));
		NodeAlloc::DeAllocate(p);
	}
private:
	ListNode<T>* _pHead;
};

void FunTest1()
{
	List<int> L;
	L.PushBack(1);
	L.PushBack(2);
	L.PushBack(3);
	L.PushBack(4);

	List<int>::Iterator it = L.Begin();
	while(it != L.End())
	{
		cout<<*it<<"->";
		++it;
	}
	cout<<"NULL"<<endl;

	List<int>::ReverseIterator Rit = L.RBegin();
	while(Rit != L.REnd())
	{
		cout<<*Rit<<" ";
		++Rit;
	}

	cout<<endl;

	Distance(L.Begin(), L.End());
}

void FunTest2()
{
	List<string> L;
	L.PushBack("1111");
	L.PushBack("2222");
	L.PushBack("3333");
	L.PushBack("4444");

	List<string>::Iterator it = L.Begin();
	while(it != L.End())
	{
		cout<<*it<<"->";
		++it;
	}
	cout<<"NULL"<<endl;
}
