#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Maker
{
public:
	Maker()
	{
		cout << "�޲ι��캯��" << endl;
	}
	Maker(int a)
	{
		cout << "�вι��캯��" << endl;
	}
	Maker(const Maker &maker)
	{
		cout << "�������캯��" << endl;
	}
	~Maker()
	{
		cout << "��������" << endl;
	}
};
//1.������ֵ��ʽ����������
void func(Maker m)//Maker m=m1;
{

}

void test01()
{
	Maker m1;
	func(m1);

}
//2.��һ�����еĶ���ȥ��ʼ����һ������
void test02()
{
	Maker m1;
	Maker m2(m1);
}

//3.�����ľֲ�������ֵ�ķ�ʽ�Ӻ�������
//vs Debugģʽ�£�����ÿ������죬vs Releaseģʽ�²�����ÿ������죬qtҲ������
Maker func2()
{
	//�ֲ�����
	Maker m;
	cout << "�ֲ�����ĵ�ַ:" << &m << endl;

	return m;
}

void test03()
{
	
	Maker m1 = func2();

	cout << "m1����ĵ�ַ��" << &m1 << endl;
}

int main()
{
	test03();
	system("pause");
	return EXIT_SUCCESS;
}

