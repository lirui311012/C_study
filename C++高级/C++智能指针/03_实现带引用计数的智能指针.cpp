#include <iostream>
#include <memory>
using namespace std;


//探究带引用计数的智能指针是如何解决多个智能指针对象去持有同一个资源 ? ? ? ?
/*
带引用计数的智能指针：shared_ptr 和 weak_ptr
shared_ptr 和 weak_ptr里面的引用计数是atomic_int,它通过CAS机制保证++,--的安全，
所以shared_ptr和weak_ptr都是支持线程安全的，可以直接使用在多线程里面

带引用计数：多个智能指针对象可以管理同一个资源
带引用计数：给每一个对象资源，匹配一个引用计数
智能指针 =》 资源的时候 =》 引用计数+1
智能指针 =》 不使用资源的时候 =》 引用计数-1 =》当引用计数!=0时 =》不释放资源
									   =》当引用计数==0时 =》释放资源
*/

/*
不带引用计数的智能指针，auto_ptr,unique_ptr
他俩的区别就是auto_ptr在用户没有感知的情况下进行了资源的持有全转移
而unique_ptr删除了拷贝构造函数和operator=重载函数
但是unique_ptr提供了带有右值引用参数的拷贝构造函数和operator=重载函数
这样可以在用户感知的情况下进行资源的转移，如
unique_ptr<int> ptr1(new int);
unique_ptr<int> ptr2(std::move(ptr1));
//std::move()传入一个左值引用，返回该类型的右值引用,调用unique_ptr的带有
//右值引用参数的拷贝构造函数。
*ptr2 = 20;
cout << *ptr2 << endl;  //正常
*ptr1 = 10;  //程序崩溃
不带引用计数的智能指针，都是只有一个智能指针对象可以去管理资源
*/

/////////////////////////////////////////////////////////////////////////

//模拟实现一个带引用计数的智能指针

//对资源进行引用计数的类
//这个实现也许逻辑上面有一些小问题，但是原理就是这样
template <typename T>
class RefCnt
{
public:
	RefCnt(T* ptr = nullptr)
		:mptr(ptr)
	{
		if (mptr != nullptr)
		{
			mcount = 1;
		}
	}
	void addRef() { mcount++; }  //增加资源的引用计数
	int delRef() { return --mcount; }
private:
	T* mptr;
	int mcount;  // atomic_int 原子整型类型通过CAS保证++,--的线程安全
};

template <typename T>
class CSmartPtr
{
public:
	CSmartPtr(T* ptr = nullptr)
		:mptr(ptr)
	{
		mpRefCnt = new RefCnt<T>(mptr); //给该资源建立一个引用计数对象
	}
	~CSmartPtr()
	{
		if (0 == mpRefCnt->delRef())
		{
			delete mptr;
			mptr = nullptr;
		}

	}
	CSmartPtr(const CSmartPtr<T>& src)
		:mptr(src.mptr), mpRefCnt(src.mpRefCnt)
	{
		if (mptr != nullptr)
			mpRefCnt->addRef();  //增加引用计数
	}
	CSmartPtr<T>& operator=(const CSmartPtr<T>& src)
	{
		if (this == &src)
			return *this;

		//由于当前这个智能指针对象不再使用原来的资源，所以当前所持有的
		//资源的引用计数-1,如果减一后为0，那么直接释放之前的空间
		//如果不为0，那就不用当前这个智能指针释放，还有其他智能指针对象持有资源
		if (0 == mpRefCnt->delRef())
		{
			delete mptr;
		}

		mptr = src.mptr;
		mpRefCnt = src.mpRefCnt;
		mpRefCnt->addRef();
		return *this;
	}
	T& operator*() { return *mptr; }
	T* operator->() { return mptr; }
private:
	T* mptr;  //指向资源的指针
	RefCnt<T>* mpRefCnt;  //指向该资源引用计数对象的指针
};


int main01() {

	CSmartPtr<int> ptr1(new int);
	CSmartPtr<int> ptr2(ptr1);
	CSmartPtr<int> ptr3;
	ptr3 = ptr2;
	*ptr1 = 100;
	cout << *ptr1 << " " << *ptr2 << " " << *ptr3 <<endl;  // 100 100 100

	return 0;
}

/*
CSmartPtr当前的实现方式就是shared_ptr的实现方式，只不过CSmartPtr在多线程下不支持线程安全
*/

