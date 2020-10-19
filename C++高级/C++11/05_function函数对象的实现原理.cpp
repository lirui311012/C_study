#include <iostream>
#include <algorithm>
#include <functional>
#include <string>
#include <typeinfo>
using namespace std;
/*
function函数对象的实现原理
C++11提供的可变参的类型参数A...
*/

void hello(string str) { cout << str << endl; }
int sum(int a, int b) { return a + b; }

template <typename Fty>
class myfunction {};
///////////////////////////////////////////////////
/*
//部分特例化
template <typename R,typename A1>
class myfunction<R(A1)>
{
public:
	//typedef  R(*PFUNC)(A1);
	using PFUNC = R(*)(A1);  //和上面等价,函数指针类型
	myfunction(PFUNC pfunc)
		:_pfunc(pfunc) {}

	//函数对象就是这个operator()重载
	R operator()(A1 arg)
	{
		return _pfunc(arg);  //hello(arg)
	}
private:
	PFUNC _pfunc;
};

//部分特例化版本
template<typename R,typename A1,typename A2>
class myfunction<R(A1, A2)>
{
public:
	using PFUNC = R(*)(A1,A2);  
	myfunction(PFUNC pfunc)
		:_pfunc(pfunc) {}

	//函数对象就是这个operator()重载
	R operator()(A1 arg1,A2 arg2)
	{
		return _pfunc(arg1, arg2);  
	}
private:
	PFUNC _pfunc;
};
*/
/*
实现的还是不错的，那么问题来了，这个myfunction模板参数列表是函数类型
那么上面传了两个不同的参数，我们实现了两个不同myfuntion的部分特例化，那函数
类型好像是变化的，返回值还好说，那参数个数呢？3个？4个？5个？，难道提供n个特例化？
不现实，接下来解决这个问题，C++11提供的模板的语法非常强大
*/



///////////////////////////////////////////////////
//如何解决上面的问题？屏蔽掉上面的代码，重新来
//typename... A代表可变参，A表示的不是一个类型，而是一组类型，个数任意
template<typename R, typename... A>
class myfunction<R(A...)>  //可变参数个数的函数类型的部分特例化
{
public:
	using PFUNC = R(*)(A...);
	myfunction(PFUNC pfunc)
		:_pfunc(pfunc) {}

	R operator()(A... arg)  //A...可变参类型
	{
		return _pfunc(arg...);  //传入一组形参变量
	}
private:
	PFUNC _pfunc;
};
//这就是function底层实现原理

int main()
{
	myfunction<void(string)> func1(hello);
	func1("hello world!");//func1.operator()("hello world!");

	myfunction<int(int, int)> func2(sum);
	cout << func2(10, 20) << endl;

	return 0;
}
/*
hello world!
30
*/

