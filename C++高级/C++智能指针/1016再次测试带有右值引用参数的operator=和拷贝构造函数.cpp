#include <iostream>
using namespace std;

class A {
public:
	A() { cout << "A()" << endl; }
	A(const A&) { cout << "A(const A&)" << endl; }
	A(const A&&) { cout << "A(const A&&)" << endl; }
	A& operator=(const A&) { cout << "operator=(const A&)" << endl;  return *this; }
	A& operator=(A&&) { cout << "operator=( A&&)" << endl;  return *this; }
	~A() { cout << "~A()" << endl; }
};
/////////////////////////////////////////////


#if 0
A GetObject(A& a1) {
	A tmp(a1);
	return tmp;
}
int main()
{
	A a1;
	A a2 = GetObject(a1);

	return 0;
}
/*
A()
A(const A&)
A(const A&&)
~A()
~A()
~A()
*/
#endif
/////////////////////////////////////

#if 0
A GetObject(A& a1) {
	A tmp(a1);
	return tmp;
}

int main()
{
	A a1;
	A a2;
	a2 = GetObject(a1);

	return 0;
}
/*
A()
A()
A(const A&)
A(const A&&)
~A()
operator=( A&&)
~A()
~A()
~A()
*/
#endif

/////////////////////////////////////////////
#if 0
A GetObject(A& a1) {
	return A(a1);
}

int main()
{
	A a1;
	A a2 = GetObject(a1);

	return 0;
}
/*
A()
A(const A&)
~A()
~A()
*/
#endif

/////////////////////////////////////
#if 1
A GetObject(A& a1) {
	return A(a1);
}

int main()
{
	A a1;
	A a2;
	a2 = GetObject(a1);

	return 0;
}
/*
A()
A()
A(const A&)
operator=( A&&)
~A()
~A()
~A()
*/
#endif
