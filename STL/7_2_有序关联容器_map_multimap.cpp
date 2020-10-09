#include <iostream>
#include <map>

using namespace std;

//Student需要提供默认构造函数
//比如map<int,Student> map1;
//map1[20000000];不存在的时候默认插入一个元素{2000000,Student()}所以需要默认构造函数
//但是Student不需要提供operator<函数重载了，因为是以key进行排序，key是int，编译器自己会排序

//有序映射表map
class Student {
public:
	//() {}
	Student(int id = 0, string name = "")  //来一个默认构造函数
		:_id(id), _name(name) {}
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
	map<int, Student> stuMap;
	stuMap.insert({ 1000, Student(1000, "张雯") });
	stuMap.insert({ 1020, Student(1020, "李广") });
	stuMap.insert({ 1030, Student(1030, "高洋") });
	//stuMap.erase(it);  stuMap.erase(1020)

	//stuMap[1020]的副作用就是，当该key不存在的时候，会插入一个默认的元素
	cout << stuMap[1020] << endl;

	cout << "========================" << endl;
	//使用迭代器对map进行统一的遍历
	auto it = stuMap.begin();
	while (it != stuMap.end()) {
		cout << "id:" << it->first <<" name:"<<it->second << endl;
		++it;
	}

	return 0;
}