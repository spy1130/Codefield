#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<algorithm>
#include<functional>
#include<numeric>//���������㷨��ͷ�ļ�
using namespace std;
//accumulate�㷨 ��������Ԫ���ۼ��ܺ�
void test01()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);

	//���������������ܺ����ټ���0
	int ret=accumulate(v.begin(), v.end(), 0);
	cout << ret << endl;

	
}

class Maker
{
public:
	Maker(int age)
	{
		this->age = age;
	}
public:
	int age;
};

struct MyPuls
{
	int operator()(int val,Maker &m)
	{
		return val + m.age;
	}
};

void test02()
{
	vector<Maker> v;
	v.push_back(Maker(10));
	v.push_back(Maker(20));
	v.push_back(Maker(30));

	int ret = accumulate(v.begin(), v.end(),0, MyPuls());
	cout << ret << endl;
}

//fill�㷨 �����������Ԫ��
void test03()
{
	vector<int> v;
	//Ҫ���ٿռ�
	v.resize(10);

	fill(v.begin(), v.end(), 100);
	for_each(v.begin(), v.end(), [](int val){cout << val << " "; });
}

int main()
{
	test03();
	system("pause");
	return EXIT_SUCCESS;
}

