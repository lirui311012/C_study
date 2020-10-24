#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
using namespace std;

/*
lock_guard   unique_lock   condition_variable
1.lock_guard和unique_lock
2.condition_variable  wait和notify方法
*/

std::mutex mtx;  //定义了互斥锁
std::condition_variable cv;  //定义了条件变量
//pthread_mutex_t
//pthread_condition_t

////////////////////////////////////////////////////////////////

//lock_guard和unique_lock
int main01()
{
	//它不仅可以使用在简单的临界区代码段的互斥操作中，还能用在函数调用过程中
	unique_lock<std::mutex> lck(mtx);
	cv.wait(lck);//#1使线程进入等待状态，#2调用lck.unlock()可以把mtx释放掉
	
	//lock_guard<std::mutex> guard(mtx);
	/*	不可能用在函数参数传递或者返回过程中
		因为lock_guard的operator=和拷贝构造被delete了
		只能用在简单的临界区代码段的互斥操作中
	*/

	//mtx.lock();
	//mtx.unlock();

	return 0;
}
////////////////////////////////////////////////////////////////

int main()
{
	/*
	通知在cv上等待的线程，条件成立了，起来干活了！
	其他在cv上等待的线程，收到通知，从等待状态 =》 阻塞状态 =》 获取互斥锁 =》 线程继续往下执行
	*/
	cv.notify_all();

	return 0;
}