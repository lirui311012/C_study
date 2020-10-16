#include <iostream>
using namespace std;
/*
智能指针无法替代裸指针的全部功能
*/

/*
概述
one
智能指针介绍
自己实现智能指针
two
不带引用计数的智能指针auto_ptr,scoped_ptr,unique_ptr
带引用计数的智能指针shared_ptr,weak_ptr
three
智能指针的交叉引用(循环引用)问题
多线程访问共享对象问题
自定义删除器
*/
/////////////////////////////////////////////////////////////////////////////


//智能指针：保证能够做到资源的自动释放!!!
//利用栈上的对象出作用域自动析构的特征，来做到资源的自动释放的。
//智能指针能够在对空间申请吗？CSmartPtr<int> *p = new CSmartPtr<int>(new int);
//delete p;
//语法角度没错误，但是失去了智能指针的作用，p指针依然是裸指针
int main01() {

	//裸指针
	//data段，heap(手动申请，手动释放)，stack
	int* p = new int();
	*p = 10;
	cout << *p << endl;
	delete p;
	p = nullptr;

	return 0;
}
/*
使用裸指针的弊端：
1.new申请空间后，忘记delete，就会导致该对空间丢掉，内存泄漏
2.或者说我们写了delete p;代码，但是在delete之前程序由于if(xxx)return -1
	写了delete但是没有执行到该代码，也是内存泄漏。
所以使用裸指针，申请空间后，首先要保证没有忘记delete，其次就是要保证delete语句能够执行得到
*/
/////////////////////////////////////////////////////////////////////////////

//实现一个简单的智能指针，需要提供*p,p->这些运算符重载函数
//operator*,operator->
template <typename T>
class CSmartPtr 
{
public:
	CSmartPtr(T *ptr)
		:mptr(ptr) {}
	~CSmartPtr() { delete mptr; }

	//operator*()和operator->()都是不带参数的，用返回值的结果进行*，和->
	T& operator*() { return *mptr; }
	T* operator->() { return mptr; }
private:
	T* mptr;
};
int main() {
	//int *p = new int;
	CSmartPtr<int> ptr1(new int);
	*ptr1 = 20;
	cout << *ptr1 << endl;
	class Test {
	public:
		void test() { cout << "call Test::test()" << endl; }
	};
	CSmartPtr<Test> ptr2(new Test());
	ptr2->test();   //(ptr2.operator->())->test();
	(*ptr2).test();

	return 0;
}
/*
20
call Test::test()
call Test::test()
*/

