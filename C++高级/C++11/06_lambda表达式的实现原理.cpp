#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

/*
C++11 函数对象的升级版本 -》 lambda表达式
函数对象的缺点：
使用在反省算法参数传递  比较性质/自定义操作  优先级队列  智能指针的删除器 等

lambda表达式的语法：
[捕获外部变量](形参列表)->返回值{操作代码};

如果lambda表达式的返回值不需要，那么"->返回值"可以省略

[捕获外部变量]
[]:表示不捕获任何外部变量
[=]:以传值的方式捕获外部的所有变量
[&]:以传引用的方式捕获外部的所有变量
[this]:捕获外部的this指针
[=,&a]:以传值的方式捕获外部的所有变量，但是a变量以传引用的方式捕获
[a,b]:以值传递的方式捕获外部变量a和b
[a,&b]:a以值传递捕获，b以引用的方式捕获
*/



#if 0
//研究lambda表达式底层实现原理
template <typename T = void>
class TestLambda01
{
public:
	void operator()() const
	{
		cout << "hello world!" << endl;
	}
};

template <typename T = int>
class TestLambda02
{
public:
	TestLambda02() {}
	int operator()(int a, int b) const
	{
		return a + b;
	}
};

template <typename T = void>
class TestLambda03
{
public:
	//TestLambda03(int a, int b) :ma(a), mb(b) {}
	TestLambda03(int& a, int& b) :ma(a), mb(b) {}
	void operator()() const
	{
		//常方法里面不能修改成员变量的值
		int tmp = ma;
		ma = mb;
		mb = tmp;
	}
private:
	//const常方法里面可以修改mutable修饰的成员变量
	//mutable int ma;
	//mutable int mb;
	int& ma;  //给引用ma,mb赋值不是改变引用变量本身，所以const常方法可以更改引用变量所引用的内存的值
	int& mb;
};
int main()
{
	//auto func1 = []()->void {cout << "Hello World!" << endl; };
	auto func1 = []() {cout << "Hello World!" << endl; }; //返回值省略
	func1();

	auto func2 = [](int a, int b)->int {return a + b; };
	cout << func2(10, 20) << endl;

	TestLambda01<> t1;
	t1();
	TestLambda02<> t2;
	cout << t2(30, 40) << endl;

	////////////////////////////////////////
	int a = 10;
	int b = 20;
	//auto func3 = [=]() mutable
	//auto func3 = [a, b]()mutable  //值传递a，b不会交换
	//auto func3 = [&a, &b]()  //a，b交换
	auto func3 = [&]()  //a,b交换
	{
		int tmp = a;
		a = b;
		b = tmp;
	};  //不加mutable报错：“a”: 无法在非可变 lambda 中修改通过复制捕获
	func3();
	cout << "a:" << a << " b:" << b << endl;

	TestLambda03<> t3(a, b);
	t3();
	cout << "a:" << a << " b:" << b << endl;

	return 0;
}
/*
Hello World!
30
hello world!
70
a:20 b:10
a:10 b:20
*/
#endif




#if 1
//lambda表达式使用举例
int main()
{
	vector<int> vec;
	for (int i = 0; i < 20; i++)
	{
		vec.push_back(rand() % 100 + 1);
	}
	sort(vec.begin(), vec.end(),
		[](int a, int b)->bool
	{
		return a > b;
	});  //降序
	for (int val : vec)
	{
		cout << val << " ";
	}
	cout << endl;

	//当前是降序序列
	//将65按序插入序列,要找到第一个小于65的数字
	auto it = find_if(vec.begin(), vec.end(),
		[](int val)->bool
	{
		return 65 > val;
	});
	if (it != vec.end())
	{
		vec.insert(it, 65);
	}

	for (int val : vec)
	{
		cout << val << " ";
	}
	cout << endl;

	cout << "==========================" << endl;
	//for_each()是将传入的容器的区间内的每个元素，应用到第三个参数函数对象上面

	//容器元素输出
	//输出偶数
	//从底层实现可以看出，需要一个一元函数对象
	for_each(vec.begin(), vec.end(),
		[](int a)
	{
		if (a % 2 == 0)
			cout << a << " ";
	});
	cout << endl;

	return 0;
}
/*
96 92 82 79 70 68 65 63 62 59 46 43 42 37 35 28 28 25 6 1
96 92 82 79 70 68 65 65 63 62 59 46 43 42 37 35 28 28 25 6 1
==========================
96 92 82 70 68 62 46 42 28 28 6
*/

#endif