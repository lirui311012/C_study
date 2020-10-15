#include <iostream>
using namespace std;

class Queue {
public:


	Queue(int size = 5) {
		_pQue = new int[size];
		_front = _rear = 0;
		_size = size;
	}
	~Queue() {
		delete[]_pQue;
		_pQue = nullptr;
	}

	//队尾入队
	void push(int val) {
		if (full()) {
			resize();
		}
		_pQue[_rear] = val;
		_rear = (_rear + 1) % _size;
	}
	void pop() {
		if (empty()) {
			return;
		}
		_front = (_front + 1) % _size;
	}
	int front() {
		return _pQue[_front];
	}
	bool full() {
		return (_rear + 1) % _size == _front;
	}
	bool empty() {
		return _front == _rear;
	}
	//删除拷贝构造和等号操作符
	//Queue(const Queue& other) = delete;
	//Queue& operator=(const Queue &other) = delete;
	Queue(const Queue& other) {
		_front = other._front;
		_rear = other._rear;
		_size = other._size;
		_pQue = new int[_size];
		for (int i = _front; i != _rear; i = (i + 1) % _size) {
			_pQue[i] = other._pQue[i];
		}
	}
	Queue& operator=(const Queue& other) {
		if (this == &other) {
			return *this;
		}
		delete[]_pQue;
		_front = other._front;
		_rear = other._rear;
		_size = other._size;
		_pQue = new int[_size];
		for (int i = _front; i != _rear; i = (i + 1) % _size) {
			_pQue[i] = other._pQue[i];
		}
		return *this;
	}
private:
	int* _pQue;
	int _front;
	int _rear;
	int _size;
	void resize() {
		int* ptmp = new int[2 * _size];
		int index = 0;
		for (int i = _front; i != _rear; i = (i + 1) % _size) {
			ptmp[index++] = _pQue[i];
		}
		delete[]_pQue;
		_pQue = ptmp;
		_front = 0;
		_rear = index;
		_size *= 2;
	}

};

int main() {
	Queue queue;
	for (int i = 0; i < 20; i++) {
		queue.push(rand() % 100);
	}
	while (!queue.empty()) {
		cout << queue.front() << " ";
		queue.pop();
	}

	cout << endl;
	Queue queue2 = queue;
	return 0;
}





#if 0
class String {
public:
	String(const char* str = nullptr) {
		if (str != nullptr) {
			int len = strlen(str);
			this->m_data = new char[len + 1];
			strcpy_s(this->m_data, len + 1, str);
		}
		else {
			this->m_data = new char[1];
			*m_data = '\0';
		}
	}
	String(const String& other) {
		m_data = new char[strlen(other.m_data) + 1];
		strcpy_s(this->m_data, strlen(other.m_data) + 1, other.m_data);
	}
	~String(void) {
		delete[]m_data;
		m_data = nullptr;
	}
	String& operator=(const String& other) {
		if (this == &other) {
			return *this;
		}

		delete[]m_data;

		m_data = new char[strlen(other.m_data) + 1];
		strcpy_s(m_data, strlen(other.m_data) + 1, other.m_data);
		return *this;
	}

private:
	char* m_data;
};

int main() {
	String str1;
	String str2("hello");
	String str3("world");

	String str4 = str3;
	String str5(str3);

	str3 = str1 = str2;
	return 0;
}
#endif