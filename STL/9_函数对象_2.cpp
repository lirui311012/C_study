#include <iostream>
using namespace std;
/*

//可以使用c语言的函数指针来解决这个问题，为什么还要使用函数对象？
使用函数对象的好处：
1.通过函数对象调用operator()，可以省略函数的调用开销，比通过函数指针
调用函数（不能够inline内联调用）效率高

2.因为函数对象是用类生成的，所以可以添加相关的成员变量，用来记录函数对象使用
时更多的信息
*/


//方案2：c++函数对象

//C++函数对象的版本实现
template <typename T>
class mygreater
{
public:
	//两个参数，就叫做二元函数对象，如果operator()只有一个参数就叫做一元函数对象
	bool operator()(T a, T b) {
		return a > b;
	}
};
template <typename T>
class myless
{
public:
	bool operator()(T a, T b) {
		return a < b;
	}
};
//class mygreater和 class myless实例化对象后，对象拥有()小括号重载函数
//这样的对象就叫做函数对象


//假设这是c++标准库函数模板
template<typename T, typename Compare>
bool compare(T a, T b, Compare comp) {
	return comp(a, b); //调用comp对象的小括号()重载函数operator()
	//在编译阶段，cpm(a,b)调用的是那个对象的operator()是非常明确的
	//所以采用函数对象可以实现内联，省去函数调用开销，调用效率非常高
}

int main() {
	//此处传参就不再是传入函数指针啦，而是函数对象
	cout << compare(10, 20, mygreater<int>()) << endl;  //比较大于
	cout << compare('b', 'y', myless<int>()) << endl;  //比较小于
	return 0;
}