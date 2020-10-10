#include <iostream>
#include <queue>
#include <vector>
#include <set>
using namespace std;


/*
c++11里面的Lambda表达式就是对函数对象的简化
c++11里面的foreach就是对迭代器iterator的简化
*/


/*
使用函数对象举例1
使用优先级队列priority_queue默认大根堆，我们使用小根堆
*/


int main() {

	//默认大根堆
	//78 69 67 64 62 58 41 34 24 0
	priority_queue<int> que1;  //priority_queue容器适配器底层依赖vector容器
	for (int i = 0;i < 10;i++) {
		que1.push(rand() % 100);
	}
	while (!que1.empty()) {
		cout << que1.top() << " ";
		que1.pop();
	}
	cout << endl;
	
	cout << "=======================" << endl;
	/*
	查看priority_queue底层
	template <class _Ty, class _Container = vector<_Ty>, 
		class _Pr = less<typename _Container::value_type>>
	class priority_queue {}  //默认大根堆
	修改第三个参数，所以前两个参数我们也需要传参
	*/
	//来一个小根堆
	//c++11使用using 也是给类型起别名，可以代器typedef
	//5 27 27 36 42 45 61 81 91 95

	using MinHeap = priority_queue<int, vector<int>, greater<int>>;
	MinHeap que2;
	//priority_queue<int, vector<int>, greater<int>> que2;
	for (int i = 0;i < 10;i++) {
		que2.push(rand() % 100);
	}
	while (!que2.empty()) {
		cout << que2.top() << " ";
		que2.pop();
	}
	cout << endl;


	return 0;
}