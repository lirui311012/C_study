#include <iostream>
using namespace std;
//提高OOP代码运行效率

#if 1
//用临时对象拷贝构造一个同类型的新对象的时候，临时对象就会被优化掉，直接构造新对象
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
/*
C++ OOP编程对象优化，效率和C语言可以一样
1. 函数参数传递过程中，对象优先按引用传递，不要按值传递  //43行
2. 函数返回对象的时候，应该优先返回一个临时对象，而不要返回一个定义过的对象  //49行
	返回一个对象的时候，先把构造该对象的参数全部计算出来，然后返回一个临时对象
3. 接收返回值是对象的函数调用的时候，优先按初始化的方式接收，不要按赋值的方式接收  //58行
*/


Test GetObject(Test& t)  //#1
{
	int val = t.getData();
	/*Test tmp(val);
	return tmp;*/
	//将上面代码改为
	return Test(val);  //#2  返回临时对象
				//这种写法就不构造该函数栈帧的临时对象，而是直接构造main函数栈帧的临时对象
}
int main()
{
	Test t1;
	/*Test t2;
	t2 = GetObject(t1); */
	//#3 上面代码改成这样
	Test t2 = GetObject(t1);
	

	return 0;
}
/*
打印结果
Test(int)
Test(int)  //直接构造t2
~Test()
~Test()

//完成对象优化后，从一开始的11个函数调用，优化到了4个函数调用
*/
#endif
