#include <iostream>
using namespace std;
/*
运算符的重载：类得成员方法，全局方法
之前的深入理解new和delete里面是通用的是全局的运算符重载
所以再整个项目中所有的new和delete都被截获，使用自己的new和delete


池：
内存池，线程池，进程池，连接池，对象池等
*/


//实现一个链式队列,队尾入队，对头出队
template<typename T>
class Queue {
public:
	Queue() {
		_front = _rear = new QueueItem();
	}
	~Queue() {
		QueueItem* cur = _front;
		while (cur != nullptr) {
			_front = _front->_next;
			delete cur;
			cur = _front;
		}
	}
	void push(const T &val) {
		//队尾入队
		QueueItem* item = new QueueItem(val);
		_rear->_next = item;
		_rear = item;
	}
	void pop() {
		//头出队
		if (empty())
			return;
		QueueItem* first = _front->_next;
		_front->_next = first->_next;
		if (_front->_next == nullptr) {
			_rear = _front;
		}
		delete first;
	}
	T front()const {
		return _front->_next->_data;
	}
	bool empty()const {
		return _front == _rear;
	}
private:
	//产生一个QueueItem的对象池(10000个QueueItem节点)
	//节点
	struct QueueItem {
		QueueItem(T data = T()):_data(data),_next(nullptr) {}

		//给QueueItem提供自定义内存管理
		//operator new和operator delete编译器自动处理成static
		void* operator new(size_t size) {
			if (_itemPool == nullptr) {
				_itemPool = (QueueItem*)new char[POOL_ITEM_SIZE*sizeof(QueueItem)];
				QueueItem* p = _itemPool;
				for (;p < _itemPool+POOL_ITEM_SIZE-1;++p) {
					p->_next = p + 1;
				}
				p->_next = nullptr;
			}
			QueueItem* res = _itemPool;
			_itemPool = _itemPool->_next;
			return res;
		}
		void operator delete(void *ptr) {
			QueueItem* p = (QueueItem*)ptr;
			p->_next = _itemPool;
			_itemPool = p;
		}
		T _data;
		QueueItem* _next;
		//新的语言标准里面静态常量可以类内直接初始化
		static const int POOL_ITEM_SIZE = 10000;
		static QueueItem* _itemPool;
	};
	QueueItem* _front; //指向头节点
	QueueItem* _rear;  //指向队尾
};

//必须加上typename，在此处的作用是告诉编译器Queue<T>作用域下的QueueItem是一个类型
template<typename T>
typename Queue<T>::QueueItem* Queue<T>::QueueItem::_itemPool = nullptr;

int main()
{
	Queue<int> que;
	for (int i = 0;i < 1000000;i++) {
		que.push(i);
		que.pop();
	}
	cout << que.empty() << endl;

	return 0;
}