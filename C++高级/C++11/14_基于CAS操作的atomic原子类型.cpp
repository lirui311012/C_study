#include <iostream>
#include <thread>
#include <atomic> // 包含了很多原子类型
#include <list>
using namespace std;

/*
C++11
//对于count++,count--等操作，它不是线程安全的
窗口卖票   int count=100;

//通过互斥锁保证原子性
lock_guard<std::mutex> guard(mtx);
count++;

lock_guard<std::mutex> guard(mtx);
count--;

互斥锁是比较重的，临界区代码做的事情稍稍复杂，比较多-->没有办法，就得使用较重的互斥锁
系统理论：CAS来保证上面++， --操作的原子特性就足够了， 无锁操作
		exchange/swap    无锁队列 => CAS来实现的，
*/
volatile std::atomic_bool isReady = false;
volatile std::atomic_int mycount = 0;
/*
volatile防止多线程对共享变量进行缓存，大家访问的时候都是原始内存中变量的值
*/

void task()
{
	while (!isReady)
	{
		std::this_thread::yield(); // 线程出让当前的CPU时间片，等待下一次调度
	}

	for (int i = 0; i < 100; ++i)
	{
		mycount++;
	}
}
int main()
{
	list<std::thread> tlist;
	for (int i = 0; i < 10; ++i)
	{
		tlist.push_back(std::thread(task));
	}

	std::this_thread::sleep_for(std::chrono::seconds(3));
	isReady = true;

	for (std::thread& t : tlist)
	{
		t.join();
	}
	cout << "mycount:" << mycount << endl;

	return 0;
}