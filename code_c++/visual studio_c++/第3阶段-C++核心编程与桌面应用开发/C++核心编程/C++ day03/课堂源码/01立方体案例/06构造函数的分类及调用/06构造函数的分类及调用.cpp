#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:
	//���ղ�������:�޲Σ��в�
	Maker()
	{
		cout << "�޲ι���" << endl;
	}
	Maker(int a)
	{
		cout << "�вι���" << endl;
	}
	//��������:��ͨ���캯�����������캯��
	Maker(const Maker &m)
	{
		cout << "��������" << endl;
	}

	//Ĭ�ϵĸ�ֵ���������潲
};

void test01()
{
	Maker m;//�����޲ι��캯��
	Maker m1(10);//�����вι���
	Maker m2(m1);//���ÿ�������

	//������
	Maker m4 = Maker(10);//���õ����вι��캯��
	Maker m3 = m2;//���ÿ�������
	cout << "=====" << endl;
	Maker m5 = 10;//Maker m5=Maker(10);
	cout << "=======" << endl;

	Maker m6;
	m6 = m5;//��ֵ����

}

int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

