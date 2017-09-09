template<typename T> 
class LinkStack;

template<typename T> 
class StackNode
{
private:
	friend class LinkStack < T > ;
	StackNode(T dt, StackNode<T> *next = NULL)
		:_data(dt)
		, _pNext(next)
	{}
private:
	T _data;
	StackNode<T> *_pNext;
};
