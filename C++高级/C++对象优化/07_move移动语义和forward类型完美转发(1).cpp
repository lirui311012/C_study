#include <iostream>
using namespace std;

/*
使用之前写好的String(带有右值引用参数的拷贝构造和带有右值引用参数的operator=)
使用之前写好的带有空间配置器的自定义vector容器
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
	void construct(T* p, const T& val) {//负责对象构造
		//定位new，在指定地址空间构造一个对象，拷贝构造函数哦
		new (p) T(val);  //定位new
	}
	//接收右值
	void construct(T* p, T&& val) {
		//new (p) T(val);
		new (p) T(std::move(val));
		//这里的问题也是一样的，val是一个右值引用类型变量，本身是一个左值，也需要移动语义
	}
	void destroy(T* p) {//负责对象的析构
		p->~T();   //~T()代表了T类型的析构函数
	}
};

template<typename T, typename Alloc = Allocator<T>>
class vector
{
public:
	//vector(int size = 10,const Alloc &alloc = Allocator<T>())
	//	:_allocator(alloc)
	//{
	//	//1111.需要把对象构造和内存开辟分开处理
	//	_first = new T[size];
	//	_last = _first;
	//	_end = _first + size;
	//}
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
	void push_back(const T& val) {
		if (full())
			expand();
		//*_last++ = val;  //往_last位置赋值，然后_last向后移动
		//在_last指针指向的地址空间构造一个对象
		_allocator.construct(_last, val);
		_last++;
	}
	//接收右值  一个右值引用变量本身还是一个左值(不要忘记)
	void push_back(T&& val) {
		if (full())
			expand();
		//_allocator.construct(_last, val);  
					//注意这里，形参val是一个右值引用变量
					//但是右值引用变量本身是一个左值，所以在_allocator.construct(_last, val); 
					//函数匹配的依然是一个左值引用的函数，我们现在希望匹配右值引用参数的函数，怎么办？
		
		//相当于把val强转成右值引用类型
		_allocator.construct(_last,std::move(val));
		/*
		move实现：
		template <class _Ty>
		_NODISCARD constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept { // forward _Arg as movable
			return static_cast<remove_reference_t<_Ty>&&>(_Arg);
		}
		*/
		_last++;
	}
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
	vec.push_back(str1);
	vec.push_back(String("bbb"));
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

由于主函数里面vec.push_back(String("bbb"));
这是一个push_back传入参数是一个临时对象，是一个右值，所以调用vec的带有右值引用参数的push_back函数
在void push_back(T&& val)里面,val本身是一个左值，所以_allocator.construct(_last,std::move(val));
需要使用std::move转移语义函数将val强转为右值，空间配置器里面同样的道理void construct(T* p, T&& val)
val本身也是一个左值，所以需要new (p) T(std::move(val));，这样就可以调用到String类里面的String(String&&)(带有右值引用参数的拷贝构造函数)
*/