template<typename T> 
class LinkQueue;

template<typename T>
class QueueNode
{
private:
	friend class LinkQueue < T > ;
	QueueNode(const T item, QueueNode<T> *next = NULL)
		:_data(item)
		, _pNext(next)
	{}
	//给需要用到节点成员的类一个友元类，然后将数据成员封装起来，
	//有利于保证数据的保密性
private:
	T _data;
	QueueNode<T> *_pNext;
};
