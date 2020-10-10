#include <iostream>
using namespace std;




//虚函数和动态绑定 问题：是不是虚函数的调用一定就是动态绑定？ 肯定不是的！
//1.在类的构造函数当中，调用虚函数，也是静态绑定（构造函数中调用其它函数（虚），不会发生动态绑定的）
//构造函数在构造对象的时候，前四个字节的vfptr还没有形成呢
//2.如果不是通过指针或者引用变量来调用虚函数，那就是静态绑定

class Base
{
public:
	Base(int data = 0) :ma(data) {}
	virtual void show() { cout << "Base::show()" << endl; }
protected:
	int ma;
};
class Derive : public Base
{
public:
	Derive(int data = 0) :Base(data), mb(data) {}
	void show() { cout << "Derive::show()" << endl; }
private:
	int mb;
};
int main()
{
	Base b;
	Derive d;

	//用对象本身调用虚函数，是静态绑定，不会发生动态绑定
	//静态绑定
	b.show(); // 虚函数 
	
	//0069285C  lea         ecx,[b]
	//0069285F  call        Base::show (06913DEh)
	
	d.show(); // 虚函数 
	
	//00692864  lea         ecx,[d]
	//00692867  call        Derive::show (069119Ah)
	

	// 动态绑定（必须由指针调用虚函数）
	//不管你是基类指针指向基类对象还是基类指针指向派生类对象，全部都是动态绑定
	Base* pb1 = &b;   //父类指针指向父类对象
	pb1->show();
	  //00231D72  mov         eax,dword ptr [pb1]  
   //   00231D75  mov         edx,dword ptr [eax]  
   //   00231D77  mov         esi,esp  
   //   00231D79  mov         ecx,dword ptr [pb1]  
   //   00231D7C  mov         eax,dword ptr [edx]  
   //   00231D7E  call        eax  
   //   00231D80  cmp         esi,esp  
   //   00231D82  call        __RTC_CheckEsp (02312A3h) 


	Base* pb2 = &d;   //父类指针指向子类对象
	//  00231D8D  mov         eax,dword ptr [pb2]  
	//  00231D90  mov         edx,dword ptr [eax]  
	//  00231D92  mov         esi,esp  
	//  00231D94  mov         ecx,dword ptr [pb2]  
	//  00231D97  mov         eax,dword ptr [edx]  
	//  00231D99  call        eax  
	//  00231D9B  cmp         esi,esp  
	//  00231D9D  call        __RTC_CheckEsp (02312A3h)
	pb2->show();


	// 动态绑定（必须由引用变量调用虚函数）
	//引用在底层和指针一样
	Base& rb1 = b;
	rb1.show();
	Base& rb2 = d;
	rb2.show();



	// 动态绑定（虚函数通过指针或者引用变量调用，才发生动态绑定）
	Derive* pd1 = &d;
	pd1->show();
	Derive& rd1 = d;
	rd1.show();

	//不可以这样哦
	//Derive* pd2 = &b;

	//强制类型转换，不安全,内存越界，流氓的行为
	Derive* pd2 = (Derive*)&b;
	//打印出的是Base::show，访问的是基类的虚函数表
	pd2->show(); // 动态绑定 pd2 -> b vfptr -> Base vftable Base::show
	
	//00DD6154  mov         eax,dword ptr [pd2]
	//00DD6157  mov         edx,dword ptr [eax]
	//00DD6159  mov         esi,esp
	//00DD615B  mov         ecx,dword ptr [pd2]
	//00DD615E  mov         eax,dword ptr [edx]
	//00DD6160  call        eax
	//00DD6162  cmp         esi,esp
	//00DD6164  call        __RTC_CheckEsp (0DD12A3h)
	

	return 0;
}