#include <iostream>
using namespace std;

#if 1
class Test
{
public:
	//注意带默认值参数的构造函数，非常强大
	// Test()  Test(20)
	Test(int data = 10) :ma(data)
	{
		cout << "Test(int)" << endl;
	}
	~Test()
	{
		cout << "~Test()" << endl;
	}
	Test(const Test& t) :ma(t.ma)
	{
		cout << "Test(const Test&)" << endl;
	}
	void operator=(const Test& t)
	{
		cout << "operator=" << endl;
		//浅拷贝
		ma = t.ma;
	}
	int getData()const { return ma; }
private:
	int ma;
};
// 不能返回局部的或者临时对象的指针或引用
//注意不可以返回局部的或者临时对象的指针或者引用，函数栈帧被回收，对象也被回收
//静态局部变量可以返回引用，因为存储在数据段，程序结束才被回收
Test GetObject(Test t)
{
	int val = t.getData();
	Test tmp(val);
	return tmp;
}
int main()
{
	Test t1;
	Test t2;
	t2 = GetObject(t1);  //函数调用，实参-->形参  是形参初始化

	return 0;
}
/*
打印结果
Test(int)
Test(int)
Test(const Test&)
Test(int)
Test(const Test&)//这个拷贝构造函数的调用是在getObject函数的return处发生，在main函数的栈帧上面使用tmp对象构造一个临时对象
~Test()  //tmp析构
~Test()  //getObject的形参析构
operator=  //main函数栈帧的临时对象给已经存在的t2对象赋值调用operator=
~Test()    //main函数栈帧的临时对象析构
~Test()
~Test()
*/

//看着简简单单的10行代码，背后实际调用了11个函数
#endif
