#include"Heap.h"
template<class T, class Compare = Less<T>>
class PriorityQueue
{
public:
	PriorityQueue() :_hp()
	{}
	PriorityQueue(T *arr, size_t size) :_hp(arr, size)
	{}
	void Push(const T& data)
	{
		_hp.Insert(data);
	}
	void Pop()
	{
		_hp.Remove();
	}
	const T& Top()const
	{
		return _hp.top();
	}
	size_t Size()const
	{
		return _hp.Size();
	}
	bool Empty()const
	{
		return _hp.Empty();
	}
	void Print()
	{
		_hp.Print();
	}
protected:
	Heap<T, Less> _hp;
};
void test1()
{
	int arr[] = { 53, 17, 78, 9, 45, 65, 87, 23 };
	Heap<int, Less> hp(arr, sizeof(arr) / sizeof(arr[0]));
	hp.Insert(5);
	hp.Print();
	//	hp.Remove();
	hp.Print();
}
void test2()
{
	int arr[] = { 53, 17, 78, 9, 45, 65, 87, 23 };
	PriorityQueue<int> pq;//pq(arr, sizeof(arr) / sizeof(arr[0]));
	pq.Push(53);
	pq.Push(17);
	pq.Push(78);
	pq.Pop();
	pq.Print();

}
int main()
{
	test1();
	test2();
	system("pause");
}