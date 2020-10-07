#include <iostream>
using namespace std;


/*
C++《语言级别》提供的四种类型转换方式
int a = (int)b;//C语言的强制类型转换

const_cast : 去掉（指针或者引用）常量属性的一个类型转换，const_cast<这里必须是指针或者引用,int*,int&>
static_cast使用最多 :  提供编译器认为安全的类型转换（没有任何联系的类型之间的转换就被否定了），基类和派生类之间的转换可以使用static_cast
reinterpret_cast : 类似于C风格的强制类型转换，可以做编译器认为不安全的类型转换
dynamic_cast动态类型转换 : 主要用在继承结构中，可以支持RTTI类型识别的上下转换
*/

/*
static_cast看作编译时期的类型转换
dynamic_cast运行时的类型转换，支持RTTI信息识别的
*/

class Base
{
public:
	virtual void func() = 0;
};
class Derive1 : public Base
{
public:
	void func() { cout << "call Derive1::func" << endl; }
};
class Derive2 : public Base
{
public:
	void func() { cout << "call Derive2::func" << endl; }
	// Derive2实现新功能的API接口函数
	void derive02func()
	{
		cout << "call Derive2::derive02func" << endl;
	}
};
/*
typeid(*p).name() == "Derive"
*/
void showFunc(Base* p)
{
	/*场景
	需求是这样的，如果p指向的是Derive2的对象，就调用p->derive02func()方法
	如果p指向的不是Derive2对象，是其他派生类对象，那就调用p->func()方法
	在这里需要识别一下*p的类型，RTTI
	*/


	// dynamic_cast会检查p指针是否指向的是一个Derive2类型的对象？
	// p->vfptr->vftable RTTI指针指向RTTI信息 如果是Derive2，dynamic_cast转换类型成功，
	// 返回Derive2对象的地址，给pd2；否则返回nullptr
	// static_cast编译时期的类型转换  dynamic_cast运行时期的类型转换 支持RTTI信息识别
	Derive2* pd2 = dynamic_cast<Derive2*>(p);
	if (pd2 != nullptr)
	{
		pd2->derive02func();
	}
	else
	{
		p->func(); // 动态绑定  *p的类型 Derive2  derive02func
	}
}
#if 1
//dynamic_cast<>
int main() {
	Derive1 d1;
	Derive2 d2;
	showFunc(&d1);
	showFunc(&d2);

	return 0;
}
#endif

#if 0
int main() {
	
	//95%的情况，都是使用static_cast<>进行类型转换

	//reinterpret_cast
	//int* p = nullptr;
	//double* q = reinterpret_cast<double*>(p);//类似于C语言得强制类型转换，不会报错

	//static_cast<>//////////////////////////////
	//int a = 10;
	//char b = static_cast<int>(a);  //完全ok

	//int* p = nullptr;
	//short* b = static_cast<short*>(p); //报错，“static_cast”: 无法从“int *”转换为“short *”
	//int *和short*之间没有任何联系
	//double* q = (double*)p;  //C语言，不安全，可以
	//基类类型 《==》派生类类型  当然可以使用static_cast进行类型转换啦！





	//const_cast<>//////////////////////////////
	//const int a = 10;
	//int* p = &a;  //不允许这样
	//int* p1 = (int *)&a;  //C风格,可以，这中强制类型转换，容易引起不可预计的错误
	//double* p1 = (double*)&a;  //C风格,可以,编译没问题
	//char* p3 = (char *)&a;  //C风格,可以，编译没问题

	//int* p2 = const_cast<int*>(&a);  //c++，可以,这样才正确
	//char* p2 = const_cast<char*>(&a);  //c++,编译出问题,“const_cast”: 无法从“const int *”转换为“char *”
	//double* p2 = const_cast<double*>(&a);  //c++,编译出问题,“const_cast”: 无法从“const int *”转换为“double *”
	/*
	int *p1 = (int *)&a;  //C风格
	int *p2 = const_cast<int*>(&a);
	这两种转换在汇编角度看没有任何差别，const_cast在语言级别提供安全转换
	*/
	//const_cast<这里面必须是指针或者引用类型>
	//int b = const_cast<int>(a); // “const_cast”: 无法从“const int”转换为“int”



	return 0;
}
#endif

#if 0
int main()
{
	Derive1 d1;
	Derive2 d2;
	showFunc(&d1);
	showFunc(&d2);

	//static_cast 基类类型 《=》 派生类类型  能不能用static_cast?当然可以！
	//int *p = nullptr;
	//double* b = reinterpret_cast<double*>(p);

	//const int a = 10;
	//int *p1 = (int*)&a;
	//int *p2 = const_cast<int*>(&a);
	// const_cast<这里面必须是指针或者引用类型 int* int&>
	//int b = const_cast<int>(a);

	return 0;
}
#endif