#include <iostream>
using namespace std;


////////////////////////////////////////////////////////////
#if 0
//自己实现一个字符串对象
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
String GetString(String &str) {
	const char* pstr = str.c_str();
	String tmpStr(pstr);
	return tmpStr;
}
int main()
{
	String str1("aaaaaaaaaaaaaaa");
	String str2;
	str2 = GetString(str1);
	cout << "str2 = " << str2.c_str() << endl;

	return 0;
}
/*运行结果
String(const char*)
String(const char*)
String(const char*)
String(const String&) =>tmpStr拷贝构造main函数栈帧上的临时对象
~String()
operator=()  =>main函数上的临时对象给t2赋值
~String()
str2 = aaaaaaaaaaaaaaa
~String()
~String()
*/

//可以看出效率其实不是很高，临时对象那里的效率太低啦
#endif

////////////////////////////////////////////////////////////
//优化：提高了效率哦
//C++11提供了带右值引用参数的成员方法

//引入了：//带右值引用参数的拷贝构造和带右值引用参数的赋值重载函数
#if 1
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
	
	//带左值引用参数的拷贝构造
	String(const String& str) {
		cout << "String(const String&)" << endl;
		_pstr = new char[strlen(str._pstr) + 1];
		strcpy(_pstr, str._pstr);
	}

	//带右值引用参数的拷贝构造，str引用的就是一个临时对象,临时对象没有名字,编译器将其处理成右值
	String(String&& str) {//普通的对象一定会匹配到左值引用上；没名字，没内存就会匹配到这个右值引用上面
		cout << "String(String&&)" << endl;
		_pstr = str._pstr;  //直接将临时对象的成员属性所指向的堆区空间，拿走
		str._pstr = nullptr;  //这句话一定要有，否则就是浅拷贝哦
	}

	//带左值引用参数的赋值重载函数
	String& operator=(const String& str) {
		cout << "operator=(const String&)" << endl;
		if (this == &str) {
			return *this;
		}
		delete[]_pstr;
		_pstr = new char[strlen(str._pstr) + 1];
		strcpy(_pstr, str._pstr);
		return *this;
	}

	//带右值引用参数的赋值重载函数
	String& operator=(String&& str) {  //临时对象可以匹配到该函数
		cout << "operator=(String&&)" << endl;
		if (this == &str) {
			return *this;
		}
		delete[]_pstr;

		_pstr = str._pstr; 
		str._pstr = nullptr;

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
String GetString(String& str) {
	const char* pstr = str.c_str();
	String tmpStr(pstr);
	return tmpStr;
}
int main()
{
	String str1("aaaaaaaaaaaaaaa");
	String str2;
	str2 = GetString(str1);
	cout << "str2 = " << str2.c_str() << endl;

	return 0;
}
/*
打印结果：
String(const char*)  //构造str1
String(const char*)  //构造str2
String(const char*)  //构造tmpStr
String(String&&)
~String()
operator=(String&&)
~String()
str2 = aaaaaaaaaaaaaaa
~String()
~String()

//效率的提升在于，内存堆区空间的申请和释放次数大大减少
*/
#endif



