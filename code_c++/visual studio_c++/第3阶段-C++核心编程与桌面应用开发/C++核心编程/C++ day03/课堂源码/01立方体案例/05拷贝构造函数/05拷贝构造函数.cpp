#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//1.ʲô�ǿ�������
class Maker
{
public:
	Maker()
	{
		cout << "�޲ι��캯��" << endl;
		a = 20;
	}
	//�������캯��
	Maker(const Maker &m)
	{
		cout << "�������캯��" << endl;
		a = m.a;
		
	}
	//��ӡ����
	void printMaker()
	{
		cout << "a=" << a << endl;
	}
private:
	int a;
};



void test01()
{
	Maker m1;
	m1.printMaker();

	//��һ�����еĶ���ȥ��ʼ����һ������
	Maker m2(m1);
	m2.printMaker();
}
//2.�������ṩ��Ĭ�ϵĿ������캯��
class Maker2
{
public:
	Maker2()
	{
		cout << "�޲ι��캯��" << endl;
		a = 20;
	}
	//�������ṩ��Ĭ�ϵĿ������캯��
	//Maker2(const Maker2 &m)
	//{
	//	//Ĭ�Ͽ������캯�������˳�Ա�����ļ򵥿���
	//	a = m.a;
	//}

	//��ӡ����
	void printMaker()
	{
		cout << "a=" << a << endl;
	}
private:
	int a;
};


void test02()
{
	Maker2 m1;
	m1.printMaker();

	
	Maker2 m2(m1);
	m2.printMaker();
}
//3.�������캯�����β�Ҫ������
class Maker3
{
public:
	Maker3(int Ma)
	{
		cout << "�вι��캯��" << endl;
		ma = Ma;
	}
	Maker3(const Maker3 &m)
	{
		cout << "�������캯��" << endl;
	}
private:
	int ma;
};

void test03()
{
	Maker3 m1(10);//�����вι���

	Maker3 m2(m1);//���ÿ�������

	Maker3 m3 = m1;//���ÿ�������

	//����������캯���е��ββ�������
	/*
	Maker3(const Maker3 m)//const Maker3 m=m1;   const Maker3 m(m1);
	{
		cout << "�������캯��" << endl;
	}

	1.Maker3 m2(m1);
	2.const Maker3 m=m1;
	3.const Maker3 m(m1);
	4.const Maker3 m=m1;
	5.������ѭ��
	*/
}


int main()
{
	test03();
	system("pause");
	return EXIT_SUCCESS;
}

