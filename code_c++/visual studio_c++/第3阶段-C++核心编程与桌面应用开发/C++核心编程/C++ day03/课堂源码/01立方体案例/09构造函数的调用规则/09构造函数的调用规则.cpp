#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:

	/*Maker(int a)
	{
		cout << "�вι��캯��" << endl;
	}*/

	Maker(const Maker &m)
	{
		cout << "�������캯��" << endl;
	}

};
//1.�������Ա�ṩ���вι��죬��ô�����������ṩĬ�Ϲ��캯�������ǻ��ṩĬ�ϵĿ�������
void test01()
{
	//Maker m;//err

	//Maker m(10);//�����вι���
	//Maker m2(m);//����Ĭ�ϵĿ�������
}
//2.�������Ա�ṩ�˿������캯������ô�����������ṩĬ�ϵĹ��캯����Ĭ�ϵĿ������캯��
void test02()
{
	//Maker m;
}
int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

