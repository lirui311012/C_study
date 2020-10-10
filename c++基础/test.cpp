//#include <iostream>
//using namespace std;
//
//
//template<typename T>//定义函数模板的参数列表
//bool compare(T a, T b) {//compare是一个函数模板
//	cout << "tempate compare" << endl;
//	return a > b;
//}
//template<>
//bool compare<const char*>(const char* a, const char* b) {
//	cout << "template compare const char *" << endl;
//	return strcmp(a, b) > 0;
//}
////告诉编译器，进行指定类型的模板实例化
//template bool compare<int>(int, int);
//template bool compare<double>(double, double);
//
//
////非模板函数
//bool compare(const char* a, const char* b) {
//	cout << "normal compare function" << endl;
//	return strcmp(a, b) > 0;
//}