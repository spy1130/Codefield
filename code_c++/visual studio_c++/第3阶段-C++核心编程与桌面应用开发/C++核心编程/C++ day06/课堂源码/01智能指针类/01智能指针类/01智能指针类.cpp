#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:
	Maker()
	{
		cout << "�޲ι���" << endl;
	}
	void printMaker()
	{
		cout << "hello Maker" << endl;
	}
	~Maker()
	{
		cout << "��������" << endl;
	}
};

class SmartPoint
{
public:
	SmartPoint(Maker *m)
	{
		this->pMaker = m;
	}
	//����ָ�������
	Maker *operator->()
	{
		return this->pMaker;
	}
	//�����ǻ�
	Maker &operator*()
	{
		return *pMaker;
	}

	~SmartPoint()
	{
		if (this->pMaker != NULL)
		{
			cout << "SmartPoint ��������" << endl;
			delete this->pMaker;
			this->pMaker = NULL;
		}
	}
private:
	Maker *pMaker;
};
void test01()
{
	Maker *p = new Maker;
	
	SmartPoint sm(p);//ջ�����������������
	//��test01()��������ʱ�������SmartPoint������������
	//��������������delete��Maker�Ķ��󣬻����Maker����������

}
void test02()
{
	Maker *p = new Maker;

	SmartPoint sm(p);
	//sm-> ==> pMaker->
	sm->printMaker();

	(*sm).printMaker();

}

int main()
{
	test02();

	system("pause");
	return EXIT_SUCCESS;
}

