#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

//�Զ������������
//����Բ����
class Circle
{
public:
	//���ð뾶�ĳ���
	void setR(double r)
	{
		mR = r;
	}
	//����Բ���ܳ�
	double getL()
	{
		return 2 * 3.14*mR;
	}
public://����Ȩ��
	double mR;//��Ա��������Ա����


};

void test()
{
	//�������Ͷ������
	//��ʵ��������
	Circle c;
	c.setR(100);
	cout << "Բ���ܳ�=" << c.getL() << endl;;
}

void test02()
{
	char *p = "hello";
	char buf[100] = "hello";
	cout << buf << endl;

	string str = "world";
	cout << str << endl;
}

int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

