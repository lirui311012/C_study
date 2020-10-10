#include <iostream>
#include <vector>
using namespace std;

//vector容器什么时候迭代器失效，如何解决迭代器失效问题？
//insert和erase函数的返回值是当前位置的新的迭代器，所以使用中注意逻辑合理性
/*
迭代器的失效问题？
1.迭代器为什么会失效？
a：当容器调用erase方法后，当前位置到容器末尾元素的所有的迭代器全部失效了
b：当容器调用insert方法后，当前位置到容器末尾元素的所有的迭代器全部失效了
	 迭代器依然有效    迭代器全部失效
首元素   ->  插入点/删除点  ->  末尾元素

c：insert来说，如果引起容器内存扩容
   原来容器的所有的迭代器就全部失效了
首元素   ->  插入点/删除点  ->  末尾元素

d：不同容器的迭代器是不能进行比较运算的

2.迭代器失效了以后，问题该怎么解决？
对插入/删除点的迭代器进行更新操作




*/
void show(const vector<int>& vec) {
	for (int val : vec) {
		cout << val << " ";
	}
	cout << endl;
}


int main() {
	vector<int> vec;
	for (int i = 0; i < 20; ++i) {
		vec.push_back(rand() % 100 + 1);
	}
	
	show(vec);

#if 0
	//场景2：给vec容器中所有的偶数前面添加一个小于该偶数值1的数字
	auto it = vec.begin();
	for (; it != vec.end() ;++it) {
		if (*it % 2 == 0) {
			//这里的迭代器在第一次insert之后，iterator就失效啦
			vec.insert(it,*it-1);
			//break;
			++it;   //意思就是+1两次
		}
	}
	show(vec);

#endif


#if 0
	//场景1：把vec中所有的偶数全部删除
	auto it = vec.begin();
	while(it != vec.end()) {
		if (*it % 2 == 0) {
			//迭代器失效的问题,第一次调用erase后，迭代器it就失效啦
			it = vec.erase(it);
			//break;
		}
		else {
			++it;
		}
	}
	show(vec);
#endif

	return 0;
}