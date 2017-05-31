#include"circleQueue.h"

void test()
{
	Queue<int> *p = new Queue<int>(4);

	p->Enqueue(5);
	p->Enqueue(6);
	p->Enqueue(7);
	p->Enqueue(8);
	p->Enqueue(9);
	cout << *p << endl;
	Queue<int> a(10);
	a.Enqueue(1);
	a.Enqueue(2);
	a.Enqueue(3);
	a.Enqueue(4);
	cout << a << endl;
	Queue<int> b(a);
	cout << b << endl;
	Queue<int> c(3);
	c = a;
	cout << c << endl;
}
int main()
{
	test();
	system("pause");
}