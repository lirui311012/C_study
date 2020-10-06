#include <iostream>
using namespace std;

/*
重载、隐藏、覆盖
1.重载关系
一组函数要重载，必须处在同一个作用域当中；而且函数名字相同，参数列表不同

2.隐藏(作用域的隐藏)的关系
在继承结构当中，派生类的同名成员，把基类的同名成员给隐藏调用了


============================================================
1.把继承结构，也说成从上（基类）到下（派生类）的结构
2.
基类对象 -> 派生类对象
派生类对象 -> 基类对象

基类指针（引用）-> 派生类对象
派生类指针（引用）-> 基类对象

总结：在继承结构中进行上下的类型转换，默认只支持从下到上的类型的转换 OK
*/
class Base
{

	/*
	#1和#2是函数重载该，#1,#2和#3不是重载，因为作用域不同
	#3将#1和#2隐藏，隐藏是指作用域的隐藏
	*/
public:
	Base(int data = 10) :ma(data) {}
	void show() { cout << "Base::show()" << endl; } //#1
	void show(int) { cout << "Base::show(int)" << endl; }//#2
	int getMa() {
		return ma;
	}
protected:
	int ma;
};
class Derive : public Base
{
public:
	Derive(int data = 20) :Base(data), mb(data) {}
	void show() { cout << "Derive::show()" << endl; } //#3
	int getMb() {
		return mb;
	}
private: 
	int mb;
};
int main()
{
	Base b(10);
	Derive d(20);

	//下面所谓的上就是基类，下就是派生类

	// 基类对象b <- 派生类对象d   类型从下到上的转换  Yes
	b = d;
	cout << b.getMa() << endl;  //20
	cout << d.getMb() << endl;  //20

	// 派生类对象d <- 基类对象b   类型从上到下的转换  No
	// d = b;

	// 基类指针（引用）<- 派生类对象  类型从下到上的转换 Yes
	Base* pb = &d;
	pb->show();
	//((Derive*)pb)->show();
	pb->show(10);

	// 派生类指针（引用）<- 基类对象  类型从上到下的转换  No
	Derive* pd = (Derive*)&b; // 不安全，涉及了内存的非法访问！
	pd->show();

#if 0
	Derive d;
	d.show();
	d.Base::show();
	//“Derive::show”: 函数不接受 1 个参数
	d.Base::show(10);
	d.show(20);//优先找的是派生类自己作用域的show名字成员；没有的话，才去基类里面找
#endif
	return 0;
}