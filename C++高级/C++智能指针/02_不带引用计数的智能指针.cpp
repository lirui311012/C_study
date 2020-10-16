
#include <iostream>
#include  <memory>  //使用智能指针，需要包含该头文件<memory>
using namespace std;

/*
不带引用计数的智能指针
auto_ptr,scoped_ptr,unique_ptr
不带引用计数的智能指针里面：推荐使用unique_ptr
*/



template <typename T>
class CSmartPtr
{
public:
	CSmartPtr(T* ptr)
		:mptr(ptr) {}
	~CSmartPtr() { delete mptr; }
	T& operator*() { return *mptr; }
	T* operator->() { return mptr; }
private:
	T* mptr;
};
////////////////////////////////////////////////////////


int main01() {

	CSmartPtr<int> p1(new int);
	cout << "===========" << endl;
	CSmartPtr<int> p2(p1);  //拷贝构造
	//程序直接崩溃

	/*
	当前这个自定义智能指针面临的一个问题就是，怎么解决浅拷贝问题???
		解决方案：
		1.不带引用计数
		2.带引用计数
	*/
	return 0;
}

////////////////////////////////////////////////////////
//auto_ptr
int main02() {

	/*不带引用计数的智能指针
	auto_ptr:C++库里面------不推荐使用
	scoped_ptr:C++11新标准--很少用
	unique_ptr:C++11新标准--推荐使用
	*/

	auto_ptr<int> ptr1(new int);
	auto_ptr<int> ptr2(ptr1);
	*ptr2 = 20;
	cout << *ptr2 << endl;  //没问题
	cout << *ptr1 << endl;  //崩溃
	//上面代码直接程序崩溃
	//auto_ptr(auto_ptr& _Right) noexcept : _Myptr(_Right.release()) {}
	//这是库里面auto_ptr的拷贝构造，先把ptr1进行release然后把返回值给ptr2进行构造
	//release：
	//_Ty* release() noexcept {
	//_Ty* _Tmp = _Myptr;
	//_Myptr = nullptr;
	//return _Tmp;
	//}
	//可以看出ptr1调用release是把ptr1的内部封装的指针置为nullptr,然后将之前的地址返回出来
	//所以程序崩溃的原因是ptr1内部封装的地址已经为nullptr，但是你还对其进行*ptr1所以崩溃
	//operator*()如下
	/*
		_NODISCARD _Ty& operator*() const noexcept {
			#if _ITERATOR_DEBUG_LEVEL == 2
			_STL_VERIFY(_Myptr, "auto_ptr not dereferenceable");
			#endif // _ITERATOR_DEBUG_LEVEL == 2

			return *get();
		}
		_NODISCARD _Ty* get() const noexcept {
			return _Myptr;
		}
	*/

	//不推荐使用auto_ptr

	/*
	auto_ptr<int> ptr1(new int);
	auto_ptr<int> ptr2(ptr1);
	ptr1底层指针为nullptr
	*/


	return 0;
}




////////////////////////////////////////////////////////
/*
scoped_ptr
//c++11新语法 void test() = delete将这个函数删除掉
scoped_ptr(const scoped_ptr<T>&) = delete;
scoped_ptr<T>& operator=(const scoped_ptr<T>&) = delete;
删除了拷贝构造和operator=运算符重载函数
*/
//不带引用计数的智能指针---推荐使用unique_ptr
/*
unique_ptr
unique_ptr也将operator=和拷贝构造函数进行了delete函数删除
unique_ptr(const unique_ptr<T>&) = delete;
unique_ptr<T>& operator=(const unique_ptr<T>&) = delete;
unique_ptr( unique_ptr<T>&& src);  //右值引用
unique_ptr<T>& operator=( unique_ptr<T>&& src);
//unique_ptr提供了带右值引用参数的operator=和拷贝构造函数

有如下操作：
template<typename T>
unique_ptr<T> getSmartPtr(){
	unique_ptr<T> ptr(new int);
	return ptr;
}
unique_ptr<int> ptr1 = getSmartPtr<int>();

ptr1 = getSmartPtr<int>();


*/
int main() {
	unique_ptr<int> ptr1(new int);
	//unique_ptr<int> ptr2(ptr1);  //编译器报错：尝试引用已删除的函数


	//std::move ==>c++11 右值引用  std::move()得到当前变量的右值类型
	//这样就是在调用unique_ptr的带有右值引用参数的拷贝构造函数
	unique_ptr<int> ptr2(std::move(ptr1));  //ptr1不再持有资源
	/*
	仔细观察，unique_ptr<int> ptr2(ptr1);报错
	unique_ptr<int> ptr2(std::move(ptr1));虽然这样操作之后，ptr1失去了资源持有权限
	但是这是在用户可以感知到的情况下来进行的，std::move(ptr1)返回右值引用
	再看一下auto_ptr，它支持auto_ptr<int> ptr2(ptr1);，然后底层将资源给了ptr2
	，并且ptr1底层指针为nullptr,在用户无法感知的情况下，将资源剥夺，不好！很危险
	*/
	return 0;
}


/*
结论：
不带引用计数的智能指针推荐使用unique_ptr!!!
*/