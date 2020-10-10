#include <iostream>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
using namespace std;

//unordered_map，unordered_multimap

//unoedered_map属于哈希表，增删查效率特别好O(1)

#if 0
int main() {
	/*
	[key, value]
	struct pair
	{
	 first; => key
	 second; => value
	}

	map的operator[]的副作用--》
	map的operator[]不仅仅有查询的功能！！！还有添加，修改的功能！
	1.查询
	2.如果key不存在，它会插入一对数据[key, string()]
	V& operator[](const K &key)
	{
		insert({key, V()});
	}
	*/

	
	unordered_map<int, string> map1;
	map1.insert(make_pair(1000, "张三"));   //make_pair可以直接将我们的key_value打包成一个pair对象
	map1.insert({1010, "李四"}); // map表增加元素
	map1.insert({1020, "王五" });  //C++11

	cout << map1.size() << endl;   //3
	map1.insert({1010, "王凯" });   //插入失败
	cout << map1.size() << endl;   //3 ,unordered_map单重集合不允许key重复
	cout << map1[1010] << endl;   //李四
	map1.erase(1020); // {1020, "王五" }删除了  //传入key值


	auto it1 = map1.find(1030);
	//如果查询到了则返回该键值对的迭代器，如果没有查询到则返回end()迭代器
	//迭代器指向的是pair对象，所以访问键值对需要访问pair对象里面的first(key),second(value)
	//(*ite).first, (*ite).second
	//迭代器提供了->的重载
	if (it1 != map1.end())
	{
		// it1 -> pair对象
		cout << "key:" << it1->first << " value:" << it1->second << endl;
	}

	//map1[2000]; // key:2000 value:""
	//map1[2000] = "刘硕"; // map1.insert({2000, "刘硕"});  //operator[]如果键值不存在则添加，如果存在则修改
	//map1[1000] = "张三2"; //由于键值1000已经存在那么就是一个修改操作
	//cout << map1.size() << endl;
	// map operator[](key) => value  查询
	//cout << map1[1000] << endl;
	


	return 0;
}
#endif

#if 0
//什么时候使用哈希表unordered_map

//例子1：海量数据查重
int main() {

	// 处理海量数据查重复；去重复的时候 ，经常使用哈希表
	//举例：
	const int ARR_LEN = 100000;
	int arr[ARR_LEN] = {0};
	for (int i = 0;i < ARR_LEN;i++) {
		arr[i] = rand() % 100 + 1;
	}
	//在上面10w个整数中，统计哪些数字重复了，并且统计数字重复的次数
	//当设计查重复，去重复的时候，一定要想到哈希表这个存储结构
	unordered_map<int, int> map1;  //key代表数字，value代表出现的次数
	for (int k : arr) {
#if 0
		auto it = map1.find(k);
		if (it == map1.end()) {
			//该数字以前没有出现过，插入哈希表中，此时出现一次
			map1.insert({k,1});
		}
		else {
			//以前出现过，那么再以前的次数基础上+1即可
			it->second++;
		}
#endif

		//上面这段代码完全可以使用下面这行代码代替
		map1[k]++;  //存在直接value++,不存在那么添加键值对默认value == 0 然后++变成1
					//{key,int()}
	}

	//foreach遍历unordered_map容器
	//for (const pair<int,int> &p : map1) {  //定义成引用，就不会出现对象拷贝构造的调用
	//	if (p.second > 1) {
	//		cout << "key:" << p.first << " value:" << p.second << endl;
	//	}
	//}
	//cout << "==========" << endl;

	//使用迭代器遍历unordered_map
	auto it = map1.begin();
	while (it != map1.end()) {
		if (it->second > 1) {
			cout << "key:" << it->first << " value:" << it->second << endl;
		}
		++it;
	}


	return 0;
}
#endif


//海量数据去重
int main() {
	// 处理海量数据查重复；去重复的时候 ，经常使用哈希表
	//举例：
	const int ARR_LEN = 100;
	int arr[ARR_LEN] = { 0 };
	for (int i = 0; i < ARR_LEN; i++) {
		arr[i] = rand() % 20 + 1;
	}
	//在上面的10w个整数中，进行去重打印，就是输哦重复的数字打印一次就行
	//思考：可以使用哈希表，那么哈希表有unordered_map和unordered_set，我们选择的谁？
	//unordered_set
	unordered_set<int> set1;
	for (int k:arr) {
		set1.insert(k);
	}
	for (const int &val : set1) {
		cout << val << " ";
	}
	cout << endl;

	return 0;
}