#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include"Circle.h"

//ȫ�ֺ����жϵ��Բ�Ĺ�ϵ
void G_isPointAndCircle(Circle &c, Point &p)
{
	//��ȡ���Բ֮������ƽ��
	double distance = pow((p.getX() - c.getHear().getX()), 2) + pow((p.getY() - c.getHear().getY()), 2);

	//�뾶��ƽ��
	double tmpR = pow(c.getR(), 2);

	if (distance > tmpR)
	{
		cout << "ȫ�� ����Բ��" << endl;
	}
	else if (distance == tmpR)
	{
		cout << "ȫ�� ����Բ��" << endl;
	}
	else
	{
		cout << "ȫ�� ����Բ��" << endl;
	}
}

int main()
{
	//ʵ���������
	Point p;
	p.setX(10);
	p.setY(10);
	//ʵ����Բ����
	Circle c;
	c.SetHear(10, 20);
	c.SetR(5);

	c.isPointAndCircle(p);

	G_isPointAndCircle(c, p);

	system("pause");
	return EXIT_SUCCESS;
}

