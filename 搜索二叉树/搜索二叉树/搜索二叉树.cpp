#include"BinarySearchTree.h"
int main()
{
	BinarySearchTree<int> bst;
	int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
		bst.insert(a[i]);
	}
	bst.InOrder();
	BinarySearchTree<int> bst1(bst);
	bst1.InOrder();
	cout << bst.findMax() << endl;
	system("pause");
}