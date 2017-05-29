#include<iostream>
using namespace std;
#include<cassert>
enum COLOR{ RED, BLACK };
template<class K, class V>
struct RBTreeNode
{
	K _key;
	V _value;
	COLOR _color;
	RBTreeNode<K, V> *_pLeft;
	RBTreeNode<K, V> *_pRight;
	RBTreeNode<K, V> *_pParent;
	RBTreeNode(const K &key, const V &value)
		: _key(key)
		, _value(value)
		, _pLeft(nullptr)
		, _pRight(nullptr)
		, _pParent(nullptr)
		, _color(RED)
	{}
};
template<class K, class V>
class RBTreeIterator
{
private:
	typedef RBTreeNode<K, V> Node;
	typedef RBTreeIterator<K, V> Iterator;
public:
	RBTreeIterator(Node *pNode = nullptr) :_pNode(pNode)
	{}
	Iterator &operator++()
	{
		_Increament(_pNode);
		return *this;
	}
	Iterator &operator--()
	{
		_Decreament(_pNode);
		return *this;
	}
	Iterator operator++(int)
	{
		Iterator temp(*this);
		++(*this);
		return temp;
	}
	Iterator operator--(int)
	{
		Iterator temp(*this);
		--(*this);
		return temp;
	}
	bool operator==(Iterator &it)
	{
		return _pNode == it._pNode;
	}
	bool operator!=(Iterator &it)
	{
		return !(*this == it);
	}
private:
	void _Increament(Node *pNode)
	{
		if (pNode->_pRight)
			pNode = FindMin(pNode->_pRight);
		else
		{
			Node *pParent = pNode->_pParent;
			while (pParent->_pRight == pNode)
			{
				pNode = pParent;
				pParent = pNode->_pParent;
			}
			if (pParent->_pRight != pNode)
				pNode = pParent;
		}
		_pNode = pNode;
	}
	Node *FindMin(Node *pNode)
	{
		assert(pNode);
		while (pNode->_pLeft)
			pNode = pNode->_pLeft;
		return pNode;
	}
	Node *FindMax(Node *pNode)
	{
		assert(pNode);
		while (pNode->_pRight)
			pNode = pNode->_pRight;
		return pNode;
	}
	void _Decreament(Node *pNode)
	{
		if (pNode->_pParent->_pParent == pNode&&pNode->_color == RED)
		{
			pNode = FindMax(_pNode->_pParent);
		}
		else if (pNode->_pLeft)
			pNode = FindMax(pNode->_pLeft);
		else
		{
			Node *pParent = pNode->_pParent;
			while (pParent->_pLeft == pNode)
			{
				pNode = pParent;
				pParent = pNode->_pParent;
			}
			pNode = pParent;

		}
		_pNode = pNode;
	}
public:
	Node *getNode()
	{
		return _pNode;
	}
private:
	Node *_pNode;
};
template<class K, class V>
class RBTree
{
private:
	typedef RBTreeIterator<K, V> Iterator;
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
	{
		_pHead = new Node(0, 0);
		_pHead->_pLeft = _pHead;
		_pHead->_pRight = _pHead;
		_pHead->_pParent = nullptr;
	}
	Node *FindMin(Node *pNode)
	{
		assert(pNode);
		while (pNode->_pLeft)
			pNode = pNode->_pLeft;
		return pNode;
	}
	Node *FindMax(Node *pNode)
	{
		assert(pNode);
		while (pNode->_pRight)
			pNode = pNode->_pRight;
		return pNode;
	}
	bool Insert(const K &key, const V &value)
	{
		bool flag=_Insert(_pHead->_pParent, key, value);
		_pHead->_pLeft = FindMin(_pHead->_pParent);
		_pHead->_pRight = FindMax(_pHead->_pParent);
		return flag;
	}
	void InOrder()
	{
		cout << "Inorder: ";
		_InOrder(_pHead->_pParent);
		cout << endl;
	}
	bool CheckRBTree()
	{
		return _CheckRBTree(_pHead->_pParent);
	}
	Iterator Begin()
	{
		return Iterator(FindMin(_pHead->_pParent));
	}
	Iterator End()
	{
		return Iterator(_pHead);
	}
private:
	void _RotateL(Node *parent)
	{
		Node *pSubR = parent->_pRight;
		Node *ppParent = parent->_pParent;
		Node *pSubRL = pSubR->_pLeft;
		parent->_pRight = pSubRL;
		if (nullptr != pSubRL)
			pSubRL->_pParent = parent;
		pSubR->_pLeft = parent;
		parent->_pParent = pSubR;
		if (_pHead == ppParent)
		{
			_pHead->_pParent = pSubR;
			pSubR->_pParent = _pHead;
		}
		else
		{
			if (parent == ppParent->_pLeft)
				ppParent->_pLeft = pSubR;
			else
				ppParent->_pRight = pSubR;
			pSubR->_pParent = ppParent;
		}
	}
	void _RotateR(Node *parent)
	{
		Node *pSubL = parent->_pLeft;
		Node *pSubLR = pSubL->_pRight;
		Node *ppParent = parent->_pParent;
		parent->_pLeft = pSubLR;
		if (pSubLR)
			pSubLR->_pParent = parent;
		pSubL->_pRight = parent;
		parent->_pParent = pSubL;
		if (_pHead == ppParent)
		{
			_pHead->_pParent = pSubL;
			pSubL->_pParent = _pHead;
		}
		else
		{
			if (parent == ppParent->_pLeft)
				ppParent->_pLeft = pSubL;
			else
				ppParent->_pRight = pSubL;
			pSubL->_pParent = ppParent;
		}
	}
	void _InOrder(Node *pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_pLeft);
			cout << pRoot->_key << " ";
			_InOrder(pRoot->_pRight);
		}
	}
	bool _Insert(Node* pRoot, const K&key, const V&value)
	{
		Node *&_pRoot = _pHead->_pParent;
		if (nullptr == pRoot)
		{
			_pRoot= new Node(key, value);
			_pRoot->_pParent = _pHead;
			_pRoot->_color = BLACK;
			return true;
		}
		Node *pCur = pRoot;
		Node *parent = nullptr;
		while (pCur)
		{
			if (pCur->_key < key)
			{
				parent = pCur;
				pCur = pCur->_pRight;
			}
			else if (pCur->_key>key)
			{
				parent = pCur;
				pCur = pCur->_pLeft;
			}
			else
				return false;
		}

		pCur = new Node(key, value);

		if (parent->_key < key)
			parent->_pRight = pCur;
		else
			parent->_pLeft = pCur;
		pCur->_pParent = parent;
		while (parent!=_pHead&&parent->_color == RED)
		{
			Node *pGrand = parent->_pParent;

			if (parent == pGrand->_pLeft)
			{
				Node *pUncle = pGrand->_pRight;
				if (pUncle&&pUncle->_color == RED)
				{
					parent->_color = BLACK;
					pUncle->_color = BLACK;
					pGrand->_color = RED;
					pCur = pGrand;
					parent = pCur->_pParent;
				}
				else
				{
					if (pCur == parent->_pRight)
					{
						_RotateL(parent);
						swap(parent, pCur);
					}
					_RotateR(pGrand);
					swap(parent->_color, pGrand->_color);
					break;
				}

			}
			else
			{
				Node *pUncle = pGrand->_pLeft;
				if (pUncle&&RED == pUncle->_color)
				{
					pUncle->_color = BLACK;
					parent->_color = BLACK;
					pGrand->_color = RED;
					pCur = pGrand;
					parent = pCur->_pParent;
				}
				else
				{
					if (pCur == parent->_pLeft)
					{
						_RotateR(parent);
						swap(parent, pCur);
					}
					_RotateL(pGrand);
					swap(parent->_color, pGrand->_color);
					break;
				}

			}

		}
		_pRoot->_color = BLACK;
		return true;
	}
	bool _CheckRBTree(Node *pRoot)
	{
		if (nullptr == pRoot)
			return true;
		size_t count = 0;
		while (nullptr != pRoot)
		{
			if (BLACK == pRoot->_color)
				count++;
			pRoot = pRoot->_pLeft;
		}
		return _CheckNum(pRoot, count, 0);
	}
	bool _CheckNum(Node *pRoot, const size_t &count, size_t Scount)
	{
		if (nullptr == pRoot)
			return true;
		if (pRoot->_color == BLACK)
		{
			Scount++;
		}
		if (nullptr == pRoot->_pLeft&&nullptr == pRoot->_pRight)
		{
			if (Scount == count)
				return true;
			else
				return false;
		}
		return _CheckNum(pRoot->_pLeft, count, Scount) && _CheckNum(pRoot->_pRight, count, Scount);
	}
private:
	Node *_pHead;
};
void TestRBTree()
{
	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	RBTree<int, int> t;
	for (int idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
		t.Insert(a[idx], idx);

	t.InOrder();
	
	RBTreeIterator<int, int> it = t.Begin();
	while (it != t.End())
	{
		cout << it.getNode()->_key << " ";
		it++;
	}
	cout << endl;
	it = t.End();
	--it;
	cout << it.getNode()->_key << endl;
	for (it = t.End(); it != t.Begin(); --it)
	{
		cout << it.getNode()->_key << " ";
	}
	cout << endl;
	cout << t.CheckRBTree() << endl;
}
int main()
{
	TestRBTree();
	system("pause");
}