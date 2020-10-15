#include <iostream>
#include <vector>
using namespace std;


int main()
{

	int arr[] = {1,2,3,4,5,6,7,8,9};
	const int len = sizeof(arr) / sizeof(arr[0]);
	vector<int> vec(arr,arr+len);


	int* first = arr;
	int* end = arr + len;
	cout << "len = " << len << endl;
	cout << "first = " << first << endl;
	cout << "end = " << end << endl;
	cout << "end - first = " << end - first << endl;


	return 0;
}