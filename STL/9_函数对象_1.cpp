
/*
函数对象 =》 C语言里面的函数指针


第一段代码：c语言代码
int sum(int a,int b){
	return a+b;
}

int ret = sum(10,20);

第二段代码：c++代码
class Sum{
	public:
		int operator()(int a,int b){
			return a+b;
		}
};

Sum sum;
int ret = sum(10,20);
//函数对象的定义：
//把有operator()小括号运算符重载函数的对象，称作函数对象，或者称作仿函数
*/

#if 1

#include <iostream>
using namespace std;


//方案1：使用C语言的函数指针
template <typename T>
bool mygreater(T a, T b) {
	return a > b;
}
template <typename T>
bool myless(T a, T b) {
	return a < b;
}
//compare比喻成C++库函数模板
template<typename T,typename Compare>
bool compare(T a, T b,Compare comp) {

	//最重要的一个问题就是，通过函数指针调用函数，是没有办法内联的，
	//效率很低，因为有函数调用开销(函数调用堆栈)
	//因为内联是发生在编译阶段，编译阶段发生时内联函数，直接将函数代码写到函数调用位置
	//此处就算在mygreater和myless函数前面加上inline
	//在此处comp(a,b)(通过函数指针)也不知道调用的是mygreater和myless中的哪一个
	//所以通过函数指针调用函数，是无法内联的，是一定有标准的函数调用开销的
	//在C++里面的函数对象就解决了这个问题
	return comp(a, b);  
}



int main() {
	cout << compare(10, 20, mygreater<int>) << endl;  //比较大于
	cout << compare('b', 'y', myless<int>) << endl;  //比较小于

	return 0;
}
#endif
//上述代码其实使用模板实现已经挺好的啦
//那么问题来了，如果现在不比较大于关系，而实比较小于关系，该怎么办？
//修改源代码？将conpare里面的>更改为<?这样真的好吗？
/*
template<typename T>
bool compare(T a, T b) {
	return a > b;
}
*/

