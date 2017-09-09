#include <iostream>
using namespace std;
#include "LinkQueue.h"
void test()
{
	LinkQueue<int> queue;
	int init[10] = { 1, 3, 6, 8, 9, 2, 0, 5, 4, 7 };

	for (int i = 0; i < 10; i++)
	{
		queue.Append(init[i]);
	}
	queue.Print();

	queue.Delete();
	queue.Print();

	cout << queue.GetFront() << endl;
	queue.Print();

	queue.MakeEmpty();
	queue.Print();
	//queue.Delete();
}
int main()
{
	test();
	
	system("pause");

	return 0;
}

