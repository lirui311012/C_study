#include <iostream>
#include <memory>  //智能指针的头文件
#include <functional>  //函数对象的头文件
using namespace std;

/*
智能指针的删除器 deletor
智能指针：能够保证资源的绝对释放 
给智能指针托管new出来的堆区空间如new int,  释放方式delete
							new int[] 释放方式delete[]
					用智能指针管理的不是一个内存资源，而是一个文件资源，释放文件的方式也不同
给智能指针自定义删除器，指导智能指针如何去释放资源
*/

//unique_ptr(不带引用计数)和shared_ptr(带引用计数)都可以提供自定义删除器

/*
unique_ptr底层
~unique_ptr(){是一个函数对象的调用 deletor(ptr);}
//deletor对象就是Deletor<T>定义的一个函数对象，调用小括号重载函数,默认删除释放资源的方式delete
unique_ptr默认的Deletor如下：
template<typename T>
class default_delete
{
public:
	void operator()(T *ptr)
	{
		delete ptr;
	}
};
*/
class A
{
public:
	A() { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
	int ma;
};

template<typename T>
class MyDeletor
{
public:
	void operator()(T *ptr) const 
	{
		cout << "MyDeletor::void operator()(T *ptr) const " << endl;
		delete[]ptr;
	}
};

template<typename T>
class MyFileDeletor
{
public:
	void operator()(T* ptr) const
	{
		cout << "MyFileDeletor::void operator()(T *ptr) const " << endl;
		fclose(ptr);
	}
};
int main()
{
	//默认的删除器是delete，所以就会出现问题
	//unique_ptr<A> ptr(new A[5]);  //程序崩溃
	//unique_ptr<A, MyDeletor<A>> ptr1(new A[2]); //成功
	//unique_ptr<FILE, MyFileDeletor<FILE>> ptr2(fopen("data.txt","w")); //成功

	/*
		A()
		A()
		MyFileDeletor::void operator()(T *ptr) const
		MyDeletor::void operator()(T *ptr) const
		~A()
		~A()
	*/

	///////////////////////////////////////////////////////////////////////

	//发现上面的两个类class MyDeletor和class MyFileDeletor只有在定义对象的时候使用
	//其他的地方都没有使用过，这种类一多，连名字怎么起，都不知到，乱套了

	//c++11 lambda表达式
	//lambda表达式 =》函数对象
	//定义智能指针的时候需要指定删除器的类型
	//我们拥有lambda表达式的对象，并且使用function(函数对象类型)确定lambda表达式的类型

	//将上面的例子用lambda表达式再次完成一次
	unique_ptr<A,function<void (A *)>> ptr1(new A[3],
		[](A* p)->void
	{
		cout << "call lambda release new A[3]" << endl;
		delete[]p;
	});

	//使用function构造函数对象的类型
	unique_ptr<FILE, function<void(FILE*)>> ptr2(fopen("data.txt","w"),
		[](FILE *p)->void
	{
		cout << "call lambda release FILE *" << endl;
		fclose(p);
	});
	/*
		A()
		A()
		A()
		call lambda release FILE *
		call lambda release new A[3]
		~A()
		~A()
		~A()
	*/

	//无敌

	return 0;
}
