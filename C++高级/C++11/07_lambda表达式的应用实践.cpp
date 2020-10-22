#include <iostream>
#include <algorithm>
#include <map>
#include <functional>
#include <memory>
#include <queue>
#include <time.h>
#include <vector>
using namespace std;

/*
lambda表达式应用实践

既然lambda表达式只能使用在语句党中，如果想跨语句使用之前定义好的lambda表达式，
怎么办？用什么类型来表示lambda表达式？
当然是使用function类型来表示函数对象的类型啦
bind1st/bind2nd bind绑定器 最终产生的都是一个函数对象，当然使用function


lambda表达式 -》 函数对象 -》 用function
*/




#if 0
int main()
{
	map<int, function<int(int, int)>> caculateMap;
	caculateMap[1] = [](int a, int b)->int {return a + b; };
	caculateMap[2] = [](int a, int b)->int {return a - b; };
	caculateMap[3] = [](int a, int b)->int {return a * b; };
	caculateMap[4] = [](int a, int b)->int {return a / b; };
	cout << "请选择1,2,3,4:";
	int choice;
	cin >> choice;
	caculateMap[1] = [](int a, int b)->int {return a + b; };
	cout << "10 + 15 = " << caculateMap[choice](10, 15) << endl;

	return 0;

}
#endif


#if 0
int main()
{
	srand((unsigned int)time(nullptr));
	vector<int> vec1;
	for (int i = 0; i < 20; i++) {
		vec1.push_back(rand() % 100 + 1);
	}
	vector<int> vec2;
	for (int i = 0; i < 20; i++) {
		vec2.push_back(rand() % 100 + 1);
	}
	function<bool(int, int)> func1 =
		[](int a, int b)->bool
	{
		return a > b;
	};
	sort(vec1.begin(), vec1.end(), func1);
	sort(vec2.begin(), vec2.end(), func1);
	for (int val : vec1) {
		cout << val << " ";
	}
	cout << endl;
	for (int val : vec2) {
		cout << val << " ";
	}
	cout << endl;

	return 0;
}
#endif



#if 0


int main()
{
	//智能指针自定义删除器,智能指针默认是delete p;
	//FILE *  fclose(FILE*)
	//这里可以自定义个函数对象，但是比较麻烦
	//使用lambda表达式吧

	//都可以

	//function<void(FILE*)> func = [](FILE* p)->void {fclose(p); };
	//unique_ptr<FILE, function<void(FILE*)>> ptr1(fopen("1022data.txt","w"),func);

	unique_ptr<FILE, function<void(FILE*)>> ptr1(fopen("1022data.txt", "w"),
		[](FILE* p)->void
	{
		fclose(p);
	});
	return 0;
}



#endif


#if 1
class Data
{
public:
	Data(int val1 = 10, int val2 = 10) :ma(val1), mb(val2) {}
	int ma;
	int mb;
};

//优先级队列
int main()
{
	/*template <class _Ty,
	class _Container = vector<_Ty>,
	class _Pr = less<typename _Container::value_type>>
	*/
	//第三个参数是函数对象类型
	//priority_queue<Data> queue;

	using FUNC = function<bool(Data&, Data&)>;
	priority_queue<Data, vector<Data>, FUNC>
		maxHeap(
			[](Data& d1, Data& d2)->bool {
				return d1.ma > d2.ma;
			});
	maxHeap.push(Data(10, 20));
	maxHeap.push(Data(15, 15));
	maxHeap.push(Data(20, 10));


	return 0;
}

#endif