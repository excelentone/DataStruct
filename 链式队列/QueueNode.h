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
	//����Ҫ�õ��ڵ��Ա����һ����Ԫ�࣬Ȼ�����ݳ�Ա��װ������
	//�����ڱ�֤���ݵı�����
private:
	T _data;
	QueueNode<T> *_pNext;
};
