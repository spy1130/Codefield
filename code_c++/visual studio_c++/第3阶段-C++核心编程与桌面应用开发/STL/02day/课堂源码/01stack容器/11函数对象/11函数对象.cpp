#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<functional>
#include<algorithm>
#include<vector>

void test()
{
	vector<int> v;
	v.push_back(8);
	v.push_back(1);
	v.push_back(6);
	v.push_back(3);
	v.push_back(7);


	sort(v.begin(), v.end(), greater<int>());

	for_each(v.begin(), v.end(), [](int val){cout << val << " "; });
	//[](int val){cout << val << " "; }//��������


}

void func()
{

}

struct Myfunc//��������������
{
public:
	Myfunc()
	{
		n = 0;
	}
	void operator()()//�����������Լ���״̬
	{
		cout << "hello" << endl;
	}
public:
	int n;

};

void test01()
{
	plus<int> myplus;
	cout << myplus(10, 20) << endl;


}

int main()
{
	

	system("pause");
	return EXIT_SUCCESS;
}

