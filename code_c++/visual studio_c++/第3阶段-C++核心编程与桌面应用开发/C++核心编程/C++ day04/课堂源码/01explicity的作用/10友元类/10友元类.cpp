#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;
class Building
{
	//����GoodF��ΪBuilding�����Ԫ��
	friend class GoodF;
	friend class GoodF2;
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

class GoodF
{
public:
	void func(Building &bd)
	{
		cout << "����:" << bd.keting << endl;
		cout << "����:" << bd.woshi << endl;//ͨ����������˽�г�Ա
	}
};
//1.ͨ������������������˽�г�Ա
void test01()
{
	Building bd;
	GoodF f;
	f.func(bd);
}

//2.ͨ������ָ�����������˽�г�Ա
class GoodF2
{
public:
	GoodF2()
	{
		cout << "�޲ι���" << endl;
		pbu = new Building;
	}
	void func()
	{
		cout << "����:" << pbu->keting << endl;
		cout << "����:" << pbu->woshi << endl;//ͨ����Աָ�����˽�г�Ա
		
	}
	//��������
	GoodF2(const GoodF2 &f2)
	{
		cout << "��������" << endl;
		//1.����ռ�
		pbu = new Building;//new�Ѿ�������Building�Ĺ��캯��,�����ٸ��ƶ�������
		
		

	}
	~GoodF2()
	{
		cout << "��������" << endl;
		if (pbu != NULL)
		{
			cout << "pbu" << endl;
			delete pbu;
		}
	}
public:
	Building *pbu;
};

void test02()
{
	GoodF2 f;
	f.func();

	GoodF2 f2 = f;

}
int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

