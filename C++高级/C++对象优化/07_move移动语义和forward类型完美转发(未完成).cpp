#include <iostream>
#include <vector>
using namespace std;

//还是以前的String，带右值引用参数的拷贝构造和带右值引用参数的赋值重载函数
//暂时不更新move移动语义 和 forward类型完美转发，以后更新
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
String operator+(const String& lhs, const String& rhs)
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

	//#3 效率最高
	String tmpStr;
	tmpStr._pstr = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
	strcpy(tmpStr._pstr, lhs._pstr);
	strcat(tmpStr._pstr, rhs._pstr);
	return tmpStr;
}

ostream& operator<<(ostream& out, const String& str) {
	out << str._pstr;
	return out;
}


////////////////////////////////////////////////////////
int main() {


	return 0;
}