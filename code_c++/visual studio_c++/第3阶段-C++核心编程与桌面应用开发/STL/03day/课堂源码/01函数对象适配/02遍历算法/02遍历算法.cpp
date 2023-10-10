#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<algorithm>
#include<functional>
using namespace std;

struct myPrint
{
public:
	myPrint()
	{
		a = 0;
	}
	void operator()(const int &val)
	{
		cout << val << " ";
		a++;
	}
public:
	int a;
};
void test01()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);
	/*
	void _For_each(_InIt _First, _InIt _Last, _Fn1& _Func)
	{	// perform function for each element
		for (; _First != _Last; ++_First)
			_Func(*_First);
	}
	*/
	myPrint m;
	myPrint m2=for_each(v.begin(), v.end(), myPrint());
	cout << endl;
	cout << m2.a << endl;
	cout << m.a << endl;
}
/*
/*
transform�㷨 ��ָ����������Ԫ�ذ��˵���һ������
ע�� : transform �����Ŀ�����������ڴ棬������Ҫ������ǰ������ڴ�
@param beg1 Դ������ʼ������
@param end1 Դ��������������
@param beg2 Ŀ��������ʼ������
@param _cakkback �ص��������ߺ�������
@return ����Ŀ������������

transform(iterator beg1, iterator end1, iterator beg2, _callbakc)
*/

struct MyAdd
{
	int operator()(int v1)
	{
		return v1 + 100;
	}
};
void test02()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int> v2;
	//Ҫ�ֶ�����ռ�
	v2.resize(v.size());
	/*
	_OutIt _Transform(_InIt _First, _InIt _Last,_OutIt _Dest, _Fn1 _Func)
	{	
		for (; _First != _Last; ++_First, ++_Dest)
			*_Dest = _Func(*_First);
		return (_Dest);
	}
	*/
	transform(v.begin(), v.end(), v2.begin(), MyAdd());
	for_each(v2.begin(), v2.end(), [](int val)->void{cout << val << " "; });
}
/*
�����������е�Ԫ�ش������������������
_OutIt _Transform(_InIt1 _First1, _InIt1 _Last1,_InIt2 _First2, _OutIt _Dest, _Fn2 _Func)
{
	for (; _First1 != _Last1; ++_First1, ++_First2, ++_Dest)
		*_Dest = _Func(*_First1, *_First2);
	return (_Dest);
}
*/
struct MyAdd2
{
	int operator()(int v1, int v2)
	{
		return v1 + v2;
	}
};
void test03()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int> v2;
	v2.push_back(100);
	v2.push_back(200);
	v2.push_back(300);
	v2.push_back(400);
	v2.push_back(500);

	vector<int> v3;
	int a = 0;
	if (v.size() > v2.size())
	{
		a = v.size();
	}
	else
	{
		a = v2.size();
	}
	v3.resize(a);
	transform(v.begin(), v.end(), v2.begin(), v3.begin(), MyAdd2());
	for_each(v3.begin(), v3.end(), [](int val)->void{cout << val << " "; });
}

int main()
{
	test03();
	system("pause");
	return EXIT_SUCCESS;
}

