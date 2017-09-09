#include "StackNode.h"

template<typename T> 
class LinkStack
{
public:
	LinkStack() :_pTop(NULL)
	{}
	~LinkStack()
	{
		MakeEmpty();
	}

public:
	void MakeEmpty();           //清空栈
	void Push(const T item); //入栈
	T Pop();                 //弹栈
	T GetTop() const;        //获取栈顶元素
	void Print();               //打印栈

	bool IsEmpty() const
	{
		return _pTop == NULL;
	}

private:
	StackNode<T> *_pTop;
};

template<typename T> 
void LinkStack<T>::MakeEmpty()
{
	StackNode<T> *pmove;
	while (_pTop != NULL)
	{
		pmove = _pTop;
		_pTop = _pTop->_pNext;
		delete pmove;
	}
}

template<typename T>
void LinkStack<T>::Push(const T item)
{
	_pTop = new StackNode<T>(item, _pTop);
}

template<typename T> 
T LinkStack<T>::GetTop() const
{
	if (IsEmpty())
	{
		cout << "There is no elements!" << endl;
		exit(1);
	}
	return _pTop->_data;
}

template<typename T>
T LinkStack<T>::Pop()
{
	if (IsEmpty())
	{
		cout << "There is no elements!" << endl;
		exit(1);
	}
	StackNode<T> *pdel = _pTop;
	_pTop = _pTop->_pNext;
	T temp = pdel->_data;
	delete pdel;
	return temp;
}

template<typename T>
void LinkStack<T>::Print()
{
	StackNode<T> *pmove = _pTop;
	cout << "buttom";
	while (pmove != NULL)
	{
		cout << "--->" << pmove->_data;
		pmove = pmove->_pNext;
	}
	cout << "--->top" << endl << endl << endl;
}

