#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//1.������Ϊ��������
void func(int &a, int &b)
{
	int sum = a + b;
	cout << "sum=" << sum << endl;
}

void test01()
{
	int a = 10;
	int b = 20;
	func(a, b);
}
//2.������Ϊ�����ķ���ֵ
int& func2()
{
	int b = 10;//��Ҫ���ؾֲ�����������
	int &p = b;
	return p;
}
int &func3()
{
	static int b = 10;
	return b;
}
void test02()
{
	int &q = func2();
	q = 100;
	cout << q << endl;

	func2() = 200;
	cout << q << endl;
////---------����Ĵ����Ǵ���ֻ�Ǳ�����û�м�����
	cout << "func2="<<func2() << endl;

	func3() = 100;//���Ҫ��������ֵ����ô�ú������뷵������
	cout << "func3()=" << func3() << endl;
}
int main()
{

	test02();
	system("pause");
	return EXIT_SUCCESS;
}

