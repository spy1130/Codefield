#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include"test.h"
using namespace std;



//�����ռ�
namespace A{//A�ǿռ�����֣�
	int a;
	void func()
	{

	}
}
//�����ռ����Ƕ�������ռ�
namespace Maker
{
	int a;
	namespace B
	{
		int b=40;
	}
}
//void test()
//{
//	cout << Maker::c << endl;
//}


namespace Maker
{
	
	int c=30;
	
}
int mya = 10;

//������static int d=50;
namespace
{
	int d = 50;
}
//�����ռ�ȡ����

void test01()
{
	//			������    ������
	namespace nameMaker = Maker;

	cout << nameMaker::a << endl;
}


int main()
{
	
	int mya = 20;
	//�ͽ�ԭ��
	cout << "mya=" << mya << endl;
	cout << "::mya=" << ::mya << endl;
	cout << Maker::a << endl;
	cout << Maker::c << endl;
	cout << Maker::B::b << endl;
	//test();
	myMaker1::func();
	

	cout << d << endl;
	system("pause");
	return EXIT_SUCCESS;
}

