#include <iostream>
#include <thread>  //多线程
using namespace std;

/*
C++语言级别的多线程编程=》代码可以跨平台 windows/linux/mac
thread/mutex/condition_variable条件变量
lock_quard/unique_lock自动加锁，解锁
atomic 原子类型 ，基于CAS操作的原子类型 线程安全的
sleep_for睡眠

   C++语言层面 thread
	windows				  linux        strace ./a.out看到程序启动的跟踪打印
	  |						|
	createThread		pthread_create
	本质上还是调用系统的API


线程内容：
一. 怎么创建启动一个线程
std::thread定义一个线程对象，传入线程所需要的线程函数和参数，
线程自动开启

二. 子线程如何结束
子线程函数运行完成，线程就结束了

三. 主线程如何处理子线程
t.join() : 阻塞等待t线程结束，当前线程继续往下运行
t.detach() : 把t线程设置为分离线程，主线程结束，整个进程结束，所有子线程都自动结束了！

*/

void threadHandle1(int time)
{
	//让子线程睡眠2秒
	std::this_thread::sleep_for(std::chrono::seconds(time));
	cout << "hello thread1" << endl;
}

void threadHandle2(int time)
{
	//让子线程睡眠2秒
	std::this_thread::sleep_for(std::chrono::seconds(time));
	cout << "hello thread2" << endl;
}
int main()
{
	//创建了一个线程对象,参数传入一个线程函数，新线程就开始运行了
	std::thread t1(threadHandle1, 3);
	std::thread t2(threadHandle2, 1);

	//主线程阻塞等待子线程t1结束，主线程继续往下运行
	t1.join();
	t2.join();

	//把子线程t1设置为分离线程，后面检查的时候就不再检查这个分离的线程了
	//t1.detach();

	cout << "main thread done!" << endl;

	//主线程运行完成，查看如果当前进程还有未完成的子线程，进程就会异常终止
	return 0;
}