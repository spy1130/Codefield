#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:
	Maker()
	{
		a = 10;
		b = 20;
	}
	void func()//func����ֻ��һ��
	{
		cout << this->a << " " << this->b << endl;
	}
public:
	int a;
	int b;
};
void test01()
{
	//1.����ռ䣬2�����ù��캯��
	Maker m;
	m.func();

	Maker m2;
	m2.func();
}

class Maker2
{
public:
	//1.���β����ͳ�Ա��������ͬʱ����thisָ������
	Maker2(int id)
	{
		this->id = id;
	}
	//2.���ض���ı���
	Maker2 &getMaker2()
	{
		return *this;//���������ʱ����
	}
	/*static void func()
	{
		this->a = 300;
	}*/
public:
	int id;
	static int a;
};
int Maker2::a = 200;
void test02()
{

}

int main()
{
	test01();

	system("pause");
	return EXIT_SUCCESS;
}

