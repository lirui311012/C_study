#include <iostream>
#include <thread> //多线程,语言级别
#include <list>
#include <mutex> //互斥锁，语言级别
using namespace std;


/*
C++ thread 模拟车站三个窗口卖票的程序

线程间的互斥 =》 互斥锁mutex =》 lock_guard封装mutex
*/

/*
多线程程序
竞态条件：多线程程序执行的结果是一致的，不会随着CPU对线程不同的调用顺序，而产生不同
的运行结果。
*/

/*
ticketCount--;
汇编指令
mov eax,ticketCount
sub eax,1
mov ticketCount,eax
*/

/////////////////////////////////////////////////////////////

#if 0

int ticketCount = 100; // 车站有100张车票，由三个窗口一起卖票

// 模拟卖票的线程函数
//这个函数存在多线程竞态条件
void sellTicket(int index)
{
	while (ticketCount > 0)
	{
		cout << "窗口:" << index << "卖出第:" << ticketCount << "张票!" << endl;
		ticketCount--;
		//睡100毫秒
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main()
{
	list<std::thread> tlist;
	for (int i = 1; i <= 3; ++i)
	{
		tlist.push_back(std::thread(sellTicket, i));
	}

	//等待所有子线程结束
	for (std::thread& t : tlist)
	{
		t.join();
	}

	cout << "所有窗口卖票结束！" << endl;
	return 0;
}
/*部分打印结果:
窗口:1卖出第:100张票!
窗口:2卖出第:100张票!
窗口:3卖出第:100张票!
窗口:2卖出第:97张票!
窗口:3卖出第:97张票!
窗口:1卖出第:97张票!
窗口:2卖出第:94张票!
窗口:3卖出第:94张票!
窗口:1卖出第:93张票!
窗口:2卖出第:91张票!
窗口:1卖出第:90张票!
窗口:3卖出第:90张票!
*/
#endif
/////////////////////////////////////////////////////////////

#if 0


int ticketCount = 100; // 车站有100张车票，由三个窗口一起卖票
std::mutex mtx;  //全局得一把互斥锁

// 模拟卖票的线程函数
void sellTicket(int index)
{

	while (ticketCount > 0)  // ticketCount=1  锁+双重判断
	{
		mtx.lock();
		if (ticketCount > 0) //双重判断  ticketCount=1
		{
			// 临界区代码段  =》  原子操作 =》 线程间互斥操作了 =》 mutex
			cout << "窗口:" << index << "卖出第:" << ticketCount << "张票!" << endl;
			//cout << ticketCount << endl;
			ticketCount--;
		}
		mtx.unlock();
		//睡100毫秒
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

}

int main()
{
	list<std::thread> tlist;
	for (int i = 1; i <= 3; ++i)
	{
		tlist.push_back(std::thread(sellTicket, i));
	}

	//等待所有子线程结束
	for (std::thread& t : tlist)
	{
		t.join();
	}

	cout << "所有窗口卖票结束！" << endl;
	return 0;
}


#endif

/////////////////////////////////////////////////////////////

#if 1

int ticketCount = 100;
std::mutex mtx;

/*
不适用互斥锁得lock和unlock，万一忘记unlock或者unlock代码没有执行，造成死锁

lock_guard ,  unique_lock
*/

void sellTicket(int index)
{

	while (ticketCount > 0)
	{
		{
			lock_guard<std::mutex> lock(mtx);  // 保证所有线程都能释放锁，防止死锁问题的发生
			/*
			lock_guard(const lock_guard&) = delete;
			lock_guard& operator=(const lock_guard&) = delete;
			lock_guard不支持拷贝构造和赋值重载函数
			所以不能用在函数调用中
			*/
			if (ticketCount > 0)
			{
				cout << "窗口:" << index << "卖出第:" << ticketCount << "张票!" << endl;
				ticketCount--;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main()
{
	list<std::thread> tlist;
	for (int i = 1; i <= 3; ++i)
	{
		tlist.push_back(std::thread(sellTicket, i));
	}

	//等待所有子线程结束
	for (std::thread& t : tlist)
	{
		t.join();
	}

	cout << "所有窗口卖票结束！" << endl;
	return 0;
}
/*
窗口:2卖出第:100张票!
窗口:1卖出第:99张票!
窗口:3卖出第:98张票!
窗口:2卖出第:97张票!
窗口:1卖出第:96张票!
窗口:3卖出第:95张票!
窗口:2卖出第:94张票!
窗口:1卖出第:93张票!
窗口:3卖出第:92张票!
窗口:2卖出第:91张票!
窗口:3卖出第:90张票!
窗口:1卖出第:89张票!
窗口:3卖出第:88张票!
窗口:2卖出第:87张票!
*/
#endif

