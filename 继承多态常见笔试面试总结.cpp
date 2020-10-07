#include <iostream>
using namespace std;

//文章从下往上看哦
/*
一定要搞清楚，C++在继承结构中，各个类型得虚函数列表的地址，是什么时候写入对象的虚函数指针里面的vfptr
答案：在构造函数开辟栈帧成功之后立刻给vfptr指针赋值，然后执行构造函数函数体指令
*/

/*
一定要搞清楚，什么事情在编译阶段做
什么事情推迟到运行阶段做


预编译，编译，汇编，链接 不要放在一起思考，是分步骤思考的
*/

#if 1
////////////////////////////////////////////////////////////////////
//题目：下面得代码得运行得情况？有问题？无问题？，若存在问题，说出原因
class Base
{
public:
	Base()
	{
		//注意vfptr指针赋值得时机
		//在当前函数栈帧开辟完成之后，立刻给vfptr指针赋值
		//然后再执行函数体代码，每个构造函数都是这样做的
		/*
		push ebp
		mov ebp, esp
		sub esp, 4Ch
		rep stos esp<->ebp 0xCCCCCCCC(windows VS GCC/G++)
		vfptr <- &Base::vftable
		*/
		cout << "call Base()" << endl;
		clear();
	}
	void clear() { memset(this, 0, sizeof(*this)); }
	virtual void show()
	{
		cout << "call Base::show()" << endl;
	}
};
class Derive : public Base
{
public:
	Derive()
	{
		//注意vfptr指针赋值得时机
		//在当前函数栈帧开辟完成之后，立刻给vfptr指针赋值
		//然后再执行函数体代码，每个构造函数都是这样做的
		/*
		push ebp
		mov ebp, esp
		sub esp, 4Ch
		rep stos esp<->ebp 0xCCCCCCCC(windows VS GCC/G++)
		vfptr <- &Derive::vftable
		*/
		cout << "call Derive()" << endl;
	}
	void show()
	{
		cout << "call Derive::show()" << endl;
	}
}; 
int main()
{
	//Base* pb1 = new Base();
	//pb1->show(); //动态绑定
	//delete pb1;

	Base* pb2 = new Derive();
	pb2->show(); //动态绑定   //打印call Derive::show()
	delete pb2;


	return 0;
}
//答案：
/*//Base* pb1 = new Base();
	//pb1->show(); //动态绑定
	//delete pb1;
	代码会引起程序崩溃。
	崩溃的原因是，再Base得构造对象的时候，构造函数开辟栈帧之后给vfptr指针赋值，随后将空间清空
	所以再pb1->show()得时候发生动态绑定，当问一个vfptr空指针，自然程序崩溃

	下面的代码正常运行，运行结果是call Derive::show()

*/
#endif



#if 0
////////////////////////////////////////////////////////////////////
class Base
{
private:
	virtual void show()
	{
		cout << "call Base::show" << endl;
	}
};
class Derive : public Base
{
public:
	void show()
	{
		cout << "call Derive::show" << endl;
	}
};
int main()
{
	Derive* p = new Derive();
	//编译阶段发现p是Derive*指针，查看Derive::show()访问权限是public，编译通过
	//动态绑定哦
	p->show();
	//打印call Derive::show
	delete p;
	return 0;
}
#endif



#if 0
////////////////////////////////////////////////////////////////////
class Base
{
private:
	virtual void show()
	{
		cout << "call Base::show" << endl;
	}
};
class Derive : public Base
{
public:
	void show()
	{
		cout << "call Derive::show" << endl;
	}
};
int main()
{
	Base* p = new Derive();
	/*
	一定要区分好哪些工作是在编译阶段完成的，哪些工作是在运行期间进行的
	*/
	//p指针是Base *
	//此处是在编译阶段确定，所以查看Base::show()发现是private，好的，编译不通过
	//“Base::show”: 无法访问 private 成员(在“Base”类中声明)
	p->show();
	delete p;
	return 0;
}
#endif


////////////////////////////////////////////////////////////////////
#if 0
class Base
{
public:
	virtual void show()
	{
		cout << "call Base::show" << endl;
	}
};
class Derive : public Base
{
private:
	void show()
	{
		cout << "call Derive::show" << endl;
	}
};
int main()
{
	Base* p = new Derive();
	/*
	成员方法能不能调用，就是说方法的访问权限是不是public的，是在编译阶段就需要确定好的
	这些访问权限public,protected,private只在编译阶段起作用
	*/

	//编译阶段只能看到Base::show(),而Base里面的show是public,所以编译器没有报错
	//最终到底是调用基类的方法还是派生类的方法，那要看生成的汇编指令是静态绑定还是动态绑定
	//编译阶段看到的是Base::show() --public
	//如果静态绑定call Base::show()
	//如果动态绑定 call ecx 寄存器
	p->show();  //最终能调到Dervie::show，是在运行期间才确定的，发生动态绑定
	//打印：call Derive::show
	delete p;
	return 0;
}
#endif

////////////////////////////////////////////////////////////////////
#if 0
class Base
{
public:
	virtual void show(int i = 10)
	{
		cout << "call Base::show i:" << i << endl;
	}
};
class Derive : public Base
{
public:
	void show(int i = 20)
	{
		cout << "call Derive::show i:" << i << endl;
	}
};
int main()
{
	Base* p = new Derive(); // 虚析构函数
	/*
	push 0Ah => 函数调用，参数压栈是在编译时期就确定好的
	mov eax, dword ptr[p]
	mov ecx, dword ptr[eax]
	call ecx
	*/
	p->show(); // 动态绑定 p-> Derive vfptr -> Derive vftable
	//打印结果：call Derive::show i:10
	delete p;

	return 0;
}
#endif

//笔试
#if  0
////////////////////////////////////////////////////////////////////
class Animal
{
public:
	Animal(string name) :_name(name) {}
	// 纯虚函数
	virtual void bark() = 0;
protected:
	string _name;
};
// 以下是动物实体类
class Cat : public Animal
{
public:
	Cat(string name) :Animal(name) {}
	void bark() { cout << _name << " bark: miao miao!" << endl; }
};
class Dog : public Animal
{
public:
	Dog(string name) :Animal(name) {}
	void bark() { cout << _name << " bark: wang wang!" << endl; }
};
int main()
{
	Animal* p1 = new Cat("加菲猫"); // vfptr -> Dog vftable
	Animal* p2 = new Dog("二哈"); // vfptr -> Cat vftable

	int* p11 = (int*)p1;
	int* p22 = (int*)p2;
	int tmp = p11[0]; // p11[0]访问的就是Cat的前4个字节
	p11[0] = p22[0]; // p22[0]访问的就是Dog的前4个字节
	p22[0] = tmp;

	p1->bark(); // p1 -> Cat vfptr -> Dog vftable bark
	p2->bark(); // p2 -> Dog vfptr -> Cat vftable bark
	/*
	打印结果：
	加菲猫 bark: wang wang!
	二哈 bark: miao miao!
	*/

	delete p1;
	delete p2;

	return 0;
}
#endif