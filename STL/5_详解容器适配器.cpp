#include <iostream>
#include <stack>  //stack
#include <queue>  //queue  priority_queue
using namespace std;
//容器适配器stack,queue,priority_queue使用频率非常高


/*
stack和queue底层依赖deque
priority_queue底层依赖vector
*/



/*
标准容器 - 容器适配器 => 有一种设计模式，就叫做适配器模式
在这里相当于代理模式，如stack里面的push代理了deque里面的push_back
怎么理解这个适配器？
1.适配器底层没有自己的数据结构，它是另外一个容器的封装，它的方法全部由底层依赖的容器进行实现的
2.没有实现自己的迭代器

template<typename T, typename Container=deque<T>>
class Stack
{
public:
	void push(const T &val) { con.push_back(val); }
	void pop() { con.pop_back(); }
	T top()const { return con.back(); }
private:
	Container con;
};

stack: push入栈  pop出栈  top查看栈顶元素  empty判断栈空  size返回元素个数
queue: push入队  pop出队  front查看队头元素 back查看队尾元素  empty判断队空   size返回元素个数
priority_queue: push入队  pop出队  top查看队顶元素  empty判断队空  size返回元素个数  默认：大根堆

queue => 依赖deque  为什么不依赖vector呢？？？
stack => 依赖deque  为什么不依赖vector呢？？？
答案：
1.vector的初始内存使用效率太低了！没有deque好 --考虑效率 
如果存放int数据，queue<int> stack<int>
如果底层使用vector，数据扩容操作是 0-1-2-4-8，效率太低啦
如果使用deque，第一维默认是2，第二维默认4096，deque 4096/sizeof(int) = 1024个int数据
2.对于queue来说，需要支持尾部插入，头部删除，O(1)  如果queue依赖vector，其出队效率很低
3.vector需要大片的连续内存，而deque只需要分段的内存，当存储大量数据时，显然deque对于内存的利用率更好一些

priority_queue => vector 为什么依赖vector？？？
答案：底层默认把数据组成一个大根堆结构  在一个内存连续的数组上构建一个大根堆或者小根堆的
构建大根堆小根堆我们通过计算index下标之间的对应关系进行计算。所以必须是连续的空间

优先级队列解决topk问题
*/
int main()
{
#if 1
	priority_queue<int> pque;   //默认是大根堆
	for (int i = 0; i < 20; ++i)
	{
		pque.push(rand() % 100 + 1);
	}
	cout << pque.size() << endl;
	while (!pque.empty())
	{
		cout << pque.top() << " ";
		pque.pop();
	}
	//96 92 82 79 70 68 65 63 62 59 46 43 42 37 35 28 28 25 6 1


	cout << endl;
	cout << "---------------------------" << endl;
#endif
	
	//queue<int> que;
	//for (int i = 0; i < 20; ++i)
	//{
	//	que.push(rand() % 100 + 1);
	//}
	//cout << que.size() << endl;
	//while (!que.empty())
	//{
	//	cout << que.front() << " ";
	//	que.pop();
	//}

	//cout << endl;
	//cout << "---------------------------" << endl;


	//stack<int> s1;
	//for (int i = 0; i < 20; ++i)
	//{
	//	s1.push(rand() % 100 + 1);
	//}

	//cout << s1.size() << endl;

	//while (!s1.empty())
	//{
	//	cout << s1.top() << " ";
	//	s1.pop();
	//}


	return 0;
}