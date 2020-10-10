#include <iostream>
#include <vector>
#include <algorithm> // 包含了C++ STL里面的泛型算法
#include <functional> // 包含了函数对象和绑定器
using namespace std;


#if 1
/*
五、泛型算法 = template + 迭代器 + 函数对象
泛型算法的特点：
特点一：泛型算法的参数接收的都是迭代器
特点二：泛型算法的参数还可以接收函数对象（和C函数指针相似），比函数指针更优秀
sort,find,find_if,binary_search,for_each

绑定器 + 二元函数对象 =》 一元函数对象
bind1st：把二元函数对象的operator()(a, b)的第一个形参绑定起来
bind2nd：把二元函数对象的operator()(a, b)的第二个形参绑定起来
*/
int main()
{
	
}
#endif