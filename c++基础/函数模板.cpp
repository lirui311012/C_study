#include <iostream>
using namespace std;


//模板声明
template<typename T>
bool compare(T a,T b); //compare<const char *> *UND*

//普通函数声明
bool compare(const char* a, const char* b);

int main() {
	//函数的调用点
	//bool __cdecl compare<int>(int,int)
	//bool __cdecl compare<double>(double,double)
	compare<int>(10, 20);
	compare<double>(10.5,20.5);
	compare(10, 20);
	compare<int>(10.5, 20);
	compare("aaa","bbb");
	compare<const char *>("aaa","bbb");

	return 0;
}