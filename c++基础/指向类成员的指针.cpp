#include <iostream>
using namespace std;

class Test
{
public:
	void func() { cout << "call Test::func" << endl; }
	static void static_func() { cout << "Test::static_func" << endl; }

	int ma;
	static int mb;
};
int Test::mb;
int main()
{
	Test t1;
	Test* t2 = new Test();

	void (Test:: * fp1)() = &Test::func;
	(t1.*fp1)();
	(t2->*fp1)();
	cout << "=============" << endl;
	void(*fp2)() = &Test::static_func;
	(*fp2)();

#if 0
	int Test::* p = &Test::ma;
	t1.*p = 20;
	t2->*p = 30;
	cout << t1.*p << endl;
	cout << t2->*p << endl;
	cout << "=============" << endl;
	int* pp = &Test::mb;
	*pp = 200;
	cout << Test::mb << endl;
#endif

	delete t2;
	return 0;
}