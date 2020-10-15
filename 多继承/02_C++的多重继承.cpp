#include <iostream>
using namespace std;


/*
c++的多重继承
经典的问题就是--菱形继承的问题，半圆继承等
      +----A-----+
	  ^          ^
	  |          |
继承  |          |继承
	  |          |
	  B--+     +-C
		 ^	   ^    
	继承 |     |继承
		 |     |  
	     +--D--+
*/


/*
如何理解c++多重继承？
好处，可以做更多的代码复用 D->B,C  
B *p = new D();
C *p = new D();

问题就是，会导致派生类有多份间接基类的数据，这是设计上面的问题
解决方案：从A直接继承的地方全部都使用虚继承virtual,可以通过画派生类的内存图看出来
*/

#if 0
class A 
{
public:
	A(int data) :ma(data) { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
protected:
	int ma;
};

class B:public A
{
public:
	B(int data) :mb(data),A(data) { cout << "B()" << endl; }
	~B() { cout << "~B()" << endl; }
protected:
	int mb;
};


class C :public A
{
public:
	C(int data) :mc(data), A(data) { cout << "C()" << endl; }
	~C() { cout << "~C()" << endl; }
protected:
	int mc;
};


class D:public B,public C
{
public:
	D(int data) :B(data),C(data),md(data) { cout << "D()" << endl; }
	~D() { cout << "~D()" << endl; }
protected:
	int md;
};

int main() {
	D d(10);
	/*
		A()
		B()
		A()
		C()
		D()
		~D()
		~C()
		~A()
		~B()
		~A()
	*/


	//cout << "sizeof(A) = " << sizeof(A) << endl;  //4
	//cout << "sizeof(B) = " << sizeof(B) << endl;  //8
	//cout << "sizeof(C) = " << sizeof(C) << endl;  //8
	//cout << "sizeof(D) = " << sizeof(D) << endl;  //20
	return 0;
}
#endif 


//改用虚基类，虚继承解决菱形多继承问题
#if 1
class A
{
public:
	A(int data) :ma(data) { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
protected:
	int ma;
};

class B :virtual public A
{
public:
	B(int data) :mb(data), A(data) { cout << "B()" << endl; }
	~B() { cout << "~B()" << endl; }
protected:
	int mb;
};


class C :virtual public A
{
public:
	C(int data) :mc(data), A(data) { cout << "C()" << endl; }
	~C() { cout << "~C()" << endl; }
protected:
	int mc;
};


class D :public B, public C
{
public:
	//增加对ma的初始化
	D(int data) :B(data), C(data), md(data),A(data) { cout << "D()" << endl; }
	~D() { cout << "~D()" << endl; }
protected:
	int md;
};

int main() {
	//直接编译error C2512: “A::A”: 没有合适的默认构造函数可用
	//ma不再属于B,C而是直接属于D，在D的构造函数初始化列表进行初始化
	//如果不指定那就要保证A有默认构造
	D(10);

	/*
	A()
	B()
	C()
	D()
	~D()
	~C()
	~B()
	~A()
	*/

	return 0;
}

//使用工具查看一下类D的内存分布
/*
D:\develop\code\vs2019\1008STL>cl "02_C++的多重继承.cpp" /d1reportSingleClassLayoutD

class D size(24):
		+---
 0      | +--- (base class B)
 0      | | {vbptr}
 4      | | mb
		| +---
 8      | +--- (base class C)
 8      | | {vbptr}
12      | | mc
		| +---
16      | md
		+---
		+--- (virtual base A)
20      | ma
		+---

D::$vbtable@B@:
 0      | 0
 1      | 20 (Dd(B+0)A)

D::$vbtable@C@:
 0      | 0
 1      | 12 (Dd(C+0)A)
vbi:       class  offset o.vbptr  o.vbte fVtorDisp
			   A      20       0       4 0
*/


#endif



