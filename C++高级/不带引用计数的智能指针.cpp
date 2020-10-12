#include <iostream>
#include  <memory>  //使用智能指针，需要包含该头文件<memory>
using namespace std;

/*
不带引用计数的智能指针
auto_ptr,scoped_ptr,unique_ptr
*/



template <typename T>
class CSmartPtr 
{
public:
	CSmartPtr(T *ptr)
		:mptr(ptr) {}
	~CSmartPtr() { delete mptr; }
	T& operator*() { return *mptr; }
	T* operator->() { return mptr; }
private:
	T* mptr;
};

int main() {
#if 0
	CSmartPtr<int> p1(new int);
	CSmartPtr<int> p2(p1);  //拷贝构造
	//程序直接崩溃
#endif
	/*
	当前这个自定义智能指针面临的一个问题就是，怎么解决浅拷贝问题？
		解决方案：
		1.不带引用计数  
		2.带引用计数
	*/

	/*
	不带引用计数的智能指针
	auto_ptr:C++库里面
	scoped_ptr:C++11新标准-
	unique_ptr:C++11新标准
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