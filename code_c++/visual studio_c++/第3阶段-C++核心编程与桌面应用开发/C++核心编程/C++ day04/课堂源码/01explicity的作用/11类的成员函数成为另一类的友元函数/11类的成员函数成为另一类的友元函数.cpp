#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;
//1.������֪�������������֪����Ľṹ
class Building;//������
class GoodGay
{
public:
	void func(Building &bud);
};
class Building
{
	//����GoodGay��ĳ�Ա����func��ΪBuilding�����Ԫ����
	friend void GoodGay::func(Building &bud);
public:
	Building()
	{
		keting = "����";
		woshi = "����";
	}

public:
	string keting;
private:
	string woshi;
};


void GoodGay::func(Building &bud)
{
	cout << "����:" << bud.keting << endl;
	cout << "����:" << bud.woshi << endl;
}

void test()
{
	Building bud;
	GoodGay GF;
	GF.func(bud);
}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

