#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//�ṹ���Ĭ��Ȩ���ǹ��еģ����Ĭ��Ȩ����˽�е�
//�ṹ��
struct Maker1
{
	int a;
	void func()
	{

	}
public:
	int b;
};
//��
class Maker2
{
	int a;
	void func()
	{

	}
public:
	int b;
};

//�̳�
struct SonMaker1 :public Maker1
{

};

class SonMaker2 :public Maker2
{

};


int main()
{
	Maker1 m1;
	Maker2 m2;
	m1.b;
	m2.b;

	m1.a;
	//m2.a;
	system("pause");
	return EXIT_SUCCESS;
}

