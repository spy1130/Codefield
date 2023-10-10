#include "Circle.h"

void Circle::SetR(int r)
{
	mR = r;
}
void Circle::SetHear(Point &p)
{
	mHear.setX(p.getX());
	mHear.setY(p.getY());
}
void Circle::SetHear(int x, int y)
{
	mHear.setX(x);
	mHear.setY(y);
}

int Circle::getR()
{
	return mR;
}
Point Circle::getHear()
{
	return mHear;
}

//�жϵ��Բ�Ĺ�ϵ
void Circle::isPointAndCircle(Point &p)
{
	//��ȡԲ�ĺ͵�֮������ƽ��
	// distance =pow(x2-x1,2)+pow(y2-y1,2)
	double distance = pow((p.getX()-mHear.getX()), 2)+pow((p.getY()-mHear.getY()),2);

	//�뾶��ƽ��
	double tmpR = pow(mR, 2);

	if (distance > tmpR)
	{
		cout << "����Բ��" << endl;
	}
	else if (distance == tmpR)
	{
		cout << "����Բ��" << endl;
	}
	else
	{
		cout << "����Բ��" << endl;
	}
}
