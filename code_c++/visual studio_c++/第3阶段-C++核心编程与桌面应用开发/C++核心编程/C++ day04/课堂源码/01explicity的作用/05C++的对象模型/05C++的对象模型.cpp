#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//1.����Ĵ�С��1������0
class Maker
{

};

void test01()
{
	cout << sizeof(Maker) << endl;
	Maker *m = new Maker[20];//Ϊʲô������1����Ϊ���������ڴ�������ֶ���
}

class Maker2
{
public:
	void func()//1.��Ա������ռ����Ĵ�С
	{
		b = 20;
		cout << b << endl;
	}
	static int a;//2.��̬��Ա������ռ����Ĵ�С
	static void func2()//3.��̬��Ա������ռ����Ĵ�С
	{

	}
	int b;//4.��ͨ��Ա����ռ����Ĵ�С
};
int Maker2::a = 100;
void test02()
{
	cout << sizeof(Maker2) << endl;
	Maker2 m2;
	m2.func();
}

int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

