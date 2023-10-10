#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Drink
{
public:
	//��ˮ
	virtual void Boil() = 0;
	//����
	virtual void Brew() = 0;
	//���뱭��
	virtual void PourInCup() = 0;
	//�ӵ㸨��
	virtual void addSonm() = 0;

	//ģ�淽��,�ѵ��ú�����˳��ȷ������
	void func()
	{
		Boil();
		Brew();
		PourInCup();
		addSonm();
	}
};

class Coffee :public Drink
{
public:
	//��ˮ
	virtual void Boil()
	{
		cout << "���¶ˮ" << endl;
	}
	//����
	virtual void Brew()
	{
		cout << "����" << endl;
	}
	//���뱭��
	virtual void PourInCup()
	{
		cout << "��������" << endl;
	}
	//�ӵ㸨��
	virtual void addSonm()
	{
		cout << "�ӵ����" << endl;
	}
};

class Tea :public Drink
{
public:
	//��ˮ
	virtual void Boil()
	{
		cout << "�������ˮ" << endl;
	}
	//����
	virtual void Brew()
	{
		cout << "������" << endl;
	}
	//���뱭��
	virtual void PourInCup()
	{
		cout << "��������" << endl;
	}
	//�ӵ㸨��
	virtual void addSonm()
	{
		cout << "�ӵ㽴��" << endl;
	}
};



void test()
{
	Drink *dr = NULL;
	dr = new Coffee;
	dr->func();
	delete dr;

	dr = new Tea;
	dr->func();
	delete dr;
	dr = NULL;
}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

