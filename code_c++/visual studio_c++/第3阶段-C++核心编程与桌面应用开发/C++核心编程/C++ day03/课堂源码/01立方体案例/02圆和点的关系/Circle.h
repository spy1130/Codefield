#pragma once
#include"Point.h"
#include<cmath>//���ݹ�ʽ�Ŀ�
#include<iostream>
using namespace std;
class Circle
{
public:
	void SetR(int r);
	void SetHear(Point &p);
	void SetHear(int x, int y);

	int getR();
	Point getHear();

	//�жϵ��Բ�Ĺ�ϵ
	void isPointAndCircle(Point &p);
	
private:
	int mR;//�뾶
	Point mHear;//Բ��
};

