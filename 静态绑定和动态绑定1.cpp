#include <iostream>
#include <typeinfo>
using namespace std;

class Base
{
public:
	Base(int data = 10) :ma(data) {}

	 void show() { cout << "Base::show()" << endl; }

	 void show(int) { cout << "Base::show(int)" << endl; }
protected:
	int ma;
};
class Derive : public Base
{
public:
	Derive(int data = 20) :Base(data), mb(data) {}

	void show() { cout << "Derive::show()" << endl; }
private:
	int mb;
};
int main()
{
	Derive d(50);
	Base* pb = &d;
	pb->show();  //Base::show()
	/*
	汇编指令：
		pb->show();  //Base::show()
		00CA2928  mov         ecx,dword ptr [pb]
		00CA292B  call        Base::show (0CA12DAh)
	*/


	pb->show(10); //Base::show(int)
	/*
	汇编指令;
	pb->show(10); //Base::show(int)
	00CA2930  push        0Ah
	00CA2932  mov         ecx,dword ptr [pb]
	00CA2935  call        Base::show (0CA12B2h)
	*/

	cout << sizeof(Base) << endl;   //4
	cout << sizeof(Derive) << endl;  //8

	cout << typeid(pb).name() << endl;   //class Base *
	
	cout << typeid(*pb).name() << endl;    //class Base

	return 0; 
}