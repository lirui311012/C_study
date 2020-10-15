#include <iostream>
#include <string>
using namespace std;

#if 1
//自己实现一个字符串对象
class String {
public:
	String(const char* p = nullptr) {
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
		_pstr = new char[strlen(str._pstr) + 1];
		strcpy(_pstr, str._pstr);
	}
	String& operator=(const String& str) {
		if (this == &str) {
			return *this;
		}
		delete[]_pstr;
		_pstr = new char[strlen(str._pstr) + 1];
		strcpy(_pstr, str._pstr);
		return *this;
	}
	~String() {
		delete[]_pstr;
		_pstr = nullptr;
	}
	const char* c_str()const {
		return _pstr;
	}

	//同时提供两个operator[],常方法提供给常对象使用
	char& operator[](int index) {
		//返回引用是为了方便修改
		return _pstr[index];
	}
	const char& operator[](int index) const {
		//返回引用是为了方便修改
		return _pstr[index];
	}
	int length() const {
		return strlen(_pstr);
	}
	bool operator>(const String& str) const {
		return strcmp(_pstr, str._pstr) > 0;
	}
	bool operator<(const String& str) const {
		return strcmp(_pstr, str._pstr) < 0;
	}
	bool operator==(const String& str) const {
		return strcmp(_pstr, str._pstr) == 0;
	}

	//给String字符拆类型提供迭代器的实现
	class iterator {
	public:
		iterator(char* p = nullptr):_p(p){}
		bool operator!=(const iterator &it) {
			return _p != it._p;
		}
		void operator++() {
			++_p;
		}
		char& operator*() {
			return *_p;
		}
	private:
		char* _p;
	};

	//begin返回的是容器底层首元素的迭代器的表示
	iterator begin() {return iterator(_pstr);}
	//end返回的是容器底层末尾元素后继位置的迭代器的表示
	iterator end() {return iterator(_pstr + length());}
private:
	char* _pstr;

	friend String operator+(const String& lhs, const String& rhs);
	friend ostream& operator<<(ostream& out, const String& str);
};


//String operator+(const String &lhs,const String &rhs) {
//	String tmp;
//	char* p = tmp._pstr;
//	delete[]p;
//	tmp._pstr = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
//	strcpy(tmp._pstr, lhs._pstr);
//	strcat(tmp._pstr, rhs._pstr);
//	return tmp;
//}
String operator+(const String& lhs, const String& rhs) {
	String tmp;
	char* p = tmp._pstr;
	delete[]p;
	tmp._pstr = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
	strcpy(tmp._pstr, lhs._pstr);
	strcpy(tmp._pstr + strlen(lhs._pstr), rhs._pstr);
	return tmp;
}

ostream& operator<<(ostream& out, const String& str) {
	out << str._pstr;
	return out;
}

#endif
int main()
{
	//迭代器的功能：
	/*
	提供一种统一的方式，来透明的遍历容器
	*/

	String str1 = "hello world!";

	//容器的迭代器，非常重要
	//String::iterator it = str1.begin();  //默认拷贝构造即可
	auto it = str1.begin();  //默认拷贝构造即可
	for (; it != str1.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;

	 //c++11 foreach的方式来遍历容器的内部元素的值=》底层还是通过迭代器实现
	for (char ch : str1) {
		cout << ch << " ";
	}
	cout << endl;


#if 0
	//char buffer[1024];
	//strcpy_s(buffer,1024,"abc");

	String str1;
	String str2 = "abcdef";  //string(const char *)
	String str3 = "bbbbb";
	String str4 = str2 + str3;
	String str5 = str2 + "ccc";
	String str6 = "ddd" + str2;
	if (str5 > str6) {
		cout << "str5 > str6" << endl;
	}
	else {
		cout << "str5 < str6" << endl;
	}

	cout << "str1 = " << str1 << endl;
	cout << "str2 = " << str2 << endl;
	cout << "str3 = " << str3 << endl;
	cout << "str4 = " << str4 << endl;
	cout << "str5 = " << str5 << endl;
	cout << "str6 = " << str6 << endl;

	cout << "--------------------------" << endl;
	int len = str6.length();
	cout << "strlen6.length() == " << len << endl;
	for (int i = 0; i < len; i++) {
		cout << str6[i] << " ";
	}
	cout << endl;

	//string  -- >const char *
	char buf[1024] = { 0 };
	strcpy_s(buf, 1024, str6.c_str());
	cout << "buf = " << buf << endl;
#endif

	return 0;
}