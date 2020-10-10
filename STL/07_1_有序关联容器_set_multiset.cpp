#include <iostream>
#include <set>
#include <string>
using namespace std;

//往set集合里面存放我们的自定义类型class Student

//单重集合set和多重集合multiset的区别就是
//multiset可以存储重复的元素
class Student {
public:
	Student(int id, string name)
		:_id(id), _name(name) {}
	bool operator<(const Student &stu) const{
		return _id < stu._id;
	}
private:
	int _id;
	string _name;
	friend ostream& operator<<(ostream& out, const Student& stu);
};

ostream& operator<<(ostream &out , const Student &stu) {
	out << "id:" << stu._id << " name:" << stu._name;
	return out;
}
int main() {
	//问题来了，set和map底层实现都是红黑树，
	//那么将我们的自定义类型class Student放进去，红黑树如何比较大小？如何排序？
	//编译器默认一定是不知道如何对Student对象进行排序
	//所以自定义类型在使用有序容器的时候要提供operator<的重载函数
	set<Student> set1;
	set1.insert(Student(1020, "李广"));
	set1.insert(Student(1000, "张雯"));
	
	//通过迭代器遍历set集合
	auto it = set1.begin();
	while (it != set1.end()) {
		cout << *it << endl;  //Student重载了 operator<<函数
		++it;
	}
	return 0;
}


#if 0
//和unordered_set使用方法一样
int main() {
	set<int> set1;
	for (int i = 0;i < 20;i++) {
		set1.insert(rand() % 20 +1);
	}

	//foreach底层原理是迭代器
	//所以当使用迭代器，遍历set的时候，底层实现是对红黑树的中序遍历的结果
	for (const int val : set1) {
		cout << val << " ";
	}
	cout << endl;

	//打印结果12个元素少于20
	//set单重有序集合，单重表示key不可以重复，底层实现是红黑树
	//有序可以从打印结果看出在递增
	//1 2 3 5 6 8 10 12 15 16 17 19


	return 0;
}
#endif