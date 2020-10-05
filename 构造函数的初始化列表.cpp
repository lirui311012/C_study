//#include <iostream>
//
//using namespace std;
//
//#if 0
//class Test {
//public:
//	Test(int data = 10) :mb(data), ma(mb) {}
//	void show() {
//		cout << "ma:" << ma << " mb:" << mb << endl;
//	}
//private:
//	int ma;
//	int mb;
//};
//int main() {
//
//	Test t;
//	t.show();
//	return 0;
//}
//#endif
//
//#if 1
////构造函数初始化列表
//
//class CDate {
//public:
//	CDate(int y, int m, int d) {
//		_year = y;
//		_month = m;
//		_day = d;
//	}
//	void show() {
//		cout << _year << "/" << _month << "/" << _day << endl;
//	}
//	void show() const{
//		cout << _year << "/" << _month << "/" << _day << endl;
//	}
//private:
//	int _year;
//	int _month;
//	int _day;
//};
//
//
//class CGoods {
//public:
//	//初始化列表
//	CGoods(const char* n, int a, double p, int y, int m, int d)
//		:_date(y, m, d) {
//		strcpy_s(_name, 20, n);
//		_amount = a;
//		_price = p;
//		count++;
//	}
//	void show() {
//		cout << "name:" << _name << endl;
//		cout << "amount:" << _amount << endl;
//		cout << "price:" << _price << endl;
//		_date.show();
//	}
//	void show() const{
//		cout << "name:" << _name << endl;
//		cout << "amount:" << _amount << endl;
//		cout << "price:" << _price << endl;
//		_date.show();
//	}
//	static void showCount() {
//		cout << "count = " << count << endl;
//	}
//private:
//	char _name[20];
//	int _amount;
//	double _price;
//	CDate _date;  //组合，成员对象
//	static int count;
//};
//int CGoods::count = 0;
//
//int main()
//{
//	CGoods goods1("iPhone", 100, 2000.56, 2020, 10, 1);
//	goods1.show();
//	CGoods goods2("iPhone", 100, 2000.56, 2020, 10, 1);
//	goods2.show();
//	CGoods goods3("iPhone", 100, 2000.56, 2020, 10, 1);
//	goods3.show();
//	CGoods goods4("iPhone", 100, 2000.56, 2020, 10, 1);
//	goods4.show();
//
//	cout << "================" << endl;
//	const CGoods m("AAA",1,555555,2000,10,5);
//	m.show();
//
//	CGoods::showCount();
//
//	return 0;
//}
//
//#endif