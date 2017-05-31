#include <iostream>
using namespace std;

#include "LinkStack.h"
void test()
{
	LinkStack<int> stack;
	int init[10] = { 1, 3, 5, 7, 4, 2, 8, 0, 6, 9 };
	for (int i = 0; i < 10; i++)
	{
		stack.Push(init[i]);
	}
	stack.Print();

	cout << stack.Pop() << endl;
	stack.Print();

	cout << stack.GetTop() << endl;
	stack.Print();

	cout << stack.Pop() << endl;
	stack.Print();

	stack.MakeEmpty();
	stack.Print();

	//stack.Pop();
}
int main()
{
	test();

	system("pause");
	return 0;
}

