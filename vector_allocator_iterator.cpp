#include <iostream>
using namespace std;


//容器的空间配置器allocator做四件事情 内存开辟/内存释放  对象构造/对现象析构



//定义容器的空间适配器，和c++标准库的allocator实现一样
template <typename T>
struct Allocator {
	T* allocate(size_t size) { //负责内存开辟
		return (T*)malloc(sizeof(T) * size);
	}
	void deallocate(void *p) {  //负责内存释放
		free(p);
	}

	//在一个已经开辟好的内存p上构造一个值为val的对象
	void construct(T *p,const T& val) {//负责对象构造
		//定位new，在指定地址空间构造一个对象，拷贝构造函数哦
		new (p) T(val);  //定位new
	}
	void destroy(T* p) {//负责对象的析构
		p->~T();   //~T()代表了T类型的析构函数
	}
};




//模仿实现一个stl中的vector,_first,_last,_end

//stl
//template <class _Ty, class _Alloc = allocator<_Ty>>
//class vector {};

//mine


//容器底层内存开辟，内存释放，对象构造和对象析构，都通过allocator空间适配器来实现

template<typename T,typename Alloc=Allocator<T>>
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
			_allocator.construct(_first+i, rhs._first[i]);
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

	//向容器末尾添加元素
	void push_back(const T& val) {
		if (full())
			expand();
		//*_last++ = val;  //往_last位置赋值，然后_last向后移动
		//在_last指针指向的地址空间构造一个对象
		_allocator.construct(_last, val);
		_last++;
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

	//[]运算符重载
	T& operator[](int index) {
		if (index < 0 || index >= size()) {
			throw "OutOfRangeException";
		}
		return _first[index];
	}

	//迭代器一般是现成容器的嵌套类型
	class iterator {
	public:
		iterator(T *ptr = nullptr)
			:_ptr(ptr) {}
		bool operator!=(const iterator& it)const {
			return _ptr != it._ptr;
		}
		//迭代器前置++运算符重载
		void operator++() {
			_ptr++;
		}

		//迭代器解引用运算符
		T& operator*(){
			return *_ptr;
		}
		const T& operator*() const{
			return *_ptr;
		}
	private:
		T* _ptr;
	};
	//需要给容器提供begin和end方法
	iterator begin() {
		return iterator(_first);
	}
	iterator end() {
		return iterator(_last);
	}
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
			_allocator.construct(ptmp+i,_first[i]);
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

class Test{
public:
	Test() { cout << "Test()" << endl; }
	Test(const Test& other) { cout << "Test(const Test& other)" << endl; }
	~Test() { cout << "~Test()" << endl; }
private:
};

#if 1
int main() {
	vector<int> vec;
	for (int i = 0; i < 20; ++i) {
		vec.push_back(rand() % 100 + 1);
	}

	cout << "[]运算符重载" << endl;
	for (int i = 0;i < vec.size();++i) {
		cout << vec[i] << " ";
	}
	cout << endl;

	cout << "迭代器方式打印" << endl;
	//vector<int>::iterator it = vec.begin();
	auto it = vec.begin();
	for (; it != vec.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;

	//现在已经提供了迭代器，所以也可已使用foreach来遍历
	//foreach底层原理就是通过容器的迭代器实现容器遍历的
	cout << "foreach方式遍历" << endl;
	for (int val : vec) {
		cout << val << " ";
	}
	cout << endl;
	return 0;
}
#endif

#if 0
int main() {
	Test t1, t2, t3;
	cout << "----------------------" << endl;
	vector<Test> vec;
	vec.push_back(t1);
	vec.push_back(t2);
	vec.push_back(t3);
	cout << "----------------------" << endl;
	vec.pop_back();//3333.只需要析构对象，要把对象的析构和内存释放分离开，如果是delete会把这两件事情一起做完
	cout << "----------------------" << endl;


	return 0;
}
#endif




#if 0
int main()
{
	vector<int> vec;
	for (int i = 0; i < 20; i++) {
		vec.push_back(rand() % 100);
	}
	while (!vec.empty()) {
		cout << vec.back() << " ";
		vec.pop_back();
	}
	cout << endl;

	return 0;
}
#endif