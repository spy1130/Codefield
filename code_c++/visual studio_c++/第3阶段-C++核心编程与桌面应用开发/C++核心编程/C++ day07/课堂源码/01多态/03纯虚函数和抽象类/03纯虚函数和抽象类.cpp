#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//�����
//�д��麯������г����࣬����ʵ��������
class rule
{
public:
	//���麯��
	virtual int getnum(int a, int b) = 0;
};

//ʵ�ֲ�
class plus_rule :public rule
{
public:
	virtual int getnum(int a, int b)//��д������麯�������������
	{
		return a+b;
	}
};

class miux_rule :public rule
{
public:
	virtual int getnum(int a, int b)
	{
		return a - b;
	}
};


//ҵ���
int doLogin(rule *cal)
{
	
	int a = 10;
	int b = 20;
	

	int ret=cal->getnum(a, b);//���������

	return ret;

}

void test()
{
	rule *r = NULL;
	r = new plus_rule;
	cout << doLogin(r) << endl;
	delete r;

	r = new miux_rule;
	cout << doLogin(r) << endl;
	delete r;
}
void test02()
{
	//�����಻��ʵ��������
	//rule r;
}

class Maker
{
public:
	virtual void func1() = 0;
	virtual void func2() = 0;
};

class Son :public Maker
{
public:
	virtual void func1()
	{

	}
	virtual void func2()
	{

	}
};

void test03()
{
	Son s;
}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

