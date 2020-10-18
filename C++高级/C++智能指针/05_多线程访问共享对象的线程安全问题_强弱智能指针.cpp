#include <iostream>
#include <memory>
#include <map>
#include <algorithm>
#include <vector>
#include <thread>  //C++11提供的线程库
using namespace std;

/*
陈硕：C++ 非常著名的开源网络库muduo库，使用了大量的强弱智能指针,shared_ptr,weak_ptr
多线程访问共享对象的线程安全问题--强弱智能指针非常适合解决这种场景

shared_ptr,weak_ptr在线程里面通过资源的计数，来检测对象的生存状态
*/

class A {
public:
	A() { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
	void testA() { cout << "非常好用的方法!" << endl; }
};

//子线程
void handler01(A* q)
{
	q->testA();
}

//main线程
int main01()
{
	A* p = new A();
	//定义线程对象，随着线程对象的产生，线程已经启动
	thread t1(handler01, p);

	//睡两秒
	std::this_thread::sleep_for(std::chrono::seconds(2));

	delete p;

	t1.join();
	return 0;
}
/*
A()
非常好用的方法!
~A()

//没有问题哦
*/

///////////////////////////////////////////////////////

void handler02(A* q)
{
	std::this_thread::sleep_for(std::chrono::seconds(2));
	// q访问A对象的时候，需要侦测一下A对象是否存活
	//这就涉及到了，多线程访问共享资源的线程安全问题，一定要侦测对象是否存活
	//解决方案，给对象添加引用计数，使用强弱智能指针,shared_ptr,weak_ptr
	q->testA();
}


int main02()
{
	A* p = new A();
	thread t1(handler02, p);
	delete p;

	t1.join();
	return 0;
}
/*
A()
~A()
非常好用的方法!


//非常不合理，是有问题的
*/
///////////////////////////////////////////////////////

//给对象添加引用计数，不再使用裸指针，使用强弱智能指针,shared_ptr,weak_ptr

//引用对象使用弱智能指针
void handler03(weak_ptr<A> pw)
{
	std::this_thread::sleep_for(std::chrono::seconds(2));
	// q访问A对象的时候，需要侦测一下A对象是否存活
	//这就涉及到了，多线程访问共享资源的线程安全问题，一定要侦测对象是否存活
	//解决方案，给对象添加引用计数，使用强弱智能指针,shared_ptr,weak_ptr

	shared_ptr<A> ps = pw.lock();
	if (ps != nullptr)
	{
		ps->testA();
	}
	else {
		cout << "A对象已经析构，不能再访问!" << endl;
	}
}


int main03()
{
	//创建对象使用强智能指针
	{
		shared_ptr<A> p(new A());
		thread t1(handler03, weak_ptr<A>(p));

		//没join线程，所以必须设置线程分离
		t1.detach();  //线程分离
	}
	
	std::this_thread::sleep_for(std::chrono::seconds(20));

	//阻塞等待子线程结束
	//t1.join();
	return 0;
}
/*
A()
~A()
A对象已经析构，不能再访问!
*/

int main()
{
	//创建对象使用强智能指针
	{
		shared_ptr<A> p(new A());
		thread t1(handler03, weak_ptr<A>(p));

		//没join线程，所以必须设置线程分离
		t1.detach();  //线程分离
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}

	std::this_thread::sleep_for(std::chrono::seconds(20));

	//阻塞等待子线程结束
	//t1.join();
	return 0;
}
/*
A()
非常好用的方法!
~A()
*/

