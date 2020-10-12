#include <iostream>
using namespace std;
/*
对于对象优化来说特别重要，提高c++代码的运行效率
*/

#if 0
class Test
{
public:
	Test(int a = 10) :ma(a)
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
	Test& operator=(const Test& t)
	{
		cout << "operator=" << endl;
		ma = t.ma;
		return *this;
	}
private:
	int ma;
};

int main() {
	Test t1;
	Test t2(t1);
	Test t3 = t1;

	// Test(20) 显示生成临时对象  生存周期：所在的语句
	/*
	C++编译器对于对象构造的优化：用临时对象生成新对象的时候，临时对象
	就不产生了，直接构造新对象就可以了
	*/
	Test t4 = Test(20);   //和Test t4(20);没有区别
	cout << "=====================" << endl;

	t4 = t2;  //t4.operator=(t2);
	
	//t4.operator=(const Test&)
	t4 = Test(30); //这个临时对象必须生成 ，出了这条语句临时对象立刻析构


	//int-->强转-->类类型
	//在把其他类型转换成类类型的时候，编译器要看有没有合适的构造函数
	//现在要把int转换成Test类型，所以编译器会先看看Test有没有Test(int)的构造函数
	//如果有就会隐式生成一个临时对象
	t4 = (Test)30;  //当然前面写了(Test)这叫显示
	/*
		t4 = Test(30);
		t4 = (Test)30;
		这俩没有任何区别，完全一样
		这俩都是显示生成临时对象
	*/
	t4 = 30;//这叫隐式生成临时对象  int-->Test(int),  char *-->Test(char *)
	cout << "=====================" << endl;

	//指针p指向临时对象，引用ref引用一个临时对象
	Test* p = &Test(40); //这个临时对象生成后除了语句直接析构，因为它没有名字
	const Test& ref = Test(50); //这个临时对象不会被立刻析构，因为引用就是起别名
	                            //这个临时对象的生命周期就变成了引用变量的生命周期
	/*
	结论就是，用指针指向临时对象不安全，但是用引用去引用临时对象是ok的
	*/


	cout << "=====================" << endl;

	/*
	运行结果：
		Test(int)
		Test(const Test&)
		Test(const Test&)
		Test(int)
		=====================
		operator=
		Test(int)
		operator=
		~Test()
		Test(int)
		operator=
		~Test()
		Test(int)
		operator=
		~Test()
		=====================
		Test(int)
		~Test()
		Test(int)
		=====================
		~Test()
		~Test()
		~Test()
		~Test()
		~Test()
	*/

	return 0;
}
#endif

#if 1

class Test
{
public:
	// Test() Test(10) Test(10, 10)
	Test(int a = 5, int b = 5)
		:ma(a), mb(b)
	{
		cout << "Test(int, int)" << endl;
	}
	~Test()
	{
		cout << "~Test()" << endl;
	}
	Test(const Test& src)
		:ma(src.ma), mb(src.mb)
	{
		cout << "Test(const Test&)" << endl;
	}
	void operator=(const Test& src)
	{
		ma = src.ma;
		mb = src.mb;
		cout << "operator=" << endl;
	}
private:
	int ma;
	int mb;
};

Test t1(10, 10);					// 1.Test(int, int)
int main()
{
	Test t2(20, 20);				// 3.Test(int, int)
	Test t3 = t2;					// 4.Test(const Test&)
	// static Test t4(30, 30);

	//静态局部变量是程序第一次运行到它的时候，才会进行初始化
	static Test t4 = Test(30, 30);  // 5.Test(int, int)
	t2 = Test(40, 40);				// 6.Test(int, int) operator= ~Test()
	// (50, 50) = (Test)50; Test(int)(逗号表达式的最终结果就是最后一个)
	t2 = (Test)(50, 50);			// 7.Test(int,int) operator=  ~Test()
	t2 = 60; //Test(int)	        // 8.Test(int,int) operator= ~Test()
	Test* p1 = new Test(70, 70);    // 9. Test(int,int) 
	Test* p2 = new Test[2];         // 10. Test(int,int) Test(int,int)
	Test* p3 = &Test(80, 80);       // 11. Test(int,int)  ~Test()
	const Test& p4 = Test(90, 90);  // 12. Test(int,int)
	delete p1;                      // 13.~Test()
	delete[]p2;                     // 14. ~Test() ~Test()
}
Test t5(100, 100);                  // 2.Test(int, int)

/*
注意t1,t4,t5的析构顺序，程序结束再析构，由于构造顺序是t1,t5,t4所以析构顺序t4,t5,t1
*/
/*
打印结果
Test(int, int)
Test(int, int)
Test(int, int)
Test(const Test&)
Test(int, int)
Test(int, int)
operator=
~Test()
Test(int, int)
operator=
~Test()
Test(int, int)
operator=
~Test()
Test(int, int)
Test(int, int)
Test(int, int)
Test(int, int)
~Test()
Test(int, int)
~Test()
~Test()
~Test()
~Test()
~Test()
~Test()
~Test()
~Test()
~Test()
*/

#endif
