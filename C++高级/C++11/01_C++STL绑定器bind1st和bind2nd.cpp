#include <iostream>
#include <time.h>
#include <vector>     //vector动态数组
#include <algorithm>  //泛型算法
#include <functional> //c++库所有的函数对象
using namespace std;

/*
绑定器和函数对象operator()

1.C++ STL中的绑定器
bind1st : operator()的第一个形参变量绑定成一个确定的值
bind2nd : operator()的第二个形参变量绑定成一个确定的值


2.C++11从Boost库中引入了bind绑定器和function函数对象机制


3.lambda表达式 底层就是依赖函数对象的机制实现的
*/

template <typename Contaner>
void showContainer(Contaner & con)
{
	//需要加一个typename
	//编译到这里编译器不知道Contaner::后面的iterator是一个类型，还是一个变量
	//如果是一个类型，那么可以定义变量，如果本身就是一个变量，那么就不能定义变量
	//加上typename就是告诉编译器，iterator是一个类型，它可以定义变量，linux下g++也需要加typename
	typename Contaner::iterator it = con.begin();
	while (it != con.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
}

int main()
{
	vector<int> vec;
	//种一个随机数的种子
	srand((unsigned int)time(nullptr));
	for (int i = 0;i < 20;i++)
	{
		vec.push_back(rand() % 100 + 1);
	}
	showContainer(vec);
	sort(vec.begin(), vec.end()); //默认升序
	showContainer(vec);
	//sort(vec.begin(), vec.end(), 
	//	[](int a,int b)->bool
	//{
	//	return a > b;
	//});  //降序

	/*
	struct greater{
		constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const {
        return _Left > _Right;
    }
	};
	*/
	//greater 二元函数对象
	sort(vec.begin(),vec.end(),greater<int>());  //降序
	showContainer(vec);

	/*当前是降序
	需求，把70按顺序插入到vector容器中
	也就是说应当找第一个小于70的数字
	需要一个一元函数对象operator()(const T &val)
	但是greater和less都是二元函数对象
	greater : a>b  => 70 > b  =>bind1st
	less    : a<b  => a < 70  =>bind2nd
	STL里面绑定器(bin1st,bind2nd)+二元函数对象(greater,less) => 一元函数对象
	bind1st:+greater =>  bool operator()(70, const _Ty& _Right)  70 > b
	bind2nd:+less    =>  bool operator()(const _Ty& _Left, 70)   a < 70 
	*/
	//find_if底层需要的也是一个一元函数对象

	/*
	//#1
	auto it = find_if(vec.begin(),vec.end(),
		[](int b)->bool
	{
		return 70 > b;
	});*/

	//#2
	//auto it = find_if(vec.begin(), vec.end(), bind2nd(less<int>(), 70));

	//#3
	auto it = find_if(vec.begin(),vec.end(),bind1st(greater<int>(),70));
	//#1,2,3最终结果都是一样的
	if (it != vec.end())
	{
		vec.insert(it,70);
	}
	else
	{
		vec.push_back(70);
	}
	showContainer(vec);
	return 0;
}
/*
67 64 80 14 30 60 18 87 12 61 53 23 18 41 24 74 49 50 84 66
12 14 18 18 23 24 30 41 49 50 53 60 61 64 66 67 74 80 84 87
87 84 80 74 67 66 64 61 60 53 50 49 41 30 24 23 18 18 14 12
87 84 80 74 70 67 66 64 61 60 53 50 49 41 30 24 23 18 18 14 12
*/
