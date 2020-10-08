#include <iostream>
using namespace std;

//STL , standard template library 标准模板库

/*
C++ STL standard template libaray 标准模板库
一、标准容器 C++11还提供了 array(数组容器)  forward_list(单向链表容器)
1.顺序容器
vector
deque
list
2.容器适配器
stack
queue
priority_queue
3.关联容器
无序关联容器 => 链式哈希表  增删查趋近O(1)
set:集合 key   map:映射表 [key,value]
unordered_set 单重集合
unordered_multiset 多重集合
unordered_map 单重映射表
unordered_multimap 多重映射表

有序关联容器 => 红黑树 增删查O(log2n) 2是底数(树的层数，树的高度)
set
multiset
map
multimap

二、近容器
数组，string，bitset(位容器)

三、迭代器
iterator和const_iterator  //普通迭代器和常量迭代器
reverse_iterator和const_reverse_iterator  //反向迭代器

四、函数对象（类似C的函数指针）
greater,less

五、泛型算法 大约70~80个算法
sort,find,find_if,binary_search,for_each
*/

/*
vector：向量容器
底层数据结构：动态开辟的数组，每次以原来空间大小的2倍进行扩容的

vector<int> vec;
增加:
vec.push_back(20); 末尾添加元素 O(1)   导致容器扩容
vec.insert(it, 20); it迭代器指向的位置添加一个元素20  O(n)   导致容器扩容

删除:
vec.pop_back(); 末尾删除元素 O(1)
vec.erase(it); 删除it迭代器指向的元素 O(n)

查询:
operator[] 下标的随机访问vec[5]  O(1)
iterator迭代器进行遍历
find，for_each
foreach => 通过iterator来实现的

注意：对容器进行连续插入或者删除操作(insert/erase)，一定要更新迭代器，否则第一次insert或者erase完成，
迭代器就失效了

常用方法介绍:
size()
empty()
reserve(20)：vector预留空间的   只给容器底层开辟指定大小的内存空间，并不会添加新的元素
resize(20)：容器扩容用的  不仅给容器底层开辟指定大小的内存空间，还会添加新的元素
swap ： 两个容器进行元素交换

*/
