#include <iostream>
using namespace std;


class test {
public:
	test(int a):ma(a) { cout << "test(int )" << endl; }
	test(const test& t) { cout << "test(const test &)" << endl; }
	test( test&& t) { cout << "test( test &&)" << endl; }
	test& operator=(const test& t) { 
		cout << "operator=(const test &)" << endl; 
		if (this == &t) return *this;
		this->ma = t.ma;
		return *this;
	}
	test& operator=( test&& t) { 
		cout << "operator=( test &&)" << endl;
		if (this == &t) return *this;
		this->ma = t.ma;
		return *this;
	}
	~test() { cout << "~test()" << endl; }

	int getA() {
		return ma;
	}
private:
	int ma;
};

test get(test &t) {
	int val = t.getA();
	//test res(val);
	//return res;
	return test(val);
}

int main() {
	test t1(1);
	test t2(2);
	t2 = get(t1);
	return 0;
}
/*
test(int )
test(int )
test(int )
test( test &&)
~test()
operator=( test &&)
~test()
~test()
~test()
*/

/*
test(int )
test(int )
test(int )
operator=( test &&)
~test()
~test()
~test()
*/