#include<iostream>
#include<cassert>
using namespace std;
template<class K, class V>
struct BSTNode
{
	BSTNode(const K& key=0, const V& value=0)
	: _pLeft(NULL)
	, _pRight(NULL)
	, _pParent(NULL)
	, _key(key)
	, _value(value)
	{}
	BSTNode<K, V>* _pParent;
	BSTNode<K, V>* _pLeft;
	BSTNode<K, V>* _pRight;

	K _key;
	V _value;
};

//
template<typename K, typename V>
class BinaryIterator
{
private:
	typedef BSTNode<K, V> Node;
	typedef BinaryIterator<K, V/*, K&, V&*/> Iterator;
public:
	BinaryIterator(Node *pNode = nullptr) :_pNode(pNode)
	{}
	Iterator &operator++()
	{
		_Increase(_pNode);
		return *this;
	}
	Iterator &operator--()
	{
		_Decrease(_pNode);
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
	Node *FindMax(Node *pCur)
	{
		assert(pCur);
		while (pCur->_pRight)
			pCur = pCur->_pRight;
		return pCur;
	}
	Node *FindMin(Node *pCur)
	{
		assert(pCur);
		while (pCur->_pLeft)
			pCur = pCur->_pLeft;
		return pCur;
	}
	void _Increase(Node *pNode)
	{
		if (pNode->_pRight)
		{
			pNode = FindMin(pNode->_pRight);
		}
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
	void _Decrease(Node *pNode)
	{
		if (pNode->_pLeft)
		{
			pNode = FindMax(pNode->_pLeft);
		}
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
	Node *_pNode;
};
template<class K, class V>
class BinarySearchTree
{
	typedef BSTNode<K, V> Node;
	typedef BinarySearchTree<K, V> Self;
public:
	
	BinarySearchTree()
		: _pRoot(NULL), _pHead(new Node())
	{}

	BinarySearchTree(const Self& bst)
	{		
		_pRoot=_Init(bst._pRoot);
	}
	Self& operator=(const Self& bst)
	{
		if (this != &bst)
		{
			if (_pRoot)
			{
				_DestroyTree(_pRoot);
			}
			_pRoot=_Init(bst._pRoot);
		}
		return *this;
	}
	~BinarySearchTree()
	{
		_DestroyTree(_pRoot);
	}

	// 查找递归和非递归
	Node * Find_Nor(const K& key)
	{
		return _Find_Nor(_pRoot,key);
	}
	Node * Find(const K& key)
	{
		return _Find(_pRoot, key);
	}

	// 插入递归和非递归
	bool Insert_Nor(const K& key, const V& value)
	{
		if (NULL == _pRoot)
		{
			_pRoot = new Node(key, value);
			return true;
		}
		return _Insert_Nor(_pRoot, key,value);
	}
	bool Insert(const K& key, const V& value)
	{
		
		bool flag=_Insert(_pRoot,NULL,key, value);
		_pHead->_pParent = _pRoot;
		_pRoot->_pParent = _pHead;
		_pHead->_pLeft = FindMin(_pRoot);
		_pHead->_pRight = FindMax(_pRoot);
		return flag;
	}

	// 删除递归和非递归
	bool Remove_Nor(const K& key)
	{
		return _Remove_Nor(_pRoot,key/*,false*/);
	}
	bool Remove(const K& key)
	{
		bool flag= _Remove(_pRoot, key);
		_pHead->_pParent = _pRoot;
		_pRoot->_pParent = _pHead;
		_pHead->_pLeft = FindMin(_pRoot);
		_pHead->_pRight = FindMax(_pRoot);
		return flag;
	}

	void InOrder()
	{
		cout << "InOrder:";
		_InOrder(_pRoot);
		cout << endl;
	}
	Node * ToList()
	{
		Node *pre = NULL;
		_ToList(_pRoot, pre);
		return FindMin(_pRoot);
	}
	BinaryIterator<K, V> end()
	{
		return BinaryIterator<K, V> (_pHead);
	}
	BinaryIterator<K, V> begin()
	{
		return BinaryIterator<K, V>(FindMin(_pRoot));
	}
private:
	Node * _Find_Nor(Node *pRoot, const K &key)
	{
		Node *pCur = pRoot;
		while (pCur)
		{
			if (key < pCur->_key)
			{
				pCur = pCur->_pLeft;
			}
			else if (key>pCur->_key)
			{
				pCur = pCur->_pRight;
			}
			else
			{
				return pRoot;
			}
		}
		return NULL;
	}
	Node *FindMin(Node *pCur)
	{
		assert(pCur);
		while (pCur->_pLeft)
			pCur = pCur->_pLeft;
		return pCur;
	}
	Node *FindMax(Node *pCur)
	{
		assert(pCur);
		while (pCur->_pRight)
			pCur = pCur->_pRight;
		return pCur;
	}
	Node* _Find(Node* pRoot, const K& key)
	{
		if (NULL == pRoot)
		{
			return false;
		}
		if (key < pRoot->_key)
		{
			_Find(pRoot->_pLeft, key);
		}
		else if (key>pRoot->_key)
		{
			_Find(pRoot->_pRight, key);
		}
		else
		{
			return pRoot;
		}
	}
	bool _Insert_Nor(Node* pRoot, const K &key, const V &value)
	{
		while (pRoot)
		{
			if (key < pRoot->_key)
			{
				if (NULL == pRoot->_pLeft)
				{
					pRoot->_pLeft = new Node(key, value);
					return true;
				}
				else
					pRoot = pRoot->_pLeft;
			}
			else if (key>pRoot->_key)
			{
				if (NULL == pRoot->_pRight)
				{
					pRoot->_pRight = new Node(key, value);
					return true;
				}
				else
					pRoot = pRoot->_pRight;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	bool _Insert(Node* &pRoot, Node *pParent,const K& key, const V& value)
	{
		if (NULL == pRoot)
		{
			pRoot = new Node(key, value);
			
			pRoot->_pParent = pParent;
			return true;
		}
		if (key < pRoot->_key)
		{
			pParent = pRoot;
			_Insert(pRoot->_pLeft,pParent, key, value);
			
		}
		else if (key>pRoot->_key)
		{
			pParent = pRoot;
			_Insert(pRoot->_pRight, pParent, key, value);
		}
		else
		{
			return false;
		}
		return true;
	}
	
	bool _Remove(Node*& pRoot, const K& key)
	{
		if (NULL == pRoot)
		{
			return false;
		}
		if (pRoot->_key > key)
		{
			_Remove(pRoot->_pLeft, key);
		}
		else if (pRoot->_key < key)
		{
			_Remove(pRoot->_pRight, key);
		}
		else
		{
			if (NULL != pRoot->_pLeft&&NULL != pRoot->_pRight)
			{
				Node *pDel = pRoot;
				pRoot->_key = FindMin(pRoot->_pRight)->_key;
				_Remove(pRoot->_pRight, pRoot->_key);
			}
			else
			{
				Node *pDel = pRoot;
				if (pRoot->_pLeft != NULL)
				{
					pRoot = pRoot->_pLeft;
				}
				else
				{
					pRoot = pRoot->_pRight;
				}
				delete pDel;
			}
		}
		return false;
	}
	bool _Remove_Nor(Node *&pRoot, const K &key/*,bool flag*/)
	{
		Node *pCur = pRoot;
		Node *pParent = pCur;
		/*if (flag == true)
		{
			pCur = pCur->_pRight;
		}*/
		while (pCur)
		{
			if (key < pCur->_key)
			{
				pParent = pCur;
				pCur = pCur->_pLeft;
			}
			else if (key>pCur->_key)
			{
				pParent = pCur;
				pCur = pCur->_pRight;
			}
			else
			{
				if (pCur->_pLeft&&NULL==pCur->_pRight)
				{
					if (pCur = pParent->_pLeft)
					{
						Node *pTemp = pCur;
						pParent->_pLeft = pCur->_pLeft;
						delete pTemp;
					}
					else if (pCur = pParent->_pRight)
					{
						Node *pTemp = pCur;
						pParent->_pRight = pCur->_pLeft;
						delete pTemp;
					}
					return true;
				}
				else if (pCur->_pLeft&&pCur->_pRight)
				{
					Node *pTempPre = pCur;
					Node *pTemp = FindMin(pCur->_pRight);
					pCur->_key = pTemp->_key;
					_Remove_Nor(pCur->_pRight, pCur->_key);
					return true;
				}
				else 
				{
					if (pCur ==pParent->_pLeft)
					{
						Node *pTemp = pCur;
						pParent->_pLeft = pCur->_pRight;
						delete pTemp;
					}
					else if (pCur == pParent->_pRight)
					{
						Node *pTemp = pCur;
						pParent->_pRight = pCur->_pRight;
						delete pTemp;
						
					}
					return true;
				}
				
			}
		}
		return false;
	}
	void _InOrder(Node* pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_pLeft);
			cout << pRoot->_key << " ";
			_InOrder(pRoot->_pRight);
		}
	}
	Node *_Init(Node *pRoot)
	{	
		if (NULL != pRoot)
		{
			Node *pNewRoot = new Node(pRoot->_key, pRoot->_value);
			pNewRoot->_pLeft = _Init(pRoot->_pLeft);
			pNewRoot->_pRight = _Init(pRoot->_pRight);
			return pNewRoot;
		}
		return NULL;
	}
	void _DestroyTree(Node *pRoot)
	{
		if (pRoot)
		{
			_DestroyTree(pRoot->_pLeft);
			_DestroyTree(pRoot->_pRight);
			delete(pRoot);
			pRoot = NULL;	
		}
	}
	void _ToList(Node *pRoot, Node *&pre)
	{
		if (NULL==pRoot)
			return;
		Node *cur = pRoot;
		_ToList(pRoot->_pLeft,pre);
		if (pre)
		{
			pre->_pRight = cur;
		}
		cur->_pLeft = pre;
		pre = cur;
		_ToList(pRoot->_pRight,pre);
	}
public:
	Node* _pRoot;
	Node *_pHead;
};

// 测试非递归的三种情况
void Test1()
{
	BinarySearchTree<int, int> bst;
	
	int a[] = { 5, 3,4,1, 7, 8, 2, 6, 0, 9 };
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
		bst.Insert(a[i],a[i]);
	}
	BinarySearchTree<int, int> bstcpy;
	bstcpy = bst;
	BSTNode<int, int> *list=bst.ToList();	
	bstcpy.InOrder();
	bst.InOrder();
	bst.Remove_Nor(6);
	bst.InOrder();
	cout << bst.Find(10) << endl;
	cout << bst.Find_Nor(5) << endl;

}

// 测试递归的三种情况
void Test2()
{
	BinarySearchTree<int, int> bst;
	int a[] = { 5, 3,4, 1, 7, 8, 2, /*6,*/ 0, 9 };
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
		bst.Insert(a[i], a[i]);
	}
	//BinaryIterator<int, int> it(bst._pRoot);
	////it = ++bst.end();
	//BSTNode<int, int> *start = bst.Find(3);
	//for (it=start; it != bst.end(); it++)
	//{
	//	cout << it._pNode->_key << " ";
	//}
	cout << endl;
	bst.InOrder();
	bst.Remove(5);
	bst.InOrder();
	cout << bst.Find(10) << endl;
	cout << bst.Find_Nor(5) << endl;

}
int main()
{
	Test1();
	//Test2();
	system("pause");
}