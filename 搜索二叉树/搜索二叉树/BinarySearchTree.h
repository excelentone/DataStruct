#include<iostream>
#include<cassert>
using namespace std;
template<class T>
struct BinarySearchNode
{
	T _data;
	BinarySearchNode *_pLeft;
	BinarySearchNode *_pRight;
	BinarySearchNode(const T &data, BinarySearchNode *pLeft = nullptr, BinarySearchNode *pRight = nullptr)
		:_data(data),
		_pLeft(pLeft),
		_pRight(pRight)
	{}
	BinarySearchNode(T &&data, BinarySearchNode *pLeft = nullptr, BinarySearchNode *pRight = nullptr)
		:_data(move(data)), _pLeft(pLeft), _pRight(pRight)
	{}
};
template<class T>
class BinarySearchTree
{
private:
	typedef BinarySearchNode<T> Node;
public:
	BinarySearchTree() :_pRoot(nullptr)
	{}
	BinarySearchTree(BinarySearchTree &bst)
	{
		_pRoot=_clone(bst._pRoot);
	}
	BinarySearchTree(const BinarySearchTree &&bst)
	{
		_pRoot=_clone(move(bst._pRoot));
	}
	~BinarySearchTree()
	{
		_makeEmpty(_pRoot);
	}
	const T &findMin()const
	{
		return _findMin(_pRoot)->_data;
	}
	const T &findMax()const
	{
		return _findMax(_pRoot)->_data;
	}
	bool contains(const T &x)const
	{
		return _contains( x,_pRoot);
	}
	bool isEmpty()const
	{
		return nullptr == _pRoot;
	}
	void InOrder()
	{
		_InOrder(_pRoot);
		cout << endl;
	}
	void insert(const T &x)
	{
		_insert(x, _pRoot);
	}
	void insert(T &&x)
	{
		_insert(x, _pRoot);
	}
	void remove(const T &x)
	{
		_remove(x, _pRoot);
	}
	BinarySearchTree &operator=(const BinarySearchTree &bst)
	{
		if (_pRoot)
		{
			_makeEmpty(_pRoot);
		}
		_pRoot = _clone(bst._pRoot);
	}
	BinarySearchTree &operator=(BinarySearchTree &&bst);
private:
	void _insert(const T &x, Node *&pRoot)
	{
		if (nullptr == pRoot)
			pRoot = new Node(x);
		else if (x < pRoot->_data)
			_insert(x, pRoot->_pLeft);
		else if (x>pRoot->_data)
			_insert(x, pRoot->_pRight);
		else
			return;
	}
	void _insert(T &&x, Node *&pRoot)
	{
		if (nullptr == pRoot)
			pRoot = new Node(move(x));
		else if (x < pRoot->_data)
			_insert(x, pRoot->_pLeft);
		else if (x>pRoot->_data)
			_insert(x, pRoot->_pRight);
		else
			return;
	}
	void _remove(const T &x, Node *&pRoot)
	{
		if (nullptr == pRoot)
			return;
		if (x < pRoot->_data)
			_remove(pRoot->_pLeft);
		else if (x>pRoot->_data)
			_remove(pRoot->_pRight);
		else if (nullptr!=pRoot->_pLeft&&nullptr!=pRoot->_pRight)
		{
			pRoot->_data = _findMin(pRoot->_pRight)->_data;
			_remove(pRoot->_data, pRoot->_pRight);
		}
		else
		{
			Node *DelNode = pRoot;
			pRoot = (nullptr != pRoot->_pLeft) ? pRoot->_pLeft : pRoot->_pRight;
			delete DelNode;
		}
	}
	Node *_findMin(Node *pRoot)const
	{
		assert(pRoot);
		while (nullptr != pRoot->_pLeft)
			pRoot = pRoot->_pLeft;
		return pRoot;
	}
	Node *_findMax(Node *pRoot)const
	{
		assert(pRoot);
		while (nullptr != pRoot->_pRight)
			pRoot = pRoot->_pRight;
		return pRoot;
	}
	bool _contains(const T &x, Node *pRoot)const
	{
		if (nullptr == pRoot)
			return false;
		else if (pRoot->_data > x)
			return _contains(x, pRoot->_pLeft);
		else if (pRoot->_data < x)
			return _contains(x, pRoot->_pRight);
		else
			return true;
	}
	void _makeEmpty(Node *&pRoot)
	{
		if (nullptr != pRoot)
		{
			_makeEmpty(pRoot->_pLeft);
			_makeEmpty(pRoot->_pRight);
			delete pRoot;
		}
		pRoot = nullptr;
	}
	void _InOrder(Node *pRoot)const
	{
		if (nullptr != pRoot)
		{
			_InOrder(pRoot->_pLeft);
			cout << pRoot->_data << " ";
			_InOrder(pRoot->_pRight);
		}
	}
	Node *_clone(Node *pRoot)
	{
		Node *newpRoot = NULL;
		if (pRoot != NULL)
		{
			newpRoot = new Node(pRoot->_data);
			newpRoot->_pLeft= _clone(pRoot->_pLeft);
			newpRoot->_pRight = _clone(pRoot->_pRight);
		}
		return newpRoot;
	}
private:
	
	Node *_pRoot;

};
