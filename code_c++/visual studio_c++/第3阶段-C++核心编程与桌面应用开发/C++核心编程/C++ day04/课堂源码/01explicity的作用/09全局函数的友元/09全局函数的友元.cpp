#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>

class Building
{
	//�������ȫ�ֺ���ΪBuilding�����Ԫ����
	friend void GoodGay(Building &bd);
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

void GoodGay(Building &bd)
{
	cout << "�û��ѷ���:" << bd.keting << endl;
	cout << "�û��ѷ���:" << bd.woshi << endl;
}

void test01()
{
	Building my;

	GoodGay(my);
}
int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

