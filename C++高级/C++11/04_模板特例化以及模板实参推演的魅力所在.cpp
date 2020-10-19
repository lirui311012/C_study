#include <iostream>
#include <typeinfo>
using namespace std;

/*
模板的完全特例化(特例化又叫做特殊的实例化)和非完全(部分)特例化---为function底层实现原理做基础准备

模板的实参推演
*/

//特例化，又叫做特殊的实例化(模板类型的实例化不是由编译器做的，而是由开发者直接提供的,
//因为有的时候编译器的实例化不符合逻辑的)
//举例


#if 0
//函数模板
template <typename T>
bool compare(T a, T b)
{
	cout << "template compare" << endl;
	return a > b;
}

//完全特例化
template<>
bool compare<const char*>(const char* a, const char* b)
{
	cout << "compare<const char *>" << endl;
	return strcmp(a, b) > 0;
}

int main()
{
	compare(10, 20);
	/*
	1.模板实参推演，根据传参10,20推演出形参T的类型是int
	2.会从函数模板实例化一个专门处理int的模板函数，进行编译
	*/

	bool res = compare("bbb", "aaa");//模板类型推演，推演出形参T是const char *
						  //但是如果从原模板里面由编译器自己实例化模板函数，不符合我们实际的处理逻辑
						  //所以需要提供一个特例化
	cout << res << endl;
	return 0;
}
/*
template compare
compare<const char *>
1
*/
#endif
////////////////////////////////////////////////////////////
#if 0
template <typename T>
class Vector
{
public:
	Vector() { cout << "call Vector template init" << endl; }
};

//下面这个是对char *类型提供的完全特例化版本
//template<>不可以省略，这是特例化的语法
template<>
class Vector<char*>
{
public:
	Vector() { cout << "call Vector<char *> init" << endl; }
};

//下面这个是对指针类型提供的部分特例化版本
template <typename Ty>
class Vector<Ty *>
{
public:
	Vector() { cout << "call Vector<Ty *> init" << endl; }
};

int sum(int a, int b) { return a + b; }

//针对函数指针(有返回值，有两个形参变量)提供的部分特例化
template<typename R,typename A1,typename A2>
class Vector<R(*)(A1, A2)>
{
public:
	Vector() { cout << "call Vector<R(*)(A1, A2)> init" << endl; }
};

//针对函数类型(有一个返回值，有两个形参变量)提供的部分特例化
template<typename R,typename A1,typename A2>
class Vector<R(A1,A2)>
{
public:
	Vector() { cout << "call Vector<R(A1,A2)> init" << endl; }
};


//有完全特例化版本就使用完全特例化版本
//其次使用部分特例化版本
//如果都没有 ，那就从原模板自己实例化模板类
int main()
{
	Vector<int> v1;
	Vector<char*> v2; //由于提供了char*的完全特例化，所以优先选择使用完全特例化
	Vector<int *> v3;//虽然没有对int*提供完全特例化版本，但是有一个对指针提供的部分特例化版本
	Vector<int(*)(int, int)> v4; //函数指针
	Vector<int(int, int)> v5; //函数类型

	//注意区分一下函数指针类型和函数类型
	typedef int(*PFUN1)(int, int);
	PFUN1 pfunc1 = sum;
	cout << pfunc1(10, 20) << endl;

	typedef int (PFUNC2)(int, int);
	PFUNC2* pfunc2 = sum;
	cout << pfunc2(30, 50) << endl;
	
	return 0;
}
/*
call Vector template init
call Vector<char *> init
call Vector<Ty *> init
call Vector<R(*)(A1, A2)> init
call Vector<R(A1,A2)> init
30
80
*/

//上面全部都是模板完全特例化和部分特例化
#endif
////////////////////////////////////////////////////////////
//模板实参推演==>基本概念很简单
//模板实参推演的魅力所在

int sum(int a, int b) { return a + b; }


//T包含了所有的大的类型，我现在想把返回值，所有的形参类型全都取出来
template<typename T>
void func(T a)
{
	cout << typeid(T).name() << endl;
}

template<typename R,typename A1,typename A2>
void func2(R(*a)(A1,A2))
{
	cout << typeid(R).name() << endl;
	cout << typeid(A1).name() << endl;
	cout << typeid(A2).name() << endl;
}


class Test
{
public:
	int sum(int a, int b) { return a + b; }
};

template<typename R,typename T,typename A1,typename A2>
void func3(R(T::*a)(A1,A2))
{
	cout << typeid(R).name() << endl;
	cout << typeid(T).name() << endl;
	cout << typeid(A1).name() << endl;
	cout << typeid(A2).name() << endl;
}
int main()
{
	//func(10);
	//func("abc");
	func(sum);  //推演出int(*)(int,int)函数指针类型
	func2(sum);
	cout << "===================" << endl;
	func(&Test::sum);  //int (__thiscall Test::*)(int,int)

	func3(&Test::sum);
	return 0;
}