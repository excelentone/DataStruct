#include "QueueNode.h"

template<typename T> class LinkQueue
{
public:
	LinkQueue() :_pre(NULL), _pFront(NULL)
	{}
	~LinkQueue()
	{
		MakeEmpty();
	}
	void Append(const T item);   //插入数据
	T Delete();                  //删除数据
	T GetFront();                //获取队头元素
	void MakeEmpty();               //m清空队列
	void Print();                   //打印队列

	bool IsEmpty() const
	{
		return _pFront == NULL;
	}

private:
	QueueNode<T> *_pre, *_pFront;
};

template<typename T>
void LinkQueue<T>::MakeEmpty()
{
	QueueNode<T> *pdel;
	while (_pFront)
	{
		pdel = _pFront;
		_pFront = _pFront->_pNext;
		delete pdel;
	}
}

template<typename T>
void LinkQueue<T>::Append(const T item)
{
	if (_pFront == NULL)
	{
		_pFront = _pre = new QueueNode<T>(item);
	}
	else
	{
		_pre = _pre->_pNext = new QueueNode<T>(item);
	}
}

template<typename T>
T LinkQueue<T>::Delete()
{
	if (IsEmpty())
	{
		cout << "There is no element!" << endl;
		exit(1);
	}
	QueueNode<T> *pdel = _pFront;
	T temp = _pFront->_data;
	_pFront = _pFront->_pNext;
	delete pdel;
	return temp;
}

template<typename T> 
T LinkQueue<T>::GetFront()
{
	if (IsEmpty())
	{
		cout << "There is no element!" << endl;
		exit(1);
	}
	return _pFront->_data;
}

template<typename T> 
void LinkQueue<T>::Print()
{
	QueueNode<T> *pmove = _pFront;
	cout << "front";
	while (pmove)
	{
		cout << "--->" << pmove->_data;
		pmove = pmove->_pNext;
	}
	cout << "--->rear" << endl << endl << endl;
}

