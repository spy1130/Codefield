#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;
//һ�����������˺������÷��ţ���ô�����ʵ�������Ķ���Ҳ�зº���
//�º��������ã�1.�������ά�� 2.������Ȩ�޵ĵ��ú�����3.��Ϊ�㷨�Ĳ��ԣ��󽫣�
class Maker
{
public:
	Maker()
	{
		name = "�仨";
	}
	void printMaker()
	{
		cout << name + "���Ư��" << endl;
	}

	void operator()()
	{
		cout << "hello" << endl;
	}

	void operator()(int v1,int v2)
	{
		cout << v1+v2 << endl;
	}
public:
	string name;
};

void func2()
{

}

void test()
{
	Maker func;

	func();//��������������ʵfunc�Ƕ���

	func(10, 20);

	func.printMaker();
}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

