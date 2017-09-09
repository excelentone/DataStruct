#include<iostream>
using namespace std;
template<class T>
class Queue
{
public:
	Queue(size_t size = 0) :_capacity(size),
		_head(0),
		_rear(0),
		_pBuf(new T[size]),
		_QueueLen(0)
	{}
	Queue(const Queue &q)
	{
		_pBuf = new T[q._capacity];
		for (size_t i = 0; i < q._capacity; i++)
		{
			_pBuf[i] = q._pBuf[i];
		}
		_capacity = q._capacity;
		_QueueLen = q._QueueLen;
		_rear = q._rear;
		_head = q._head;
	}
	Queue &operator=(const Queue &q)
	{
		if (this != &q)
		{
			delete[]_pBuf;
			_pBuf = new T[q._capacity];

			for (size_t i = 0; i < q._capacity; i++)
			{
				_pBuf[i] = q._pBuf[i];
			}
			_QueueLen = q._QueueLen;
			_capacity = q._capacity;
			_rear = q._rear;
			_head = q._head;
		}
		return *this;
	}
	bool QueueFull()
	{
		return _QueueLen == _capacity;
	}
	void Enqueue(const T data)
	{
		if (!QueueFull())
		{
			_pBuf[_rear] = data;
			_rear = (_rear + 1) % _capacity;
			_QueueLen++;
		}
		else
		{
			cout << "¶ÓÁÐÒÑÂú" << endl;
		}
	}
	bool QueueEmpty()
	{
		return _QueueLen == 0;
	}
	void Dequeue()
	{
		if (!QueueEmpty())
		{
			_head = (_head + 1) % _capacity;
			_QueueLen--;
		}
	}
	friend ostream& operator<<(ostream &os, const Queue &q)
	{
		for (size_t i = q._head; i < q._head + q._QueueLen; i++)
		{
			os << q._pBuf[i%q._capacity] << " ";
		}
		return os;
	}
	~Queue()
	{
		if (NULL != _pBuf)
		{
			delete[]_pBuf;
		}
	}
private:
	T *_pBuf;
	size_t _capacity;
	size_t _QueueLen;
	size_t _head;
	size_t _rear;
};