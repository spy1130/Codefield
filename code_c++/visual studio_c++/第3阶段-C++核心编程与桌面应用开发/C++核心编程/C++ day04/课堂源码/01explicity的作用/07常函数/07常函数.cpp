#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:
	Maker(int id, int age)
	{
		this->id = id;
		this->age = age;
		score = 100;
	}
	//������,1.������()�������const,�ú����ǳ�����
	void printMaker()const
	{
		//id = 100;err//2.�������ڲ����޸���ͨ��Ա����
		//3.const���ε���thisָ��ָ��Ŀռ��еı������ܸı�
		//Maker *const this;
		//const Maker*const this;���ǳ��������ε�
		score = 200;//4.mutable���εĳ�Ա�����ڳ������п����޸�
		cout << "score=" << score << endl;
	}
public:
	int id;
	int age;
	mutable int score;//mutable���εĳ�Ա����

};
void test01()
{
	Maker m(1, 18);
	m.printMaker();
}

int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

