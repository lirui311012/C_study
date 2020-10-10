#include <iostream>
#include <vector>
using namespace std;

/*
无论是顺序容器，还是关联容器，都有迭代器，并且都有反向迭代器噢噢噢噢
*/


/*容器的迭代器
iterator
const_iterator
reverse_iterator
const_reverse_iterator
*/


/*
正向迭代器：
const_iterator:常量的正向迭代器，只能读，不能写
iterator:普通的正向迭代器,既可以读元素的值，又可以修改元素的值,iterator继承const_iterator
如：
vector<int>::const_iterator it1 = vec.begin();
vector<int>::iterator it2 = vec.begin();
*/

/*
反向迭代器

reverse_iterator：普通反向迭代器，可以读取可以修改
const_reverse_iterator:常量的反向迭代器，可以读取不能修改

如：
vector<int>::reverse_iterator it3 = vec.rbegin();
vector<int>::const_reserve_iterator it4 = vec.rbegin();

*/

int main()
{
	vector<int> vec;
	for (int i = 0;i < 20;i++) {
		vec.push_back(rand() % 100);
	}
	vector<int>::iterator it = vec.begin();  //普通的正向迭代器
	/*
	vector<int>::const_iterator it = vec.begin();
	问：为什么iterator对象可以给const_iterator赋值？
	原因：class const_iterator{
		public:
			const T& operator*(){
				return *_ptr;
			}
	}
	class iterator:public const_iterator{
		public:
			T& operator*(){
				return *_ptr;
			}
	}
	*/
#if 0
	while (it != vec.end()) {
		cout << *it << " ";
		if (*it % 2 == 0) {
			*it = 0;
		}
		++it;
	}
	cout << endl;
#endif


	//41 67 34 0 69 24 78 58 62 64 5 45 81 27 61 91 95 42 27 36
	for (int val : vec) {
		cout << val << " ";
	}
	cout << endl;

	cout << "======================" << endl;
	//rbegin():返回的是最后一个元素的反向迭代器的表示
	//rend():返回的是首元素前驱位置的迭代器的表示
	//vector<int>::reverse_iterator it3 = vec.rbegin();
	auto it3 = vec.rbegin();
	while (it3 != vec.rend()) {
		cout << *it3 << " ";
		++it3;
	}
	cout << endl;
	//36 27 42 95 91 61 27 81 45 5 64 62 58 78 24 69 0 34 67 41

	return 0;
}