#include <iostream>
#include <array>
#include <forward_list>
using namespace std;

/*
常用的C++11标准总结：
一：关键字和语法
auto：可以根据右值，推导出右值的类型，然后左边变量的类型也就已知了
nullptr：给指针专用（能够和整数进行区别） #define NULL 0
foreach：可以遍历数组，容器等
for(Type val : container) => 底层就是通过指针(数组)或者迭代器(容器)来实现的
{
	cout<<val<<" ";
}
右值引用：move移动语义函数 和 forward类型完美转发函数
模板的一个新特性：typename... A  表示可变参（类型参数）


二：绑定器和函数对象
function：函数对象
bind：绑定器  STL里面bind1st和bind2nd+二元函数对象 =》 一元函数对象
lambda表达式


三：智能指针
shared_ptr强智能指针 和 weak_ptr弱智能指针


四：容器
set和map：红黑树 O(lgn)
unordered_set和unordered_map：哈希表 O(1)  
array：数组#include <array> array不能扩容   vector   
forward_list：前向链表#include <forward_list>   list 


五：C++语言级别支持的多线程编程
跨平台


下面的系统api不跨平台
在windows下createThread
在linux下pthread_create
clone
*/



int main()
{

	return 0;
}