#include <iostream>
#include <time.h>
#include <vector>      
#include <algorithm>   
#include <functional>  
using namespace std;

//绑定器应用在，需要一个低元函数对象，但是你只有高元函数对象，这时使用绑定器来解决
//STL库里面提供的bind1st和bind2nd只能应用于二元函数对象，局限性出来了!!!!
//bind1st,bind2nd,  绑定器+二元函数对象 ==》 一元函数对象
//引出C++11从Boost库中引入了bind绑定器和function函数对象机制(更加强大的机制)


/*
结论：
绑定器是函数对象的一个应用
STL里面的bind1st,bind2nd,  绑定器+二元函数对象 ==》 一元函数对象
*/
/*
STL里面的bind1st就是以下原理
bind2nd也是如此
bind2nd只需要将一元函数对象_mybind1st::bool operator()(const T& second)的return返回值临时对象
的构造参数交换，调用的时候不再使用greater<>(a>b)，改用less(a <b)即可
*/



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

/*
实现一个泛型算法my_find_if
*/
template <typename Iterator,typename Compare> //迭代器类型和函数对象类型
Iterator my_find_if(Iterator first, Iterator last, Compare comp)
{
	//first到last之间的元素只要有满足comp这个函数对象的运算，那就么就返回当前元素的迭代器
	//否则返回last
	for (; first != last; ++first)
	{
		if (comp(*first))  //comp.operator()(*first)
		{
			return first;
		}
	}
	return last;
}


//实现一元函数对象
template <typename Compare, typename T>
class _mybind1st
{
public:
	_mybind1st(Compare comp, T val)
		:_comp(comp), _val(val)
	{}
	bool operator()(const T& second)
	{
		//底层还是二元函数对象，只是对二元函数对象进行了封装
		return _comp(_val,second);   //_val是绑定好的，second是一元函数对象从容器里面接收的值
	}//底层调用还依然还是greater二元函数对象
private:
	Compare _comp;
	T _val;
};


//第一个参数二元函数对象，第二个参数元素类型
//my_bind1st(greater<int>(),70)
//结论：绑定器是函数对象的一个应用
template <typename Compare,typename T>
_mybind1st<Compare, T> my_bind1st(Compare comp, const T& val)
{
	//直接使用函数模板，好处是，可以进行类型的推演，免得我们去写模板类型参数
	return _mybind1st<Compare, T>(comp, val);
}
int main()
{
	vector<int> vec;
	srand((unsigned int)time(nullptr));
	for (int i = 0;i < 20;i++)
	{
		vec.push_back(rand() % 100 + 1);
	}
	showContainer(vec);
	sort(vec.begin(), vec.end()); //默认升序
	showContainer(vec);

	sort(vec.begin(),vec.end(),greater<int>());  //降序
	showContainer(vec);

	//auto it = find_if(vec.begin(),vec.end(),bind1st(greater<int>(),70));

	//bind1st从调用方式上看，他是一个函数模板，封装了一元函数对象的产生
	auto it = my_find_if(vec.begin(),vec.end(),my_bind1st(greater<int>(),70));
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