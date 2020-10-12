#include <iostream>
using namespace std;

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


//智能指针：保证能够做到资源的自动释放!!!
//利用栈上的对象出作用域自动析构的特征，来做到资源的自动释放的。
//CSmartPtr<int> *p = new CSmartPtr<int>(new int);
//delete p;
//语法角度没错误，但是失去了智能指针的作用，p指针依然是裸指针


#if 0
int main() {

	//裸指针
	//data段，heap(手动申请，手动释放)，stack
	int* p = new int();
	*p = 10;
	cout << *p << endl;
	delete p;
	p = nullptr;

	return 0;
}
#endif


#if 1

//实现一个简单的智能指针，需要提供*p,p->这些运算符重载函数
//operator*,operator->
template <typename T>
class CSmartPtr 
{
public:
	CSmartPtr(T *ptr)
		:mptr(ptr) {}
	~CSmartPtr() { delete mptr; }
	T& operator*() { return *mptr; }
	T* operator->() { return mptr; }
private:
	T* mptr;
};
int main() {
	//int *p = new int;
	CSmartPtr<int> ptr1(new int);
	*ptr1 = 20;

	class Test {
	public:
		void test() { cout << "call Test::test()" << endl; }
	};
	CSmartPtr<Test> ptr2(new Test());
	ptr2->test();   //(ptr2.operator->())->test();
	(*ptr2).test();

	return 0;
}

#endif