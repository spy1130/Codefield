#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Maker
{
public:
	Maker()
	{
		cout << "Maker�Ĺ���" << endl;
	}
	Maker(const Maker &m)
	{
		cout << "Maker�Ŀ�������" << endl;
	}
	~Maker()
	{
		cout << "Maker������" << endl;
	}
};
//������������
void func1()
{
	Maker m;//��һ���������쳣����ǰ���ͷ�
	throw m;//�ڶ��������ǵ�һ�����󿽱�������
}

void test01()
{
	try
	{
		func1();
	}
	catch (Maker m1)//�����������ǵڶ������󿽱�������
	{
		cout << "����һ��Maker���͵��쳣" << endl;
		//�ڶ����͵�����������catch����ʱ�ͷ�

	}
}
//������������
void func2()
{
	//��һ������
	throw Maker();//��������
}

void test02()
{
	try
	{
		func2();
	}
	catch (Maker m1)//�ڶ�������
	{
		cout << "����һ��Maker���͵��쳣" << endl;
		//��һ���͵ڶ���������catch����ʱ�ͷ�

	}
}
//����һ�����󣬳������ַ�ʽ
void func3()
{
	throw Maker();//��������

}

void test03()
{
	try
	{
		func3();
	}
	catch (Maker &m1)
	{
		cout << "����һ��Maker���͵��쳣" << endl;


	}

}

void func4()
{
	//�������������ջ�е���������ȡ��ַ����
	//throw Maker();//��������
	//����������Զ����е���������ȡ��ַ����
	throw new Maker();
}

void test04()
{
	try
	{
		func4();
	}
	catch (Maker *m1)
	{
		cout << "����һ��Maker���͵��쳣" << endl;

		//delete m1;
	}
}
int main()
{
	test04();
	system("pause");
	return EXIT_SUCCESS;
}

