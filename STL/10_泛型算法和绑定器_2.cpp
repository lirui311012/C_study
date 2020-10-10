#include <algorithm> // 包含了C++ STL里面的泛型算法
#include <functional> // 包含了函数对象和绑定器
#include <iostream>
#include <vector>
using namespace std;

/*
熟悉更多的泛型算法，可以提高代码开发效率！！
*/

//sort   //快速排序
//binary_search  //二分查找
//find   //查找元素
//find_if  //按照条件进行查询

//绑定器 + 二元函数对象 =》 一元函数对象
//bind1st：把二元函数对象的operator()(a, b)的第一个形参绑定起来
//bind2nd：把二元函数对象的operator()(a, b)的第二个形参绑定起来

//泛型算法
int main() {
	int arr[] = {12,4,78,9,21,43,56,52,42,31};
	vector<int> vec(arr,arr+sizeof(arr)/sizeof(arr[0]));
	for (int val : vec) {
		cout << val << " ";
	}
	cout << endl;
	//12 4 78 9 21 43 56 52 42 31
	cout << "===============" <<endl;

	//sort排序
	//默认升序
	sort(vec.begin(),vec.end());
	for (int val : vec) {
		cout << val << " ";
	}
	cout << endl;
	//4 9 12 21 31 42 43 52 56 78


	//binary_search有序容器中进行二分查找
	//时间复杂度O(logn)
	auto flag = binary_search(vec.begin(),vec.end(),21);
	if (flag) {
		cout << "21存在-binary_search" << endl;
	}
	else {
		cout << "21不存在-binary_search" << endl;
	}

	//find
	//时间复杂度计算O(n)
	auto it = find(vec.begin(),vec.end(),21);
	if (it != vec.end()) {
		cout << "21存在-find" << endl;
	}
	else {
		cout << "21不存在-find" << endl;
	}
	//find和binary_search那指定是二分查找效率高，二分查找用于元素有序时候噢噢噢噢


	//降序排序
	//传入函数对象greater,改变容器元素排序时的比较方式
	//底层实现，快速排序算法
	sort(vec.begin(),vec.end(),greater<int>());  //传入函数对象
	for (int val : vec) {
		cout << val << " ";
	}
	cout << endl;
	//78 56 52 43 42 31 21 12 9 4


	//find_if
	//当前vec容器元素
	//78 56 52 43 42 31 21 12 9 4是一个降序
	//需求，把48按序插入到vector容器当中，需要找到第一个小于48的数字
	//find_if需要的是一个一元函数对象
	//而库里面提供的greater,less都是二元函数对象，怎么办?
	//c++11提供了绑定器
	//绑定器 + 二元函数对象 = 》 一元函数对象
	//greater a > b
	//less a < b
	//我们现在需要找第一个小于48的
	//如果使用greater那么我们需要把a绑定成48，也就是使用bind1st  
	//auto it2 = find_if(vec.begin(),vec.end(),bind1st(greater<int>(),48));  //可以

	//如果我想用less，那么就把b绑定成48，也就是使用bind2nd
	//auto it2 = find_if(vec.begin(), vec.end(), bind2nd(less<int>(), 48));

	//也可以使用lambda表达式
	auto it2 = find_if(vec.begin(), vec.end(), [](int val)->bool {return val < 48; });
	//上面的这个lambda表达式参数只有一个，所以就是一元函数对象，lambda就是函数对象

	//还有一个方式就是，自己写一段c++代码进行上述场景的实现

	//找到了，find_if返回迭代器
	//找不到返回end(),那就往end()位置插入就行
	vec.insert(it2,48);
	for (int val : vec) {
		cout << val << " ";
	}
	cout << endl;
	//78 56 52 48 43 42 31 21 12 9 4

	cout << "======================" << endl;


	//for_each
	//把容器中所有的偶数都打印出来，查看for_each源码可以看到，需要一个一元函数对象
	//for_each可以自行遍历容器的所有元素，可以自行添加合适的函数对象
	//对容器的元素进行过滤
	for_each(vec.begin(), vec.end(), 
		[](int val)->void 
	{
		if (val % 2 == 0) 
		{
			cout << val << " ";
		}
	});
	cout << endl;
	//78 56 52 48 42 12 4
	
	return 0;
}
/*
lambda表达式就是函数对象：
最大的好处就是：
比如在使用for_each的时候传入函数对象，使用lambda就不许写一个class然后重载里面的operator()函数
如果用到大量的函数对象，那么lambda的优势就可以显而易见了
*/