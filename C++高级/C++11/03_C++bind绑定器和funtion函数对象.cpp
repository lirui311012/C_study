#include <iostream>
#include <string>
#include <map>
#include <functional>  //使用function函数对象类型
using namespace std;

/*
function函数对象的特点：
能够把函数，绑定器，函数对象，lambda表达式的类型保留起来,在其他地方都可以使用
*/


/*
C++11提供的绑定器和函数对象
	 bind ,function
	 STL bind1st ,bind2nd =》 绑定器的结果本身还是一个函数对象

function : 绑定器，函数对象，lambda表达式 (本身都是函数对象)  他们只能使用在一条语句中
		   如果想在多条语句中应用怎么办？如何将其类型保留下来？这就是function的作用
*/


///////////////////////////////////////////////////////////////////

void hello1()
{
	cout << "hello world!" << endl;
}
void hello2(string str)
{
	cout << str << endl;
}

int sum(int a, int b)
{
	return a + b;
}

class Test
{
public:
	/*
	指向全局的C函数的函数指针和指向类的成员方法的函数指针是不同的
	因为普通函数直接调用就行，而成员方法必须依赖于对象来调用
	例如上面的void hello2(string str) --> void (*pfunc)(string)
	下面的成员方法：void hello(string str) --> void (Test::*pfunc)(string)
	加上作用域用来表示通过函数指针间接调用成员方法的时候前面必须以来一个对象
	*/
	void hello(string str)
	{
		cout << str << endl;
	}
};

int main01()
{
	//从function的类模板定义出，看到希望用一个函数类型实例化function -->template <class _Fty>class function{};
	//区分好函数类型和函数指针类型
	//如：void hello1();
	//函数类型:void()   //只给出返回值和参数列表即可
	//函数指针类型:void(*)()
	function<void()> func1(hello1);  //或者function<void()> func1=hello1;//都是调用构造函数
					 //用函数对象类型func1将hello1的函数类型保留下来了
	func1();  //调用func1.operator()() => 里面又调用了hello1()函数

	function<void(string)> func2 = hello2;
	func2("123");   //调用func2.operator()(string)  => hello2("123");

	function<int(int, int)> func3 = sum;
	cout << func3(20, 30) << endl;

	//lambda表达式的本质就是函数对象
	function<int(int, int)> func4 =
		[](int a, int b)->int
	{
		return a - b;
	};
	cout << func4(100, 200) << endl;

	/*
	function函数对象类型，不仅可以直接将函数的类型(全局函数，类得成员函数也可以)保留下来，而且也可以将函数对象的类型保留下来
	因为函数对象的本质就是operator()的重载函数
	*/

	//成员函数第一个隐藏参数是this指针
	function<void(Test*, string)> func5 = &Test::hello;
	func5(&Test(), "call Test::hello");
	return 0;
}
/*
hello world!
123
50
-100
call Test::hello
*/

/*
总结：
使用function的注意事项：
1.需要使用函数类型实例化function
2.通过function调用operator()函数的时候，需要根据函数类型传入相应的参数
*/
///////////////////////////////////////////////////////////////////

/*
体现出function好用的例子
*/

void doShowAllBooks() { cout << "------------------查询所有书籍信息------------------" << endl; }
void doBorrow() { cout << "------------------借书------------------" << endl; }
void doBack() { cout << "------------------还书------------------" << endl; }
void doQueryBooks() { cout << "------------------查询书籍------------------" << endl; }
void doLoginOut() { cout << "------------------注销------------------" << endl; }

int main()
{
	int choice = 0;
	//虽说C函数指针也可以做到
	//但是函数指针只能接收普通的函数，不能接收绑定器绑定的函数对象，lambda表达式的函数对象
	//所以function更强大
	map<int, function<void()>> actionMap;
	//{}结构体的初始化
	actionMap.insert({ 1,doShowAllBooks });//insert(make_pair(xx,xx));
	actionMap.insert({ 2,doBorrow });
	actionMap.insert({ 3,doBack });
	actionMap.insert({ 4,doQueryBooks });
	actionMap.insert({ 5,doLoginOut });
	for (;;)
	{
		cout << "----------------" << endl;
		cout << "1.查看所有书籍信息" << endl;
		cout << "2.借书" << endl;
		cout << "3.还书" << endl;
		cout << "4.查询书籍" << endl;
		cout << "5.注销" << endl;
		cout << "----------------" << endl;
		cout << "请选择:";
		cin >> choice;

		auto it = actionMap.find(choice);
		if (it == actionMap.end())
		{
			cout << "输入数字无效，重新选择！" << endl;
		}
		else
		{
			//map  pair  first  second
			it->second();  //second是function函数对象
		}

		/*
		//这样实现不好，因为这块代码无法闭合，无法做到“开-闭原则”
		switch (choice)
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			break;
		}
		*/
	}
	return 0;
}
/*
----------------
1.查看所有书籍信息
2.借书
3.还书
4.查询书籍
5.注销
----------------
请选择:1
------------------查询所有书籍信息------------------
----------------
1.查看所有书籍信息
2.借书
3.还书
4.查询书籍
5.注销
----------------
请选择:2
------------------借书------------------
----------------
1.查看所有书籍信息
2.借书
3.还书
4.查询书籍
5.注销
----------------
请选择:3
------------------还书------------------
----------------
1.查看所有书籍信息
2.借书
3.还书
4.查询书籍
5.注销
----------------
请选择:4
------------------查询书籍------------------
----------------
1.查看所有书籍信息
2.借书
3.还书
4.查询书籍
5.注销
----------------
请选择:5
------------------注销------------------
----------------
1.查看所有书籍信息
2.借书
3.还书
4.查询书籍
5.注销
----------------
请选择:6
输入数字无效，重新选择！
----------------
1.查看所有书籍信息
2.借书
3.还书
4.查询书籍
5.注销
----------------
请选择:
*/