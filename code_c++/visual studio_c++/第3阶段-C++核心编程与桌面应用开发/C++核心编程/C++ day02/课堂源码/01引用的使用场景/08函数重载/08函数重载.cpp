#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//�����ĸ�����ͬ
void func()
{
	cout << "func()" << endl;
}

void func(int a)
{
	cout << "func(int a)" << endl;
}

//���������Ͳ�ͬ
//void func(char c)
//{
//	cout << "func(char c)" << endl;
//}
//������˳��ͬ
void func(int a, double b)
{
	cout << "func(int a, double b)" << endl;
}
void func(double b, int a)
{
	cout << "func(double b, int a)" << endl;
}

void test01()
{
	int a = 10;
	double b = 3.14;
	//��������ͨ������ú���ʱ������Ĳ������жϵ������ص��ĸ�����
	func();
	//func(b);// err doubleת�����˳�Ϊint��char
	func(a, b);
	func(b, a);
	char c = 'c';
	func(c);//charת��int�ɹ�������int�����ĺ���
}

//�������غͺ���Ĭ�ϲ���һ��ʹ��
void myfunc(int a, int b = 0)
{
	cout << "myfunc(int a, int b = 0)" << endl;
}

void myfunc(int a)
{
	cout << "myfunc(int a)" << endl;
}

void test02()
{
	//myfunc(10); //err,���������⣬��֪�������ĸ�����
}

int main()
{
	test01();

	
	system("pause");
	return EXIT_SUCCESS;
}

