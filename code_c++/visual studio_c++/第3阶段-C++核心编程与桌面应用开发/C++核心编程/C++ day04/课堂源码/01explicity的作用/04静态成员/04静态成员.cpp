#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:
	Maker()
	{
		
	}
public:
	//1.��̬��Ա������������������������������������
	static int a;
};
//2.��̬��Ա����Ҫ�����������������ʼ��
int Maker::a = 100;
void test01()
{
	//3.��̬��Ա���������࣬�����ڶ��������ж�����
	cout << Maker::a << endl;
	Maker m;
	cout << m.a << endl;
	//4.��̬��Ա��������������ʣ�Ҳ�����ö������
}

class Maker2
{
public:
	Maker2()
	{

	}
	//��̬��Ա����ֻ�ܷ��ʾ�̬��Ա����
	static void setA(int a)
	{
		a2 = a;
		cout << "a2=" << a2 << endl;
		//a1 = a;���ܷ�����ͨ��Ա����
	}
public:
	int a1;
	static int a2;
};
int Maker2::a2 = 200;
void test02()
{
	Maker2::setA(300);
}
class Maker3
{
private:
	static void func()
	{
		cout << "a3=" << a3 << endl;
	}
private:
	static int a3;
};
int Maker3::a3 = 300;
void test03()
{
	//Maker3::func();err
}
//const���εľ�̬��Ա������������ڳ�ʼ��
class Maker4
{
public:
	const static int a = 20;
	const static int b;
};
//����Ҳ���Գ�ʼ��
const int Maker4::b = 30;


class Maker5
{
public:
	void func()
	{
		cout << "a=" << a << endl;
	}
public:
	static int a;
};

//����Ҳ���Գ�ʼ��
int Maker5::a = 30;

void test04()
{
	Maker5 m;

	m.func();
	
}
int main()
{
	
	test04();
	system("pause");
	return EXIT_SUCCESS;
}

