//#include <iostream>
//#include <set>
//#include <map>
//#include <unordered_map>
//#include <unordered_set>
//using namespace std;

////该文章简述了unordered_set和unordered_multiset的使用
//
//
//int main() {
//
//
//#if 1
//  //unordered_multiset<int> set1;  //后面的set1.size()一定打印50，因为允许元素key重复
//	unordered_set<int> set1; // 不会存储key值重复的元素
//	for (int i = 0; i < 50; ++i)
//	{
//		set1.insert(rand() % 20 + 1); // vector/deque/list  insert(it, val)
//	}
//
//	//cout << set1.size() << endl;   //打印的时候发现size小于50，因为有重复的，而uordered_set单重无序集合，不允许重复
//	//cout << set1.count(15) << endl; //返回key为15的元素个数，unordered_set是单重集合，所以key为15若存在则为1，不存在则为0
//
//	
//	auto it1 = set1.begin();
//	for (; it1 != set1.end(); ++it1)
//	{
//		cout << *it1 << " ";
//	}
//	cout << endl;
//
//	set1.erase(20); // 按key值删除元素
//
//	for (it1 = set1.begin(); it1 != set1.end(); )
//	{
//		if (*it1 == 30)
//		{
//			it1 = set1.erase(it1); // 调用erase，it1迭代器就失效了
//		}
//		else
//		{
//			++it1;
//		}
//	}
//
//	it1 = set1.find(20);
//	if (it1 != set1.end())
//	{
//		set1.erase(it1);
//	}
//
//	//使用c++11提供的foreach遍历容器，底层依赖迭代器
//	for (int v : set1)
//	{
//		cout << v << " ";
//	}
//	cout << endl;
//#endif
//
//	return 0;
//}