#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:
	Maker()
	{
		cout << "���캯��" << endl;
	}

	Maker(int a)
	{
		cout << "�вι��캯��" << endl;
	}
	~Maker()
	{
		cout << "��������" << endl;
	}
};

void test01()
{
	//��C���Է�ʽ��������ռ䣬������ù��캯����
	Maker *m = (Maker*)malloc(sizeof(Maker));
	//�����ͷ�ʱ���������������
	free(m);
}

void test02()
{
	//��new��ʽ��������ռ䣬�������Ĺ��캯��
	Maker *m = new Maker;

	//�ͷŶ����ռ䣬����������������
	delete m;
	m = NULL;

	Maker *m2 = new Maker(10);

	delete m2;
	m2 = NULL;
}
int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

