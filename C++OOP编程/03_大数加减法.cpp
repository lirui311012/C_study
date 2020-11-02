#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// 编程题目：请实现以#下类的方法，完成大数的加减法
class BigInt
{
public:
	BigInt(string str) :strDigit(str) {}
private:
	string strDigit;   // 使用字符串存储大整数

	friend ostream& operator<<(ostream& out, const BigInt& src);
	friend BigInt operator+(const BigInt& lhs, const BigInt& rhs);
	friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);
};
// 打印函数
ostream& operator<<(ostream& out, const BigInt& src)
{
	out << src.strDigit;
	return out;
}
// 大数加法
BigInt operator+(const BigInt& lhs, const BigInt& rhs)
{
	/*
	遍历字符串l，r，从后往前遍历
	同位置的数字相加， 进位 flag  存入一个结果当中 string result
	同时完成
	某个字符串先完成   都要考虑进位
	*/
	string result;
	bool flag = false;  //记录相对位置相加的时候需不需要考虑进位，进位(true)多加1
	int i = lhs.strDigit.length() - 1;
	int j = rhs.strDigit.length() - 1;

	for (; i >= 0 && j >= 0; --i, --j)
	{
		//得到字符'0'~'9'对应的整数  <字符 - '0'>
		int ret = lhs.strDigit[i] - '0' + rhs.strDigit[j] - '0';
		if (flag)
		{
			ret += 1;
			flag = false;
		}

		if (ret >= 10)
		{
			ret %= 10;
			flag = true;
		}
		result.push_back(ret + '0');
	}
	//for循环结束，可能一起结束，可能有一个有剩余，进行判断即可

	// i j
	if (i >= 0)  //第一个字符串还没结尾
	{
		while (i >= 0)
		{
			int ret = lhs.strDigit[i] - '0';
			if (flag)
			{
				ret += 1;
				flag = false;
			}

			if (ret >= 10)
			{
				ret %= 10;
				flag = true;
			}
			result.push_back(ret + '0');
			i--;
		}
	}
	else if (j >= 0)  //第二个字符串还没结尾
	{
		while (j >= 0)
		{
			int ret = rhs.strDigit[j] - '0';
			if (flag)
			{
				ret += 1;
				flag = false;
			}

			if (ret >= 10)
			{
				ret %= 10;
				flag = true;
			}
			result.push_back(ret + '0');
			j--;
		}
	}

	if (flag)
	{
		result.push_back('1');
	}

	//字符串反转，之前是push_back进去的,所以需要反转一下，泛型算法reverse元素逆置
	reverse(result.begin(), result.end());
	return result; // return BigInt(result);
}
// 大数减法
BigInt operator-(const BigInt& lhs, const BigInt& rhs)
{
	/*
	找大的字符串左减数，小的左被减数
	遍历两个字符串，减法，借位（bool flag）， string result 存下来
	*/
	string result;
	bool flag = false;  //是否借位
	bool minor = false;  //最终结果会否需要输出负号(-)

	string maxStr = lhs.strDigit;
	string minStr = rhs.strDigit;
	if (maxStr.length() < minStr.length())
	{
		maxStr = rhs.strDigit;
		minStr = lhs.strDigit;
		minor = true;
	}
	else if (maxStr.length() == minStr.length())
	{
		//两字符串长度相等，那就直接比较字符串大小就可以
		if (maxStr < minStr)
		{
			maxStr = rhs.strDigit;
			minStr = lhs.strDigit;
			minor = true;
		}
		else if (maxStr == minStr)
		{
			//长度相等，并且字符串相等，那么相减就是0
			return string("0");
		}
	}
	else
	{
		;
	}

	int i = maxStr.length() - 1;
	int j = minStr.length() - 1;

	for (; i >= 0 && j >= 0; --i, --j)
	{
		int ret = maxStr[i] - minStr[j];
		//flag->true表示低位向我有借位,所以当前位本身需要减1
		if (flag)
		{
			ret -= 1;
			flag = false;  //借位已经处理，置位false
		}

		if (ret < 0)
		{
			ret += 10;
			flag = true;
		}
		result.push_back(ret + '0');
	}
	//在上面已经区分出大小数字串，所以j字符串一定先结束，只需要判断一下i就行
	while (i >= 0)
	{
		int ret = maxStr[i] - '0';
		if (flag)
		{
			ret -= 1;
			flag = false;
		}

		if (ret < 0)
		{
			ret += 10;
			flag = true;
		}

		result.push_back(ret + '0');
		i--;
	}

	// 100000->反转000001
	//简单处理一下这种000001情况
	//100000从后往前遍历，如果是0，直到第一个不是0为止
	string retStr;
	auto it = result.rbegin();
	for (; it != result.rend(); ++it)
	{
		//注意是和字符'0'比较，而不是数字0
		if (*it != '0')
		{
			break;
		}
	}
	for (; it != result.rend(); ++it)
	{
		retStr.push_back(*it);
	}
	
	if (minor)
	{
		retStr.insert(retStr.begin(), '-');
	}

	//reverse(result.begin(), result.end());
	return retStr;
}
int main()
{
	BigInt int1("9785645649886874535428765");
	BigInt int2("28937697857832167849697653231243");
	BigInt int3("9785645649886874535428765");
	//28937707643477817736572188660008
	//28937707643477817736572188660008
	cout << int1 + int2 << endl;
	//28937688072186517962823117802478
	//28937688072186517962823117802478
	cout << int1 - int2 << endl;

	BigInt int4("123");
	BigInt int5("99");
	cout << int5 - int4 << endl;

	//999545-999544
	BigInt int6("999545");
	BigInt int7("999544");
	cout << int6 - int7 << endl;  //1
	cout << int7 - int6 << endl;  //-1

	return 0;
}
