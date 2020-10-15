#include <iostream>
using namespace std;

//运算符重载
//template <typename T>
//T sum(T a,T b) {
//	return a + b;  //a.operator+(b);
//}

//复数类
class CComplex {
public:
	CComplex(int r = 0, int i = 0)
		:mreal(r), mimage(i) {}

	//知道编译器怎么做CComplex类对象的加法操作
	//CComplex operator+(const CComplex& src) {
	//	cout << "CComplex::CComplex operator+(const CComplex& src)" << endl;
	//	//CComplex comp;
	//	//comp.mreal = this->mreal + src.mreal;
	//	//comp.mimage = this->mimage + src.mimage;
	//	//return comp;

	//	return CComplex(this->mreal + src.mreal, this->mimage + src.mimage);
	//}

	void show() {
		cout << "mreal:" << mreal << " " << "mimage:" << mimage << endl;
	}

	//右++
	CComplex operator++(int) {
		cout << "右++" << endl;
		//CComplex temp = *this;
		//this->mimage += 1;
		//this->mreal += 1;
		//return temp;

		return CComplex(mreal++,mimage++); // 对象优化
	}

	//左++
	CComplex& operator++() {
		cout << "左++" << endl;
		this->mimage += 1;
		this->mreal += 1;
		return *this;
	}
	void operator+=(const CComplex& com) {
		mreal += com.mreal;
		mimage += com.mimage;
	}
private:
	int mreal;
	int mimage;
	friend std::istream& operator>>(std::istream& is, CComplex& com);
	friend ostream& operator<<(std::ostream& out, const CComplex& com);
	friend CComplex operator+(const CComplex& lhs, const CComplex& rhs);
};

//全局加法运算符重载
//全局这个既可以对象+整数，整数+对象，对象+对象
//全局的方法不需要使用对象去调用
CComplex operator+(const CComplex &lhs,const CComplex &rhs) {
	cout << "CComplex operator+(const CComplex &lhs,const CComplex &rhs)" << endl;
	return CComplex(lhs.mreal+rhs.mreal,lhs.mimage+rhs.mimage);
}

//cout << comp1 << endl;
//第一个参数前面不可以加const,因为流是不断变化的
ostream& operator<<(std::ostream & out,const CComplex &com) {
	//参数的out其实就是cout的一个别名
	out << "mreal:" << com.mreal << " mimage:" << com.mimage;
	return out;
}

//istream,ostream输入输出流的对象不要用const 修饰
istream& operator>>(std::istream& in,CComplex& com) {
	in >> com.mreal >> com.mimage;
	return in;
}

template <typename T>
void show(T a);

int main()
{
	CComplex comp1(10, 10);
	CComplex comp2(20, 20);
#if 0
	//               comp1.operator(comp2); //加法运算符的重载函数
	CComplex comp3 = comp1 + comp2; // comp1+comp2
	comp1.show();
	comp2.show();
	comp3.show();

	cout << "---------------" << endl;
	CComplex comp4 = comp1 + 20;
	comp1.show();
	comp4.show();
	cout << "---------------" << endl;

	//编译器做对象运算的时候，会调用对象的运算符重载函数(优先调用成员方法)，如果没有成员方法
	//，就在全局作用域下找合适的运算符重载函数
#endif
	CComplex comp5 = 30 + comp1;
	//comp5.show();


	//operator++() 左++
	//operator++(int) 右++
	//comp5 = comp1++;
	//comp1.show();
	//comp5.show();
	//comp5 = ++comp1;
	//comp1.show();
	//comp5.show();

	comp1 += comp2;
	//comp1.show();
	//comp2.show();  //对象信息的输出   cout << comp2 << endl;

	//无法提供成成员方法，所以只能提供成全局方法,全局方法不需要使用对象调用

	cout << "---------------------" << endl;
	cout << comp1 << " "<<comp2 << endl;
	cout << "----------------------" << endl;
	show<CComplex>(comp1);
	show<CComplex>(comp2);
	cout << "=================================" << endl;
	cout << "=================================" << endl;
	cout << "=================================" << endl;
	CComplex comp666;
	CComplex comp777;
	cin >> comp666 >> comp777;
	cout << comp666 << " " << comp777 << endl;
	return 0;
}

template <typename T>
void show(T a) {
	cout << a << endl;
}

