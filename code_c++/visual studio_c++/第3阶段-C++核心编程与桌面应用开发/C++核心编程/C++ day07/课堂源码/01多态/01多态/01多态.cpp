#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class People
{
public:
	//�麯��
	virtual void Mypro()
	{

	}
};
//1.�м̳�
class xishi :public People
{
public:
	//2.��д������麯��
	virtual void Mypro()
	{
		cout << "Լ��ʩ" << endl;
	}
};

class wangzhaojun :public People
{
public:
	
	//��д������麯��
	virtual void Mypro()
	{
		cout << "Լ���Ѿ�" << endl;
	}
	
};



class diaochan :public People
{
public:
	//��д������麯��
	virtual void Mypro()
	{
		cout << "Լ����" << endl;
	}
};

class yangguifei :public People
{
public:
	//��д������麯��
	virtual void Mypro()
	{
		cout << "Լ�����" << endl;
	}
};


//ͬһ������
void doLogin(People *pro)
{
	pro->Mypro();//������ͬ��Ч��
}

void test()
{
	People *pro = NULL;
	//3.����ָ��ָ���������
	pro = new xishi;
	doLogin(pro);//��ͬ�Ķ���
	delete pro;

	pro = new wangzhaojun;
	doLogin(pro);//��ͬ�Ķ���
	delete pro;

	pro = new diaochan;
	doLogin(pro);//��ͬ�Ķ���
	delete pro;

	pro = new yangguifei;
	doLogin(pro);//��ͬ�Ķ���
	delete pro;
	pro = NULL;

	
	
}

int main()
{
	test();

	system("pause");
	return EXIT_SUCCESS;
}

