#include <iostream>
#include <memory>
using namespace std;

/*
shared_ptr和weak_ptr

shared_ptr:强智能指针->可以改变资源的引用计数
weak_ptr:弱智能指针->不会改变资源的引用计数
这俩都是带引用计数的智能指针

shared_ptr提供一个成员方法use_count()可以打印资源的引用计数

weak_ptr观察->shared_ptr观察->资源(内存)
弱智能指针观察强智能指针，强智能指针观察资源
所以弱智能指针weak_ptr没有提供operator*和operator->的重载函数
*/


//强智能指针shared_ptr循环引用(交叉引用)是什么问题，会产生什么结果，怎么解决这个问题？
//造成new出来的资源无法释放！！！资源的内存泄漏
//解决方案：定义对象的时候用强智能指针，引用对象的时候用弱智能指针
class B;
class A {
public:
	A() { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
	shared_ptr<B> _ptrb;
};
class B {
public:
	B() { cout << "B()" << endl; }
	~B() { cout << "~B()" << endl; }
	shared_ptr<A> _ptra;
};


int main01()
{
	shared_ptr<A> pa(new A());
	shared_ptr<B> pb(new B());
	cout << pa.use_count() << endl;
	cout << pb.use_count() << endl;
	return 0;
}
/*
A()
B()
1
1
~B()
~A()
//引用计数为1，出了main函数作用范围，回收main栈帧空间
//两个智能指针对象回收，引用计数-1 ==>0 delete掉new申请的空间
*/



int main02()
{
	shared_ptr<A> pa(new A());
	shared_ptr<B> pb(new B());

	pa->_ptrb = pb;
	pb->_ptra = pa;

	cout << pa.use_count() << endl;
	cout << pb.use_count() << endl;
	return 0;
}
/*
A()
B()
2
2
//引用计数为2，智能指针对象出作用域，引用计数-1,后为1，未达到delete释放空间的条件内存泄漏
*/


//解决强智能指针的循环引用问题：
//定义对象的时候使用强智能指针shared_ptr，引用对象的时候用弱智能指针weak_ptr
//如下
class D;
class C {
public:
	C() { cout << "C()" << endl; }
	~C() { cout << "~C()" << endl; }

	void testA() { cout << "非常好用的方法!" << endl; }

	weak_ptr<D> _ptrb;  //引用对象的时候使用弱智能指针
};
class D {
public:
	D() { cout << "D()" << endl; }
	~D() { cout << "~D()" << endl; }
	void func() {
		//场景想利用_ptra弱智能指针去调用类C里面的方法，不可以直接调用哦
		//错误使用方式
		//_ptra->testA();  //不可以这样，weak_ptr只是一个观察者
						   //弱智能指针不能使用资源
		                   //weak_ptr没有提供operator*和operator->重载函数


		//正确使用方法  //weak_ptr只是一个观察者
		shared_ptr<C> ps = _ptra.lock();  //提升方法,提升成一个强智能指针
						//weak_ptr弱智能指针是一个观察者，随着程序的运行，他所观察的资源可能已经被释放掉
						//所以这个提升，可能成功，可能失败
		if (ps != nullptr) {
			ps->testA();  //提升成功，引用计数已经+1，别人也是放不了该资源
		}
	}
	weak_ptr<C> _ptra;
};
int main() {
	shared_ptr<C> ptra(new C());
	shared_ptr<D> ptrb(new D());  //定义对象使用强智能指针

	ptra->_ptrb = ptrb;
	ptrb->_ptra = ptra;
	ptrb->func(); //打印，非常好用的方法

	cout << ptra.use_count() << endl;
	cout << ptrb.use_count() << endl;

	return 0;
}
/*
C()
D()
非常好用的方法!
1
1
~D()
~C()
*/

