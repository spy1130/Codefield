#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

int func(int a, int b)
{
	if (b == 0)
	{
		//2.�׳��쳣
		throw 10;//�׳�һ��int���͵��쳣��
	}

	return a / b;
}
void test()
{
	int a = 10;
	int b = 0;
	//1.���п��ܳ����쳣�Ĵ����ŵ�try��
	try
	{
		func(a, b);
	}
	catch (int &a)//3.����һ��int���͵��쳣
	{
		cout << "����һ��int���͵��쳣" << endl;
		
	}

}
int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

