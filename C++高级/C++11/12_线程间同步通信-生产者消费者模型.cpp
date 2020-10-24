#include <iostream>
#include <thread>
#include <condition_variable>
#include <queue> //C++ STL所有得容器都不支持线程安全
#include <mutex>
using namespace std;

//C++11语言级别的条件变量

/*
C++多线程编程 - 线程间的同步通信机制
多线程编程两个问题：
1.线程间的互斥
竟态条件 =》 临界区代码段 =》 保证原子操作 =》互斥锁mutex 或者 轻量级的无锁实现CAS
linux下使用strace命令跟踪 strace ./a.out mutex => pthread_mutex_t
2.线程间的同步通信
生产者，消费者线程模型
*/


std::mutex mtx;  //定义互斥锁，做线程间得互斥操作
std::condition_variable cv;  //定义条件变量,做线程间的同步通信操作

//生产者生产一个物品，通知消费者消费一个；消费完了，消费者再通知生产者继续生产物品
class Queue
{
public:
	void put(int val)  //生产物品
	{
		unique_lock<std::mutex> lck(mtx);
		while (!que.empty())
		{
			//que不为空，生产者应该通知消费者去消费，消费完了，再继续生产
			//生产者线程#1进入等待状态，并且#2把mtx互斥锁释放掉
			cv.wait(lck);
		}
		que.push(val);
		
		/*
		notify_one:通知另外的一个线程的
		notify_all:通知其他所有线程的
		通知其他所有的线程，我生产了一个物品，你们赶紧消费吧
		其他线程得到该通知，就会从等待状态 =》 阻塞状态 =》 获取互斥锁才能继续执行
		*/
		cv.notify_all(); 
		cout << "生产者 生产：" << val << "号物品" << endl;
	}
	int get()  //消费物品
	{
		unique_lock<std::mutex> lck(mtx);
		while (que.empty())
		{
			//消费者线程发现que是空的，通知生产者线程先生产物品
			//#1进入等待状态 #2把互斥锁mtx释放掉
			cv.wait(lck);
			//收到notify后从等待=》阻塞 ，获取到互斥锁后，继续往下执行
		}
		int val = que.front();
		que.pop();
		cv.notify_all();   //通知其他线程我消费完了，赶紧生产吧
		cout << "消费者 消费：" << val << "号物品" << endl;
		return val;
	}
private:
	queue<int> que;
};



void producer(Queue* que)//生产者线程
{
	for (int i = 1; i <= 10; ++i)
	{
		que->put(i);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
void consumer(Queue*  que)//消费者线程
{
	for (int i = 1; i <= 10; ++i)
	{
		que->get();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
int main()
{
	Queue que;  //两个线程共享得队列
	std::thread t1(producer,&que);
	std::thread t2(consumer,&que);

	t1.join();
	t2.join();

	return 0;
}