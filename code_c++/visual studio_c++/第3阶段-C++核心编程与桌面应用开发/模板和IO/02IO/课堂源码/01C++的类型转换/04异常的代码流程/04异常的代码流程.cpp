#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

int func(int a, int b)
{
	if (b == 0)
	{
		//�ڶ�����
		throw 10;//�׳�һ��int���͵��쳣��
		cout << "throw��Ĵ���" << endl;
	}

	return a / b;
}
void test()
{
	int a = 10;
	int b = 0;
	
	try
	{
		func(a, b);//��һ����
		cout << "func��Ĵ���" << endl;
	}
	catch (int)//��������
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

