#include <iostream>
#include <vector>
using namespace std;

//右值引用

/*
结论：
1.一个右值引用本身是一个左值=====也就是说，右值引用变量在赋值号右边的时候，左边必须是左值引用
2.常量数字如30，20，临时量，都是右值====必须使用右值引用去引用
*/


//还是以前的String没有改动过
class String {
public:
	String(const char* p = nullptr) {
		cout << "String(const char*)" << endl;
		if (p != nullptr) {
			int len = strlen(p);
			_pstr = new char[len + 1];
			strcpy(_pstr, p);
		}
		else {
			_pstr = new char[1];
			*_pstr = '\0';
		}
	}
	//深拷贝
	String(const String& str) {
		cout << "String(const String&)" << endl;
		_pstr = new char[strlen(str._pstr) + 1];
		strcpy(_pstr, str._pstr);
	}
	String& operator=(const String& str) {
		cout << "operator=()" << endl;
		if (this == &str) {
			return *this;
		}
		delete[]_pstr;
		_pstr = new char[strlen(str._pstr) + 1];
		strcpy(_pstr, str._pstr);
		return *this;
	}
	~String() {
		cout << "~String()" << endl;
		delete[]_pstr;
		_pstr = nullptr;
	}
	const char* c_str()const { return _pstr; }
private:
	char* _pstr;

};

//右值引用
int main() {

	int a = 10;
	int& b = a;  //左值：有内存，有名字
	             //用左值引用去引用它没有任何问题

	//int&& c = a;  //报错：无法将左值绑定到右值引用 ,无法从“int”转换为“int &&”
	//int && 这个叫做右值引用，右值引用是用来绑定右值的   //右值：没有名字(如临时量)或者没内存
	//int &  左值引用是用来绑定左值的

	//int& c = 20;  //编译器报错哦无法从“int”转换为“int &，20没内存，是在寄存器中的,也没有名字,20是一个右值
	                                  //不能用一个左值引用去绑定一个右值
	const int& c = 20;  //这样就可以啦
	/*
	底层是这样做的
	int tmp = 20;
	const int &c = tmp;   //用常引用
	*/

	
	int&& d = 20; //右值引用,用右值引用去引用一个右值，ok
	              //可以把一个右值，绑定到一个右值引用上面
	/*
	底层是这样做的
	int tmp = 20;
	int &&d = tmp;   //用常引用
	*/
	//上面的const int &c = 20;  和int && d = 20;汇编指令其实一模一样
	//但是通过d可以改变临时量的值，通过c不可以改变临时量的值



	//String& e = String("aaa");   // 编译器报错：非常量引用只能绑定到左值
	//      String("aaa")没有名字，所以认为是右值，右值不可以用左值引用去绑定
	String&& e = String("aaa");   //因该使用右值引用去绑定右值,或者使用const常引用
	//const String& e = String("bbb");  //没问题


	//int&& f = d;  //d是上面定一个一的一个右值引用
				//编译器报错： 无法从“int”转换为“int &&”，无法将左值绑定到右值引用
				//记住这句话，一个右值引用本身是一个左值
				//int &&d = 20; 的底层相当于引用了一块内存，有内存，有名字，名字就叫做d==》左值
	int& f = d;  //使用左值引用去引用就可以啦


	return 0;
}