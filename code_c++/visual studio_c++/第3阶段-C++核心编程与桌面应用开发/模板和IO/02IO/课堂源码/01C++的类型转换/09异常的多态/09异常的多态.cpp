#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//�쳣�Ļ���
class Father
{
public:
	virtual void printM()
	{

	}
};
//1.�м̳�
class SonNULL :public Father
{
public:
	virtual void printM()//2.��д������麯��
	{
		cout << "��ָ���쳣" << endl;
	}
};

class SonOut :public Father
{
public:
	virtual void printM()
	{
		cout << "Խλ���" << endl;
	}
};

void func(int a,int b)
{
	if (a == 0)
	{
		throw SonNULL();
	}
	
	if (b == 0)
	{
		throw SonOut();
	}
	
}

void test()
{
	int a = 0;
	int b = 10;
	try
	{
		func(a,b);
	}
	catch (Father &f)//3.��������ָ���������
	{
		f.printM();
	}
}
int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

