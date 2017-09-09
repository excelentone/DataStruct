#include<iostream>
#include<cassert>
#include <vector>
using namespace std;
template<class T>
struct Less
{
	bool operator()(const T&left, const T &right)
	{
		return left < right;
	}
};
template<class T>
struct Greater
{
	bool operator()(const T&left, const T&right)
	{
		return left>right;
	}
};
//template<class T,class Compare=Less<T> >
template<class T, template<class T> class Compare = Less >
class Heap
{
public:
	// 创建一个空堆
	Heap()
	{}

	Heap(const T array[], size_t size)
	{
		_heap.resize(size);
		for (size_t i = 0; i < size; i++)
		{
			_heap[i] = array[i];
		}
		AdjustDown();
	}
	void AdjustDown()
	{
		int flag = (_heap.size() - 2) / 2;
		for (int i = flag; i >= 0; i--)
		{
			_AdjustDown(i);
		}
	}
	size_t Size()const
	{
		return _heap.size();
	}
	bool Empty()const
	{
		return _heap.empty();
	}
	void Insert(const T& data)
	{
		_heap.push_back(data);
		_AdjustUp(_heap.size() - 1);
	}
	void Remove()
	{
		assert(_heap.size() != 0);
		swap(_heap[0], _heap[_heap.size() - 1]);
		_heap.pop_back();
		AdjustDown();
	}
	void Print()
	{
		for (size_t i = 0; i < _heap.size(); i++)
		{
			cout << _heap[i] << " ";
		}
		cout << endl;
	}
	const T &Top()
	{
		return _heap[0];
	}
protected:
	void _AdjustDown(size_t parent)
	{
		int child = 2 * parent + 1;
		Compare<T> com;
		while (child <= _heap.size())
		{
			if (((child + 1) < _heap.size()) && com(_heap[child + 1], _heap[child]))
			{
				child += 1;
			}
			if (com(_heap[child], _heap[parent]))
			{
				swap(_heap[parent], _heap[child]);
				parent = child;
				child = 2 * parent + 1;
			}
			else
			{
				break;
			}
		}
	}
	void _AdjustUp(size_t child)
	{
		int parent = (child - 1) / 2;
		while (child > 0)
		{
			if (Compare<T>()(_heap[child], _heap[parent]))
			{
				swap(_heap[child], _heap[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
				break;
		}
	}


protected:
	vector<T> _heap;
};
