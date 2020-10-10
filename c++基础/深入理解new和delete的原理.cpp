#include <iostream>
using namespace std;

/*
new 和 delete

1.malloc和new区别？
a.malloc按字节开辟内存的；new开辟内存是需要指定类型new int[10]
 所以malloc开辟内存返回的都是void * ; operator new->int *
b.malloc只负责开辟空间，new不仅仅有malloc的功能，可以进行数据的初始化
   new int(10) ;   new int[20]  ;  new int[20]()
c.malloc开辟内存失败返回nullptr指针，new失败抛出的是bad_alloc类型的异常

2.free和delete区别？
delete :先析构对象，然后free回收空间

new -> operator new
delete -> operator delete
*/


/*
面试的时候如果面试官，让你设计一个检测内存泄漏的方案：
内存泄漏就是new操作没有对应的delete操作
那么可以重写operator new和operator delete
operator new[] 和operator delete[]  
记录你申请过哪些空间，那些空间没有被释放，可以采用哈希表的方式来记录
当程序运行结束前检测一下哈希表内的记录即可知道是否存在内存泄漏
*/

/*
内存池
对于实时性比较强的，小块内存频繁释放，频繁申请的场景就可以设计一个内存池
*/

/*
面经：new,delete,new[],delete[]能混用吗？c++为什么区分单个元素和数组的内存分配和释放呢？
new delete 
new[] delete[]
对于普通的编译器内置类型new/delete[] ，new[]/delete可以混合使用
自定义类类型，有析构函数，为了正确调用析构函数，那么开辟对象数组的时候new[]
会多开辟4个字节，记录对象的个数。
*/



//size_t -->unsigned int 无符号整型
//new   先调用operator new 开辟内存空间，然后调用对象的构造函数(初始化对象)
void* operator new(size_t size) {
	void* p = malloc(size);
	if (p == nullptr) {
		throw bad_alloc();
	}
	cout << "operator new addr:开辟的空间地址" << p << endl;
	return p;
}

//delete p;  调用p指向的对象的析构函数，再调用operator delete释放内存空间
void operator delete(void* ptr) {
	cout << "operator delete addr:释放的地址" << ptr << endl;
	free(ptr);
}

void* operator new[](size_t size) {
	void* p = malloc(size);
	if (p == nullptr) {
		throw bad_alloc();
	}
	cout << "operator new[] addr:开辟的空间地址" << p << endl;
	return p;
}
void operator delete[](void* ptr) {
	cout << "operator delete[] addr:释放的地址·" << ptr << endl;
	free(ptr);
}


//自定义类型
class Test {
public:
	Test() {
		cout << "Test()" << endl;
	}
	~Test() {
		cout << "~Test()" << endl;
	}
private:
	int ma;
};

int main() {

	//Test* p1 = new Test();
	//cout << "用户得到的地址：" << p1 << endl;
	//delete p1;   //开辟的空间地址和用户得到的地址一样

	Test* p2 = new Test[5];
	cout << "用户得到的p2 = " << p2 << endl;
	cout << "-----------" << endl;
	cout << *(int *)((char *)p2 - 4) << endl;   //5
	delete []p2;
	//cout << sizeof(Test) << endl;

#if 0
	try {
		int* p = new int;
		delete p;
		p = nullptr;

		int* q = new int[10];
		
		delete []q;
		q = nullptr;
	}
	catch (const bad_alloc & err) {
		cerr << err.what() << endl;
	
#endif

	return 0;
}