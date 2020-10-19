#include <iostream>
using namespace std;
//使用vs提供的cmd可以查看源文件里面类的对象的空间布局情况
//cl 01_虚基类与虚继承.cpp /d1reportSingleClassLayoutB
//用大写来区分一下1和l
//CL 源文件名字.cpp /D1REPORTSINGLECLASSLAYOUT类名字


//必须记住这句话，一定要记住
//////*&*&*&*&*************&*&*&*&************&*&*&*&*&***&……&%#&￥%&*￥……&*￥……&*￥……&*
//////注意：基类指针指向派生类对象，该指针指向的位置永远是派生类中基类数据部分的起始地址，仔细理解，该指针指向的位置很重要！
//////*&*&*&*&*************&*&*&*&************&*&*&*&*&***&……&%#&￥%&*￥……&*￥……&*￥……&*


/*
多重继承：代码的复用   一个派生类有多个基类
class C : public A,public B
{
};

不要把抽象类和虚基类搞混

抽象类：有纯虚函数的类
虚基类定义在下面


virtual:
1.修饰成员方法，是虚函数
2.可以修饰继承方式，是虚继承。被虚继承的类就是虚基类（虚基类的定义）
*/
/*
class A {
public:
private:
	int ma;
};
class B :public A {
public:
private:
	int mb;
};

A a; ma 4字节
B b; ma,mb  8字节
*/


//类A是B的虚基类
/*
#if 1
class A {
public:
private:
	int ma;
};
class B :virtual public A {
public:
private:
	int mb;
};
#endif
//sizeof(A) == 4
//sizeof(B) == 12
*/



//重载new为了验证指针的指向，实际申请空间的地址和释放的时候的指针指向的地址
class A {
public:
	virtual void func() { cout << "A::func()" << endl; }
	void operator delete(void *ptr) {
		cout << "operator delete p:" << ptr << endl;
		free(ptr);
		ptr = nullptr;
	}
private:
	int ma;
};
class B :virtual public A {
public:
	virtual void func() { cout << "B::func()" << endl; }
	void* operator new(size_t size) {
		void* p = malloc(size);
		cout << "operator new p:" << p << endl;
		return p;
	}
private:
	int mb;
};
/*
对于上述代码-->
如果class B :public A
那么B类的对象的内存布局就是
A::ma
mb

如果class B :virtual public A
那么B类的对象的内存布局如下所示
class B size(16):
		+---
 0      | {vbptr}
 4      | mb
		+---
		+--- (virtual base A)
 8      | {vfptr}
12      | ma
		+---

B::$vbtable@:
 0      | 0
 1      | 8 (Bd(B+0)A)

B::$vftable@:
        | -8
 0      | &B::func

B::func this adjustor: 8
vbi:        class  offset o.vbptr  o.vbte fVtorDisp
			   A       8       0       4 0

虚基类的数据一定要搬到内存的最后，然后在前面添加一个vbptr指针
vbptr就是指向vbtable虚基类表，一个类对应一个虚基类表
虚基类表里面第一个数据0代表向上的偏移量，因为vbptr在对象的前4字节，所以向上偏移量为0
第二个数据记录着在派生类对象中vbptr离虚基类数据的偏移量
vbptr和vfptr一样，都是在编译时期生成，运行时期放到数据段
一个类型定义的多个对象都指向同一个vbptr

cl 01_虚基类与虚继承.cpp /d1reportSingleClassLayoutB
*/


int main() {
	//cout << sizeof(A) << endl;
	//cout << sizeof(B) << endl;

	////*&*&*&*&*************&*&*&*&************&*&*&*&*&***&……&%#&￥%&*￥……&*￥……&*￥……&*
	//注意：基类指针指向派生类对象，永远指向的是派生类基类部分数据的起始地址
	////*&*&*&*&*************&*&*&*&************&*&*&*&*&***&……&%#&￥%&*￥……&*￥……&*￥……&*

	//普通情况下，派生类内存布局肯定是:先是基类，后是派生类
	/*
		普通情况下派生类内存布局
		+------+     <----p= 00B4F340:父类指针指向子类对象，指向这里没有任何问题
		|  基类 |              开辟空间的首地址就是这里，而且父类指针p也指向这里
		|------|               内存回收delete p;p  = nullptr;成功~
		|  子类 |
		|	   |
		+------+
		这种情况下，基类指针指向派生类对象，指向的永远都是基类数据部分的其实地址，这样没有任何问题


		但是虚继承就不一样啦，子类虚继承基类后，对于派生类来说，基类是虚基类，前面有一个vbptr指针
		然后虚基类的数据部分移到派生类对象内存的后部分
		虚基类指针指向派生类对象，虚继承的派生类对象的内存分布如下
		就拿上面的例子举例，此时虚函数和虚继承撞在了一起
		+------------+   《-------q=00B4F340
		| vbptr      |
		| mb         |
		+------------+
		| A::        |   
		|	vfptr    |     <---p =00BF348  依然还是父类指针指向子类对象
		|	ma       |         此时是的基类对于派生类来说是虚基类
	    +------------+         根据上述描述可以知道
		                      父类指针一定指向这里吧，这里是基类的数据部分
							  但是实际空间申请是从指针q的位置开始申请的
							  所以在p->func()调用虚函数的时候可以找到虚指针指向的虚函数表
							  从而进行动态绑定，但是在delete的时候依然从p出释放就出现问题
							  因为空间申请是从q位置开始的
		
	*/
	A* p = new B();
	cout << "main p :" << p << endl;
	p->func();  //B::func() 依然可以多态哦
	//调用没有问你，是动态绑定，但是delete的时候出现了问题，内存回收出现问题，原因在上面
	delete p;
	p = nullptr;

	/*
	增加了operator new和operator delete的打印结果：
	operator new p:00B4F340
	main p :00B4F348
	B::func()
	operator delete p:00BF348
	程序崩溃

	对于windows vs的编译器就很呆头呆脑，他就知道基类指针指向派生类的基类数据部分的起始位置，所以必然出错
	对于linux的g++编译器，在delete的时候会自动偏移到new空间的其实位置进行正确的空间释放
	//开发服务器都是在linux下所以上述代码不是错误噢噢噢噢
	*/


	//如果不考虑堆空间
	//B b;
	//A *p = &b=b;
	//p->func();  //这样就没问题啦，因为不需要delete释放空间，b对象生命周期结束自动回收

	return 0;
}


/*
面试题
1.
class A{}  sizeof(A) = 1
class B :public A{}  sizeof(B) = 1

2.
class A
{
public:
	virtual void test(){}
}
class B:public A{}
sozepf(A) = 4
sizeof(B) = 4   //vfptr

3.
class A
{
public:
	virtual void test(){}
}
class B:virtual public A{}    //vfptr,vbptr
sizeof(A) = 4;
sizeof(B) = 8

vfptr指向vftable虚函数表
vbptr指向vbtable虚基类表
*/



//一定要搞清楚vfptr指针的存放位置

//对于vfptr属于基类数据还是派生类数据总结
//如果基类有虚函数，那么基类就有vfptr指针
//派生类继承后，派生类对象中的vfptr指针依然属于基类数据部分

//如果基类无虚函数，那么基类就没有虚指针
//而是在派生类才有的虚函数，那么虚指针就是属于派生类对象的派生类数据部分

//上述代码使用命令可以查看内存结构
// cl 01_虚基类与虚函数.cpp /d1reportSingleClassLayoutB
//结果如下
/*
01_虚基类与虚继承.cpp
class B size(16):
		+---
 0      | {vbptr}
 4      | mb
		+---
		+--- (virtual base A)
 8      | {vfptr}
12      | ma
		+---
B::$vbtable@:   //虚基类表
 0      | 0  //偏移量，由于vbptr在该对象的其实部分所以偏移量是0
 1      | 8 (Bd(B+0)A)   //8代表从vbptr偏移八字节就可以找到虚基类的数据

B::$vftable@:   //虚函数表
		| -8   //注意此时不是0了，而是-8，意思就是vfptr离其实空间的偏移量也就是vfptr所在地址-8可以找到其实空间位置
 0      | &B::func

B::func this adjustor: 8
vbi:       class  offset o.vbptr  o.vbte fVtorDisp
			   A       8       0       4 0
*/
