#include <iostream>
#include <vector>
using namespace std;

/*
还是上一篇文章的代码，增加了operator+()重载函数
*/

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

	friend String operator+(const String& lhs, const String& rhs);
	friend ostream& operator<<(ostream& out, const String& str);
};
///////////////////////////////////////////////////////////
String operator+(const String &lhs,const String &rhs) 
{ 
	//#1
	//char* ptmp = new char[strlen(lhs._pstr)+strlen(rhs._pstr)+1];
	//strcpy(ptmp,lhs._pstr);
	//strcat(ptmp,rhs._pstr);
	//return String(ptmp); 
	
	//#2
	//char* ptmp = new char[strlen(lhs._pstr)+strlen(rhs._pstr)+1];
	//strcpy(ptmp,lhs._pstr);
	//strcat(ptmp,rhs._pstr);
	//String tmpStr(ptmp);
	//delete []ptmp;
	//ptmp = nullptr;
	//return tmpStr;

    //#3
	String tmpStr;
	tmpStr._pstr = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
	strcpy(tmpStr._pstr,lhs._pstr);
	strcat(tmpStr._pstr,rhs._pstr);
	return tmpStr;
}

ostream& operator<<(ostream &out,const String &str) {
	out << str._pstr;
	return out;
}
///////////////////////////////////////////////////////////


String GetString(String& str) {
	const char* pstr = str.c_str();
	String tmpStr(pstr);
	return tmpStr;
}
#if 0
int main()
{
	String str1 = "hello";
	String str2 = "world!";
	//#3
	cout << "=====================" << endl;
	String str3 = str1 + str2;
	cout << "=====================" << endl;
	cout << str3 << endl;
	return 0;
}
/*
运行结果：
String(const char*)
String(const char*)
String(const char*)
helloworld!
~String()
~String()
~String()
//运行结果还是不错的，但是你会发现在tring operator+(const String &lhs,const String &rhs)函数内部
//的new没有对应的delete，每做一次operator+就有一块内存没有delete  #1




改进：
String operator+(const String &lhs,const String &rhs)方法不再返回一个临时对象#2
结果：
String(const char*)
String(const char*)
String(const char*)
String(String&&)
~String()
helloworld!
~String()
~String()
~String()
仔细观察貌似效率不是很高


再次改进#3
运行结果：
String(const char*)
String(const char*)
=====================
String(const char*)
String(String&&)     <---------------注意这里
~String()
=====================
helloworld!
~String()
~String()
~String()



*/
#endif


#if 1
int main() {
	String str1 = "aaa";
	vector<String> vec;
	//预留空间,防止扩容
	vec.reserve(10);
	cout << "============================" << endl;
	vec.push_back(str1);
	vec.push_back(String("bbb"));
	cout << "============================" << endl;
	return 0;
}
/*
打印结果是什么？
String(const char*)
============================
String(const String&)
String(const char*)
String(String&&)
~String()
============================
~String()
~String()
~String()
*/
#endif

//其实可以看出自从c++11引入了右值引用后，其实一个函数返回一个容器，的开销实际上没有你想象的那么大
//那么vector的push_back是如何做到的选择左值引用参数的拷贝构造和右值引用参数的拷贝构造？
//还有就是右值引用参数里面涉及到的move和forword到底有什么作用？
//下篇文章介绍