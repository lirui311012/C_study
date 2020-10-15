#include <iostream>

using namespace std;
//模仿实现一个stl中的vector,_first,_last,_end

//stl
//template <class _Ty, class _Alloc = allocator<_Ty>>
//class vector {};

//mine
template<typename T>
class vector
{
public:
	vector(int size = 10) 
	{
		_first = new T[size];
		_last = _first;
		_end = _first + size;
	}
	~vector() {
		delete[]_first;
		_first = _last = _end = nullptr;
	}
	vector(const vector<T>& rhs) {
		//地址相减得到的答案就是元素个数
		int size = rhs._end - rhs._first;
		_first = new T[size];
		int len = rhs._last - rhs._first; // rhs里面有效元素个数
		for (int i = 0;i < len;i++) {
			_first[i] = rhs._first[i];
		}
		_last = _first + len;
		_end = _first + size;

	}
	vector<T>& operator=(const vector<T>& rhs) {
		if (this == &rhs)
			return *this;

		delete _first;

		//地址相减得到的答案就是元素个数
		int size = rhs._end - rhs._first;
		_first = new T[size];
		int len = rhs._last - rhs._first; // rhs里面有效元素个数
		for (int i = 0; i < len; i++) {
			_first[i] = rhs._first[i];
		}
		_last = _first + len;
		_end = _first + size;
		return *this;
	}

	//向容器末尾添加元素
	void push_back(const T& val) {
		if (full())
			expand();
		*_last++ = val;  //往_last位置赋值，然后_last向后移动
	}

	//从容器末尾删除元素
	void pop_back() {
		if (empty())
			return;
		--_last;
	}

	//返回容器末尾元素的值
	T back() const{
		return *(_last-1);
	}
	bool full() const{
		return _last == _end;
	}
	bool empty() const{
		return _last == _first;
	}
	int size() {
		//返回容器中元素个数
		return (_last - _first);
	}
private:
	T* _first; // 指向数组起始的位置
	T* _last;  // 指向数组中有效元素的后继位置
	T* _end;   // 指向数组空间的后继位置

	//2倍扩容
	void expand() {
		//其实len 和 size相等,因为容器已经满了，此实_last == _end
		int size = _end - _first;
		int len = _last - _first;
		T* ptmp = new T[size * 2];
		for (int i = 0; i < len; i++) {
			ptmp[i] = _first[i];
		}
		delete[]_first;
		_first = ptmp;
		_last = _first + len;
		_end = _first + size * 2;
	}
};

int main()
{
	vector<int> vec;
	for (int i = 0;i < 20;i++) {
		vec.push_back(rand() % 100);
	}
	while (!vec.empty()) {
		cout << vec.back() << " ";
		vec.pop_back();
	}
	cout << endl;

	return 0;
}