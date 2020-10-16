#include <iostream>
#include <vector>
using namespace std;

/*
目录：
左值引用和右值引用的作用
面向对象的效率问题
右值引用的拷贝构造和operator=赋值函数
一道笔试题
函数接口返回容器
引用折叠
std::move移动语义
std::forward类型完美转发
std::move和std::forward总结：
【总结】：std::move是获取实参的右值引用类型；std::forward是在代码实现过程中，保持实参的原有的引用类型（左引用或者右引用类型）。
*/

/////////////////////////////////////////////////////////////////////////////


//C++左值引用和C++11右值引用，参考资料CSDN
/*
结论：有地址的用左值引用，没有地址的用右值引用；有变量名字的用左值引用，没有变量名字
的（比如临时量没有名字）用右值引用。
有地址，有名字-->左值引用
没有地址，没有名字(临时变量无名字)-->右值引用
*/


//C++左值引用
int main01()
{
	int a = 10;
	int& b = a; // 定义一个左值引用变量
	b = 20; // 通过左值引用修改引用内存的值
	
	cout << a << endl;  //20
	cout << b << endl;  //20
	cout << &a << endl; //007AF8FC
	cout << &b << endl; //007AF8FC
	/*
	20
	20
	007AF8FC
	007AF8FC
	*/

	return 0;
}
/*
反汇编
	int a = 10;
008718C8  mov         dword ptr [a],0Ah   // 这条mov指令把10放到a的内存中

	int& b = a;
//下面的lea指令把a的地址放入eax寄存器
//mov指令把eax的内容放入b内存里面
008718CF  lea         eax,[a]
008718D2  mov         dword ptr [b],eax

	b = 20;
//下面的mov指令把b内存的值放入eax寄存器（就是a的地址）
//mov指令再把20放入eax记录的地址的内存里面（就是把20赋值给a）
008718D5  mov         eax,dword ptr [b]
008718D8  mov         dword ptr [eax],14h
*/


/*
从上面的指令可以看出，定义一个左值引用在汇编指令上和定义一个指针是没有任何区别的，定
义一个引用变量int &b=a，是必须初始化的，因为指令上需要把右边a的地址放入一个b的内存
里面（相当于定义了一个指针的内存），当给引用变量b赋值时，指令从b里面取出a的地址，并
把20写入该地址，也就是a的内存中（相当于给指针解引用赋值），所以也说，使用引用变量时，
汇编指令会做一个指针自动解引用的操作。所以在汇编指令层面，引用和指针的操作没有任何区别！
*/
////////////////////////////////////////////////////////////////////

int main02() {

	//int& b = 20;  //编译报错“初始化”: 无法从“int”转换为“int &”
	/*
	因为定义引用变量，需要取右边20的地址进行存储，但是20是立即数字，没有在内存上存储，
	因此是无法取地址的。
	*/
	///////////////////////////////////////////////////////////////
	//解决方法:可以使用常引用
	const int& b = 20;   //编译成功
	/*
		这里temp是在内存上产生的临时量
		const int temp = 20;
		const int &b = temp;
	*/
	/*
	反汇编
	004B1A68  mov  dword ptr [ebp-14h],14h 《= ebp-14h就是内存栈上产生的临时量的内存地址
	004B1A6F  lea  eax,[ebp-14h]  《= 取临时量的内存地址放入寄存器eax
	004B1A72  mov  dword ptr [b],eax  《= 再把eax寄存器的值（放的是临时量地址）存入b中
	*/
	///////////////////////////////////////////////////////////////

	return 0;
}
/*
左值引用要求右边的值必须能够取地址，如果无法取地址，可以用常引用，如const int& b = 20; 但是这样一来，
我们只能通过b来读取数据，无法修改数据，因为b被const修饰成常量引用了。
但是其底层实现依然是
const int temp = 20;
const int &b = temp;//引用了一个临时变量
*/

/*
const int &b = 20;
不能够修改，如果想要修改那么就要用到C++11的右值引用
*/

///////////////////////////////////////////////////////////////
/*
右值引用
*/

int main03() {
	// 通过指令可以看到，原来const int &b=20和int &&b=20一模一样！！！
	int&& b = 20;
	/*
	005B1B18  mov   dword ptr [ebp-14h],14h  //这里mov指令相当于是产生了临时量，起始地址ebp-14h
	005B1B1F  lea   eax,[ebp-14h]  //把临时量的地址放入eax寄存器当中
	005B1B22  mov   dword ptr [b],eax  //再把eax的值（临时量的地址）放入b内存中（一个指针大小的内存）
	*/


	b = 40;
	/*
	005B1B25  mov         eax,dword ptr [b]
	005B1B28  mov         dword ptr [eax],28h
	*/

	return 0;
}
/*
定义一个右值引用变量是这样的int &&b=20，从汇编指令来看，依然要产生临时量，然后保存临时量的地址，也就是说
const int &b=20和int &&b=20在底层指令上是一模一样的，没有任何区别，不同的是，通过右值引用变量，可以进
行读操作，也可以进行写操作。

结论：
所以，可以给一个这样的结论，有地址的用左值引用，没有地址的用右值引用；有变量名字的用左值引用，没有变量名字
的（比如临时量没有名字）用右值引用。
*/

///////////////////////////////////////////////////////////////
/*
右值引用还能解决以下问题
*/

//1.面向对象的效率问题
//下面代码
class Stack
{
public:
	// size表示栈初始的内存大小
	Stack(int size = 1000)
		:msize(size), mtop(0)
	{
		cout << "Stack(int)" << endl;
		mpstack = new int[size];
	}
	// 栈的析构函数
	~Stack()
	{
		cout << "~Stack()" << endl;
		delete[]mpstack;
		mpstack = nullptr;
	}
	// 栈的拷贝构造函数
	Stack(const Stack& src)
		:msize(src.msize), mtop(src.mtop)
	{
		cout << "Stack(const Stack&)" << endl;
		mpstack = new int[src.msize];
		memcpy(mpstack, src.mpstack, sizeof(int) * mtop);
	}
	// 栈的赋值重载函数
	Stack& operator=(const Stack& src)
	{
		cout << "operator=" << endl;
		if (this == &src)
			return *this;

		delete[]mpstack;

		msize = src.msize;
		mtop = src.mtop;
		mpstack = new int[src.msize];
		memcpy(mpstack, src.mpstack, sizeof(int) * mtop);
		return *this;
	}

	//在后面使用带右值引用参数的拷贝构造函数和带右值引用参数的赋值重载函数时将条件编译打开#if 1
#if 1
	// 带右值引用参数的拷贝构造函数
	Stack(Stack&& src)
		:msize(src.msize), mtop(src.mtop)
	{
		cout << "Stack(Stack&&)" << endl;
		/*此处没有重新开辟内存拷贝数据，把src的资源直接给当前对象，再把src置空*/
		mpstack = src.mpstack;
		src.mpstack = nullptr;
	}

	// 带右值引用参数的赋值重载函数
	Stack& operator=(Stack&& src)
	{
		cout << "operator=(Stack&&)" << endl;
		if (this == &src)
			return *this;

		delete[]mpstack;

		msize = src.msize;
		mtop = src.mtop;
		/*此处没有重新开辟内存拷贝数据，把src的资源直接给当前对象，再把src置空*/
		mpstack = src.mpstack;
		src.mpstack = nullptr;
		return *this;
	}
#endif

	// 返回栈的长度
	int getSize()const { return msize; }
private:
	int* mpstack;
	int mtop;
	int msize;
};
Stack GetStack(Stack& stack)
{
	// 这里构造新的局部对象tmp
	Stack tmp(stack.getSize());
	/*
	因为tmp是函数的局部对象，不能出函数作用域，
	所以这里tmp需要拷贝构造生成在main函数栈帧上
	的临时对象，因此这里会调用拷贝构造函数，完成
	后进行tmp局部对象的析构操作
	*/
	return tmp;
}
int main04()
{
	Stack s;
	/*
	GetStack返回的临时对象给s赋值，该语句结束，临时对象
	析构，所以此处调用operator=赋值重载函数，然后调用
	析构函数
	*/
	s = GetStack(s);
	return 0;
}
/*
Stack(int) =》 对应Stack s;
Stack(int) =》 对应 Stack tmp(stack.getSize());
Stack(const Stack&) =》 对应return tmp;使用tmp对象再main函数栈帧拷贝构造临时对象
~Stack() =》 对应tmp的析构
operator= =》 s = GetStack(s);
~Stack() =》 对应s = GetStack(s);语句完成，临时对象的析构
~Stack() =》 对应main函数中s局部对象的析构
*/
/*
Stack对象由于成员变量是一个指针int *mpstack，构造时指向了堆内存，因此这样的对象做默认的浅拷贝和赋值
操作是有问题的，导致两个对象的成员指针指向同一个资源，析构时同一个资源被delete两次，代码运行崩溃，因此
我们需要给Stack提供自定义的拷贝构造函数和operator=赋值重载函数,上面的代码已经提供了拷贝构造函数重载和
operator=()的重载函数。
*/

/*
上面的代码虽然解决了对象的浅拷贝问题，但是效率却非常的低下，主要在这两句代码上：
1.return tmp;
这句代码中，tmp是函数的局部对象，因此不能出函数作用域，所以这里由tmp拷贝构造生成main函数栈帧上的临时对象。
然后拷贝构造的深拷贝实现如下：
// 栈的拷贝构造函数
Stack(const Stack &src)
	:msize(src.msize), mtop(src.mtop)
{
	cout << "Stack(const Stack&)" << endl;
	mpstack = new int[src.msize];
	memcpy(mpstack, src.mpstack, sizeof(int)*mtop);
}
src引用的是tmp对象，this指针指向的是main函数栈帧上的临时对象，它的实现是根据tmp临时对象的内存大小给临时
对象底层开辟内存，然后把tmp的数据再通过memcpy拷贝过来，关键是tmp马上就析构了！！！
如果能够做到你这个tmp对象马上析构，那你把你的mpstack指针，指向的堆区空间送给main函数栈帧的临时对象那该多好？
为什么不能把tmp持有的内存资源直接给临时对象呢？非得给临时对象重新开辟内存拷贝一份数据，然后tmp的资源又没有什
么用处，而且马上就要析构，这样只能造成代码运行效率低下。在tmp的析构函数里面将空间给释放掉啦
2.s = GetStack(s);
这里先通过临时量对象给s赋值，然后再析构临时对象，看看上面的operator=赋值函数的代码实现，先释放s占用的内存，
又根据临时量的大小给s重新分配内存，拷贝数据.
Stack& operator=(const Stack& src)
{
	cout << "operator=" << endl;
	if (this == &src)
		return *this;

	delete[]mpstack;

	msize = src.msize;
	mtop = src.mtop;
	mpstack = new int[src.msize];
	memcpy(mpstack, src.mpstack, sizeof(int) * mtop);
	return *this;
}
同样的问题，临时量对象给s赋值完成后，马上就析构了，为什么不能把临时对象的资源直接给s呢？如果这样做的话，效率就
很高了，省了内存的开辟和大量数据的拷贝时间了。
*/
/*
上面提到的两个问题，在C++11中的解决方式是提供《带右值引用参数的拷贝构造函数和operator=赋值重载函数》。
增加了：// 带右值引用参数的拷贝构造函数Stack(Stack &&src)
	  //带右值引用参数的赋值重载函数Stack& operator=(Stack&& src)
*/
int main05() {
	Stack s;
	s = GetStack(s);
	return 0;
}
/*
Stack(int)
Stack(int)
Stack(Stack&&) =》对应return tmp; 自动调用带右值引用参数版本的拷贝构造
~Stack()
operator=(Stack&&)=》 s = GetStack(s); 自动调用带右值引用参数的赋值重载函数
~Stack()
~Stack()
上面两处的拷贝构造函数和赋值重载函数的调用，自动使用了带右值引用参数的版本，效率大大提升，因为没有涉及任何的内存
开辟和数据拷贝。因为临时对象马上就要析构了，直接把临时对象持有的资源拿过来就行了。
以，临时量都会自动匹配右值引用版本的成员方法，旨在提高内存资源使用效率。带右值引用参数的拷贝构造和赋值重载函数，又
叫移动构造函数和移动赋值函数，这里的移动指的是把临时量的资源移动给了当前对象，临时对象就不持有资源，为nullptr了，
实际上没有进行任何的数据移动，没发生任何的内存开辟和数据拷贝。
*/

///////////////////////////////////////////////////////////////

/*
笔试题
*/

class A
{
public:
	A(int data = 10) :ptr(new int(data)) { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; delete ptr; ptr = nullptr; }
	
	//带有左值引用参数的拷贝构造函数
	A(const A& src)
	{
		cout << "A(const A&)" << endl;
		ptr = new int(*src.ptr);
	}
	//带右值引用参数的拷贝构造函数
	A(A&& src)
	{
		cout << "A(A&&)" << endl;
		ptr = src.ptr;
		src.ptr = nullptr;
	}
private:
	int* ptr;
};
int main06()
{
	vector<A> vec;
	vec.reserve(2);  //预留空间，就不会因为扩容而造成运行效果的影响
	A a;
	vec.push_back(a);  // 调用哪个构造函数？
	vec.push_back(A(20)); // 调用哪个构造函数？
	return 0;
}
/*
A()
A(const A&)  //vec.push_back(a);
A()
A(A&&)  //vec.push_back(A(20));
~A()
~A()
~A()
~A()
很明显，vec.push_back(a)调用的是左值引用参数的拷贝构造函数。vec.push_back(A(20))实参传入的是临时量对象
，调用的是右值引用参数的拷贝构造函数，效率较高。
*/
///////////////////////////////////////////////////////////////


//函数接口返回容器
/*
以上面class A为例
*/
vector<A> getVector()
{
	vector<A> vec;
	vec.reserve(3);//预留空间
	vec.push_back(A(20));
	vec.push_back(A(30));
	vec.push_back(A(40));
	cout << "————————" << endl;
	/*
		这里返回vec时，会调用vector容器的带右值引用参数的拷贝构造函数，
		类似vector(vector &&src)，直接把这里vec的资源移动给main函数
		中的v，效率很高，也就是说函数在返回容器的过程中，没有做任何的内存和
		数据开销
	*/
	return vec;
}
int main07()
{
	vector<A> v = getVector();
	return 0;
}
/*
A()
A(A&&)
~A()
A()
A(A&&)
~A()
A()
A(A&&)
~A()
————————
~A()
~A()
~A()
可以看出，其实你使用一个函数，然后返回一个容器，效率没有你想象的那么低，效率很高的
可以看到，vector< A > v = getVector()没有做任何的容器数据拷贝，调用带右值引用参数的成员方法，大大
提高了对象的使用效率。
*/

///////////////////////////////////////////////////////////////

//引用折叠
int main08()
{
	int a = 10;
	int& b = a;
	//int &&c = a; // 错误，无法将左值a绑定到右值引用c
	//int &&d = b; // 错误，无法将左值b绑定到右值引用d
	int&& e = 20;  // 正确，20是一个右值(没地址没名字)，可以绑定到右值引用e上
	//int &&f = e; // 错误，无法将左值e绑定到右值引用f，因为e有名字，有地址，本身也是左值，右值引用变量本身是左值
	int& g = e;    // 正确，e本身有名字，有地址，是一个左值，可以被g引用
	return 0;
}
//右值引用变量e本身是一个左值，这个需要做正确的理解。
//引用折叠的概念主要用在函数模板类型参数的推导中，如下面的函数模板：
template<typename T>
void func(T&& val)
{
	cout << "01 val:" << val << endl;
	T tmp = val;
	tmp++;
	cout << "02 val:" << val << " tmp:" << tmp << endl;
}
int main09()
{
	int a = 10;
	int& b = a;
	int&& c = 10;

	cout << "func(10):" << endl;
	func(10);// 10是右值，引用类型是int&&，T&&推导过程是int&&+&&折叠成int&&，所以T是int，下同
	cout << "func(a):" << endl;
	func(a);// a是左值，不可能用右值引用来引用，所以func推导T为int&，那么T&&->int&+&&折叠成int&
	cout << "func(std::move(a)):" << endl;
	func(std::move(a)); // std::move(a)是把a转成右值类型，右值引用类型是int&&，所以func推导T为int
	cout << "func(b):" << endl;
	func(b);// b是左值，不可能用右值引用来引用，所以func推导T为int&，那么T&&->int&+&&折叠成int&
	cout << "func(c):" << endl;
	func(c);// c是左值，不可能用右值引用来引用，所以func推导T为int&，那么T&&->int&+&&折叠成int&

	return 0;
}
/*
代码运行打印如下：
func(10): //T tmp = val; T是int
01 val:10
02 val:10 tmp:11
func(a): //T tmp = val; T是int&
01 val:10
02 val:11 tmp:11
func(std::move(a)): //T tmp = val; T是int
01 val:11
02 val:11 tmp:12
func(b): //T tmp = val; T是int&
01 val:11
02 val:12 tmp:12
func(c): //T tmp = val; T是int&
01 val:10
02 val:11 tmp:11

从上面的代码可以看出，func这个函数模板的T类型有时候推导出来是int类型，有时候是int&类型，这个通过上面的代码测试，希望大
家能够掌握。再说引用折叠，就是int && + &&折叠成int&&，除此之外，都折叠成int&，如int& + &&折叠成int&，知道这个就可
以，具体的应用再看下面对move和forward函数的讲解。
*/


///////////////////////////////////////////////////////////////

//std::move转移语义
//下面的代码的打印结果
class B
{
public:
	B(int data = 10) :ptr(new int(data)) {}
	~B() { delete ptr; ptr = nullptr; }

	//带有左值引用参数的拷贝构造函数
	B(const B& src)
	{
		cout << "B(const B&)" << endl;
		ptr = new int(*src.ptr);
	}

	//带有右值引用参数的拷贝构造函数
	B(B&& src)
	{
		cout << "B(B&&)" << endl;
		ptr = src.ptr;
		src.ptr = nullptr;
	}
private:
	int* ptr;
};
int main10()
{
	vector<B> vec;
	vec.reserve(10);
	for (int i = 0; i < 10; ++i)
	{
		B b(i);
		/*
		这里b是一个左值，因此vec.push_back(b)会调用左值的
		拷贝构造函数，用b拷贝构造vector底层数组中的对象
		*/
		vec.push_back(b);
	}
	return 0;
}
/*
B(const B&)
B(const B&)
B(const B&)
B(const B&)
B(const B&)
B(const B&)
B(const B&)
B(const B&)
B(const B&)
B(const B&)
调用了10次左值引用的拷贝构造函数，看上面的代码，A a(i)在for循环中其实算是局部对象，在vec.push_back(a)
完成后，a对象也就该析构了，所以在vec.push_back(a)时，应该把对象a的资源直接移动给vector容器底层的对象，
也就是调用右值引用参数的拷贝构造函数，怎么做到呢？这时候就用到了带移动语义的std::move函数，main函数代码修改如下:
*/
int main11() {
	vector<B> vec;
	vec.reserve(10);
	for (int i = 0; i < 10; ++i)
	{
		B b(i);
		/*
		由于a马上就会销毁，因此这里应该用右值引用参数
		的拷贝构造函数，效率会更高
		*/
		vec.push_back(std::move(b));
	}
	return 0;
}
/*
B(B&&)
B(B&&)
B(B&&)
B(B&&)
B(B&&)
B(B&&)
B(B&&)
B(B&&)
B(B&&)
B(B&&)
此时在vec.push_back(std::move(a))这段代码中会调用到a对象的右值引用参数的拷贝构造函数。可以看move函数的源码，其
实move就是返回传入的实参的右值引用类型，做了一个类型强转，move代码：
template<class _Ty>
_NODISCARD constexpr remove_reference_t<_Ty>&&
	move(_Ty&& _Arg) noexcept
{	// forward _Arg as movable
	return (static_cast<remove_reference_t<_Ty>&&>(_Arg));
}
*/

///////////////////////////////////////////////////////////////

//std::forward类型完美转发
//代码示例，实现一个简单的vector，来描述forward的应用场景，示例代码如下：
//容器里面元素的类型
class C
{
public:
	C() {}
	// 带左值引用参数的赋值函数
	C& operator=(const C& src)
	{
		cout << "operator=" << endl;
		return *this;
	}
	// 带右值引用参数的赋值函数
	C& operator=(C&& src)
	{
		cout << "operator=(C&&)" << endl;
		return *this;
	}
};
// 容器的类型
template<typename _Ty>
class Vector
{
public:
	Vector():mcur(0){
	}
	// 引用左值的push_back函数
	void push_back(const _Ty& val)
	{
		addBack(val);
	}
	// 引用右值的push_back函数
	void push_back(_Ty&& val)
	{
		// 这里传递val时，要用move转换成右值引用类型，
		// 因为val本身是左值，有名字有地址，见前面引用折叠部分的说明
		addBack(std::move(val));
	}
public:
	enum { VEC_SIZE = 10 };
	_Ty mvec[VEC_SIZE];
	int mcur;

	//template<typename _Ty>
	//void addBack(_Ty&& val)
	//{
	//	/*
	//	这里val本身永远是左值，所以不可能调用
	//	容器内部对象的右值引用参数的operator=赋值函数
	//	*/
	//	mvec[mcur++] = val;
	//}
	template<typename _Ty>
	void addBack(_Ty&& val)
	{

		//这里使用std::forward，可以获取val引用的实参的引用类型，
		//是左引用，还是右引用，原理就是根据“引用折叠规则”
		//int&+&&->int&     int&&+&&->int&&
		mvec[mcur++] = std::forward<_Ty>(val);
	}
};

int main()
{
	Vector<C> vec;
	C c;
	vec.push_back(c); // 调用A的左值引用的赋值函数
	vec.push_back(C()); // 理应调用A的右值引用参数的赋值函数，却调用了左值引用的赋值函数
	return 0;
}
/*
operator=
operator=

vec.push_back(A())这句代码传入的是临时对象，最终却没有调用A对象的右值引用参数的赋值函数operator=，
主要原因就是在Vector中addBack函数里面，val永远被当作左值了，无法保持它接收的实参的引用类型，是左引用
还是右引用，此时std::forward就要起作用了，它称作“类型完美转发”，也就是说可以保持实参数据的左引用或者
右引用类型，上面的addBack函数修改如下：
template<typename _Ty>
void addBack(_Ty &&val)
{
	
	//这里使用std::forward，可以获取val引用的实参的引用类型，
	//是左引用，还是右引用，原理就是根据“引用折叠规则”
	//int&+&&->int&     int&&+&&->int&&
	mvec[mcur++] = std::forward<_Ty>(val);
}
修改成这样之后，代码运行如下：
operator=
operator=(C&&)
完美！vec.push_back(A())这句代码，最终调用了A对象的右值引用参数的赋值函数operator=，符合预期。因为
在addBack中使用了std::forward类型完美转发机制，它的源码实现如下：
template<class _Ty>
_NODISCARD constexpr _Ty&& forward(remove_reference_t<_Ty>& _Arg) noexcept
{	// forward an lvalue as either an lvalue or an rvalue
	return (static_cast<_Ty&&>(_Arg));
}

template<class _Ty>
_NODISCARD constexpr _Ty&& forward(remove_reference_t<_Ty>&& _Arg) noexcept
{	// forward an rvalue as an rvalue
	static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
	return (static_cast<_Ty&&>(_Arg));
}
上面是C++库里面提供的两个forward重载函数，分别接收左值和右值引用类型，进行一个类型强转，(static_cast<_Ty&&>(_Arg))， 
如果实参类型是int& + && -> int&就保持了实参的左值引用类型，如果实参类型是int&& + && -> int&&就保持了实参的右值引用类型。

【总结】：std::move是获取实参的右值引用类型；std::forward是在代码实现过程中，保持实参的原有的引用类型（左引用或者右引用类型）。
*/

