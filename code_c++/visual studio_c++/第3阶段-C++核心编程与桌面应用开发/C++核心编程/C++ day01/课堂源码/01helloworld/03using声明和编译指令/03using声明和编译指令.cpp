#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
namespace A
{
	int a = 10;
	int b = 20;
	int c = 30;
}

void test01()
{
	//using�������������ռ���ĳ����ʶ������ֱ��ʹ��
	using A::a;
	cout <<a << endl;

	//int a = 50;//ע��:using������ĳ���������ڸ��������ڲ��ܶ���ͬ���ı���
}

void test02()
{
	//using����ָ���ĳ�������ռ��еı�ʶ��������ֱ��ʹ��
	using namespace A;
	cout << a << endl;
	cout << b << endl;
	cout << c << endl;

	int a = 100;//Ϊʲô�����ͻ
	//�������������е�a��ȫ�ֱ����������a�ľֲ�����
	cout << "a=" << a << endl;

}

int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

