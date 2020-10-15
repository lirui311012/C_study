#include <iostream>
using namespace std;


//类模板


//类模板名称+类模板参数列表 = 类名
template <typename T=int>    //可以提供默认类型参数
class SeqStack {   //模板名称
public:
	// 构造和析构函数名不用加<T>，其它出现模板的地方都加上类型参数列表
	SeqStack(int size = 10) 
		: _pstack(new T[size])
		, _top(0)
		, _size(size)
	{}

	~SeqStack()
	{
		delete[]_pstack;
		_pstack = nullptr;
	}

	SeqStack(const SeqStack<T>& other)
		:_top(other._top)
		,_size(other._size)
	{
		_pstack = new T[_size];
		for (int i = 0;i < _size;i++)
		{
			_pstack[i] = other._pstack[i];
		}
	}
	SeqStack<T>& operator=(const SeqStack<T>& other)
	{
		if (this == &other)
			return *this;
		delete[]_pstack;
		_top = other._top;
		_size = other._size;
		_pstack = new T[_size];
		for (int i = 0; i < _size; i++)
		{
			_pstack[i] = other._pstack[i];
		}
		return *this;
	}

	void push(const T& val)
	{
		if (full())
			expand();  //扩容
		_pstack[_top++] = val;
	}
	void pop()
	{
		if (empty())
			return;
		_top--;
	}
	T top()const
	{
		if (empty())
			throw "stack is empty!";//抛异常也代表函数逻辑结束
		return _pstack[_top-1];

	}
	bool full()const
	{
		return _top == _size;
	}
	bool empty()const
	{
		return _top == 0;
	}
private:
	T* _pstack;
	int _top;
	int _size;

	// 顺序栈底层数组按2倍的方式扩容
	void expand();
};

template<typename T>
void SeqStack<T>::expand()
{
	T* ptmp = new T[_size * 2];
	for (int i = 0; i < _top; i++)
	{
		ptmp[i] = _pstack[i];
	}
	delete[]_pstack;
	_pstack = ptmp;
	_size *= 2;
}


int main() {
	//类模板的选择性实例化
	//模板类class SeqStack<int>{}
	SeqStack<int> s1;
	s1.push(20);
	s1.push(78);
	s1.push(32);
	s1.push(15);
	s1.pop();
	cout << s1.top() << endl;

	//默认类型参数int,类模板设置了template<typename T = int>
	SeqStack<> s2;


	return 0;
}



#if 0
template<typename T, int SIZE>
void sort(T* arr)
{
	for (int i = 0; i < SIZE - 1; ++i)
	{
		for (int j = 0; j < SIZE - 1 - i; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				int tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}
int main()
{
	int arr[] = { 12,5,7,89,32,21,35 };
	const int size = sizeof(arr) / sizeof(arr[0]);
	for (int val : arr)
	{
		cout << val << " ";
	}
	cout << endl;
	sort<int, size>(arr);

	cout << "=================" << endl;
	for (int val : arr)
	{
		cout << val << " ";
	}
	cout << endl;
	return 0;
}
#endif