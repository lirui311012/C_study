#include <iostream>
#include <queue>
#include <set>
#include <vector>
using namespace std;
//set底层,红黑树，默认打印出来是升序，从小到大



int main() {
	set<int> set1;
	//默认less<> 
	//0 24 34 41 58 62 64 67 69 78
	for (int i = 0;i < 10;i++) {
		set1.insert(rand() % 100);
	}
	for (int val : set1) {
		cout << val << " ";
	}
	cout << endl;

	cout << "=====================" << endl;
	/*set底层
	template <class _Kty, class _Pr = less<_Kty>, class _Alloc = allocator<_Kty>>
	*/
	//95 91 81 61 45 42 36 27 5
	set<int, greater<int>> set2;
	for (int i = 0; i < 10; i++) {
		set2.insert(rand() % 100);
	}
	for (int val : set2) {
		cout << val << " ";
	}
	cout << endl;

	return 0;
}