#include <iostream>
using namespace std;

/*
std::move(左值):移动语义函数，输入一个左值，得到右值类型
std::forward<>():类型完美转发函数，能够识别左值和右值类型
*/

/*
使用之前写好的String(带有右值引用参数的拷贝构造和带有右值引用参数的operator=)
使用之前写好的带有空间配置器的自定义vector容器

//函数模板类型推演+引用折叠
String& && val == String& val
Strin&& && val == String && val
*/

//自己实现的String类
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
String operator+(const String& lhs, const String& rhs)
{
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



//定义vector容器的空间适配器，和c++标准库的allocator实现一样
template <typename T>
struct Allocator {
	T* allocate(size_t size) { //负责内存开辟
		return (T*)malloc(sizeof(T) * size);
	}
	void deallocate(void* p) {  //负责内存释放
		free(p);
	}

	//在一个已经开辟好的内存p上构造一个值为val的对象
	//接收左值
	/*void construct(T* p, const T& val) {//负责对象构造
		//定位new，在指定地址空间构造一个对象，拷贝构造函数哦
		new (p) T(val);  //定位new
	}
	//接收右值
	void construct(T* p, T&& val) {
		//new (p) T(val);
		new (p) T(std::move(val));
		//这里的问题也是一样的，val是一个右值引用类型变量，本身是一个左值，也需要移动语义
	}
	*/
	//将construct函数也通过模板进行优化
	template <typename Ty>
	void construct(T* p, Ty&& val) //通过引用折叠可以得到左值和右值类型
	{
		new (p) T(std::forward<Ty>(val));
	}
	//这样就可以匹配到String的左值引用拷贝构造和右值引用拷贝构造函数

	void destroy(T* p) {//负责对象的析构
		p->~T();   //~T()代表了T类型的析构函数
	}
};

template<typename T, typename Alloc = Allocator<T>>
class vector
{
public:
	vector(int size = 10)
	{
		//1111.需要把对象构造和内存开辟分开处理
		//_first = new T[size];
		_first = _allocator.allocate(size);
		_last = _first;
		_end = _first + size;
	}
	~vector() {
		//2222.释放容器“有效”的元素对象，然后释放_first指针指向的堆内存
		//delete[]_first;

		//mine
		/*for (int i = 0;i < _last-_first;i++) {
			_allocator.destroy(_first+i);
		}*/


		//把_first指针指向的数组的有效元素进行析构操作
		for (T* p = _first; p != _last; p++) {
			_allocator.destroy(p);
		}
		//释放堆上的数组内存
		_allocator.deallocate(_first);
		_first = _last = _end = nullptr;
	}
	vector(const vector<T>& rhs) {
		//地址相减得到的答案就是元素个数
		int size = rhs._end - rhs._first;
		//_first = new T[size];
		_first = _allocator.allocate(size);

		int len = rhs._last - rhs._first; // rhs里面有效元素个数
		for (int i = 0; i < len; i++) {
			//_first[i] = rhs._first[i];
			_allocator.construct(_first + i, rhs._first[i]);
		}
		_last = _first + len;
		_end = _first + size;

	}
	vector<T>& operator=(const vector<T>& rhs) {
		if (this == &rhs)
			return *this;

		//delete _first;
		//把_first指针指向的数组的有效元素进行析构操作
		for (T* p = _first; p != _last; p++) {
			_allocator.destroy(p);
		}
		//释放堆上的数组内存
		_allocator.deallocate(_first);


		//地址相减得到的答案就是元素个数
		int size = rhs._end - rhs._first;
		//_first = new T[size];
		_first = _allocator.allocate(size);

		int len = rhs._last - rhs._first; // rhs里面有效元素个数
		for (int i = 0; i < len; i++) {
			//_first[i] = rhs._first[i];
			_allocator.construct(_first + i, rhs._first[i]);
		}
		_last = _first + len;
		_end = _first + size;
		return *this;
	}

	//从容器末尾删除元素
	void pop_back() {
		if (empty())
			return;
		//--_last;
		//不仅_last指针--，而且还要析构对象噢噢噢噢
		--_last;
		_allocator.destroy(_last);
	}

	//返回容器末尾元素的值
	T back() const {
		return *(_last - 1);
	}
	bool full() const {
		return _last == _end;
	}
	bool empty() const {
		return _last == _first;
	}
	int size() {
		//返回容器中元素个数
		return (_last - _first);
	}

	//////////////////////////////////////////////////////////////////////////
	//向容器末尾添加元素  接收左值
	/*void push_back(const T& val) {
		if (full())
			expand();
		_allocator.construct(_last, val);
		_last++;
	}
	//接收右值  一个右值引用变量本身还是一个左值(不要忘记)
	void push_back(T&& val) {
		if (full())
			expand();
		//相当于把val强转成右值引用类型
		_allocator.construct(_last, std::move(val));
		_last++;
	}
	*/

	//将刚刚实现的push_back两个重载函数屏蔽掉，我们使用模板实现
	template<typename Ty>
	void push_back(Ty&& val)//String& && val == String& val
	{                       //String&& &&val == String&& val
							//右值引用折叠
		if (full())
			expand();
		//注意，不管你是左值引用还是右值引用，val这个变量依然都是一个左值，在这里依然匹配左值引用函数
		//_allocator.construct(_last, val);

		//所以引出了forward类型的完美转发
		//根据val的而定义，如果val是一个左值引用变量，那么就返回左值
		//如果val是一个右值引用变量 ，那么就返回一个右值
		//所以就可以匹配左值引用参数的construct和带有右值引用参数的construct函数
		//forward类型的完美转发，能够识别左值和右值类型
		_allocator.construct(_last, std::forward<Ty>(val));
		_last++;
	}
	//所以当你传入一个左值引用String&的时候就相当于void push_back(String& val)
	//所以当你传入一个左值引用String&&的时候就相当于void push_back(String&& val)
	//结论：函数模板类型推演+引用折叠==就可以得到左值引用参数的函数和右值引用参数的函数
	//////////////////////////////////////////////////////////////////////////

private:
	T* _first; // 指向数组起始的位置
	T* _last;  // 指向数组中有效元素的后继位置
	T* _end;   // 指向数组空间的后继位置
	Alloc _allocator;//定义容器的空间适配器对象
	//2倍扩容
	void expand() {
		//其实len 和 size相等,因为容器已经满了，此实_last == _end
		int size = _end - _first;
		int len = _last - _first;
		//T* ptmp = new T[size * 2];
		T* ptmp = _allocator.allocate(size * 2);
		for (int i = 0; i < len; i++) {
			//ptmp[i] = _first[i];
			_allocator.construct(ptmp + i, _first[i]);
		}

		//delete[]_first;
		for (T* p = _first; p != _last; p++) {
			_allocator.destroy(p);
		}
		_allocator.deallocate(_first);
		_first = ptmp;
		_last = _first + len;
		_end = _first + size * 2;
	}
};


int main() {
	String str1 = "aaa";
	vector<String> vec;
	cout << "============================" << endl;
	vec.push_back(str1); //左值String  --类型推导String&左值引用
	vec.push_back(String("bbb")); //右值类型String &&
	cout << "============================" << endl;

	return 0;
}

/*
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