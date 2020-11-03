#include <iostream>
#include <typeinfo>
#include <string>
#include <functional>
#include <vector>
#include <thread>  // C++11 thread
using namespace std;
using namespace placeholders;
/*
function类模板
bind函数模板
*/

// C++11 bind绑定器 => 返回的结果还是一个函数对象
#if 1
void hello(string str) { cout << str << endl; }
int sum(int a, int b) { return a + b; }
class Test
{
public:
	int sum(int a, int b) { return a + b; }
};

int main()
{
	// #1 bind使用方法1
	// bind 是函数模板，可以自动推演模板类型参数
	bind(hello, "hello bind!")();
	cout << bind(sum, 10, 20)() << endl;
	
	// 对于C函数来说，函数名字前面加不加&都可以，C++的类内部成员函数，一定要取地址&Test::sum
	cout << bind(&Test::sum, Test(), 20, 30)() << endl;

	// 存在的问题就是bind绑定器只能使用在当前语句中，你如果还想继续使用，必须重新写
	// 为了将绑定器bind的类型留下来，就要将function和bind综合起来
	
	// #2 bind使用方法2
	// 参数占位符
	// placeholders::_1
	bind(hello, _1)("hello placeholders!");
	cout << bind(sum, _1, _2)(30, 80) << endl;

	// 此处就把bind绑定器返回的binder复用起来了，将类型保留下来
	function<void(string)> func1 = bind(hello, _1);
	function<int(int, int)> func2 = bind(sum, _1, _2);
	function<void()> func3 = bind(hello, "hello world 666666666");

	cout << "----function----" << endl;
	func1("hello function bind1");
	func1("hello function bind2");
	func1("hello function bind3");
	cout << func2(200, 300) << endl;
	cout << func2(100, 300) << endl;
	cout << func2(500, 300) << endl;
	func3();

	return 0;
}
/*
hello bind!
30
50
hello placeholders!
110
----function----
hello function bind1
hello function bind2
hello function bind3
500
400
800
hello world 666666666
*/
#endif
/////////////////////////////////////////////////////////
#if 0

// 类的成员方法不可以用做线程函数，但是可以通过bind绑定器做到
// 类的成员方法和C函数不同之处是类的成员函数需要一个对象，我们把this绑定进去啦
/*
muduo库 threadpool.cc thread.cc bind function
*/

// 线程类
class Thread
{
public:
	Thread(function<void(int)> func, int no)
		:_func(func), _no(no) {}
	thread start()
	{
		thread t(_func,_no); // 底层调用_func(_no);
		return t; // 在主线程join等待子线程运行完毕
	}
private:
	function<void(int)> _func;
	int _no;
};

// 线程池类
class ThreadPool
{
public:
	ThreadPool() {}
	~ThreadPool() 
	{
		// 释放Thread对象占用的堆资源
		for (int i = 0; i < _pool.size(); ++i)
		{
			delete _pool[i];
		}
	}

	// 开启线程池
	void startPool(int size)
	{
		for (int i = 0; i < size; ++i)
		{
			_pool.push_back(
				new Thread(bind(&ThreadPool::runInThread, this, _1), i));
			// 通过bind返回的绑定器，是一个没有返回值，但是需要一个int的参数
		}
		for (int i = 0; i < size; ++i)
		{
			_handler.push_back(_pool[i]->start());
		}

		// 等待所有子线程运行完成后，主线程再结束
		for (thread& t : _handler)
		{
			t.join();
		}
	}
private:
	vector<Thread*> _pool;
	vector<thread> _handler;  // 所有的线程句柄

	// 把这个runInThread这个成员方法充当线程函数
	// 无论是C++ thread 还是 Linux pthread_create他们的线程函数都是C语言的函数
	// 不可以是C++的类的成员函数,因为C的函数指针接收不了类的成员函数地址，使用bind解决这个问题
	void runInThread(int id)
	{
		cout << "call runInThread! id:" << id << endl;
	}
};
int main()
{
	ThreadPool pool;
	pool.startPool(10);

	return 0;
}
/*
call runInThread! id:0
call runInThread! id:5
call runInThread! id:6
call runInThread! id:8
call runInThread! id:2
call runInThread! id:1
call runInThread! id:7
call runInThread! id:3
call runInThread! id:9
call runInThread! id:4
*/

#endif
