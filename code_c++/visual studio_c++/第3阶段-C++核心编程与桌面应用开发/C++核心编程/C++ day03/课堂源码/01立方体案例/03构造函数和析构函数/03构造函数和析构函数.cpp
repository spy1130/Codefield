#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:
	//���캯���������ǳ�ʼ����Ա�������Ǳ�����ȥ���õ�
	Maker()
	{
		a = 10;
		cout << "���캯��" << endl;
	}

	//�����������ڶ�������ǰ��������������������
	~Maker()
	{
		cout << "��������" << endl;
	}
public:
	int a;
};

void test01()
{
	//ʵ���������ڲ����������£�1.����ռ䣬2.���ù��캯�����г�ʼ��
	Maker m;
	int b = m.a;
	cout << b << endl;

}
//��������������
class Maker2
{
public:
	//�вι���
	Maker2(const char *name,int age)
	{
		cout << "�вι���" << endl;
		//�Ӷ����ռ�����
		pName = (char*)malloc(strlen(name) + 1);
		strcpy(pName, name);
		mAge = age;
	}

	void printMaker2()
	{
		cout << "name:" << pName << " age:" << mAge << endl;
	}
	~Maker2()
	{
		cout << "��������" << endl;
		//�ͷŶ����ռ�
		if (pName != NULL)
		{
			free(pName);
			pName = NULL;
		}
	}
private:
	char *pName;
	int mAge;
};

class Maker3
{
public://ע��2�����캯�����������������ǹ���Ȩ��
	//ע��1�����캯����������
	Maker3()//�޲ι��캯��
	{
		cout << "Maker3���޲ι���" << endl;
	}
	Maker3(int a)//�вι��캯��
	{
		cout << "Maker3���вι���" << endl;
	}
	
	~Maker3()
	{
		cout << "��������" << endl;
	}

};

void test02()
{
	Maker2 m2("�仨",18);
	m2.printMaker2();

}

void test03()
{
	Maker3 m;//�����캯��˽��ʱ��ʵ�������˶���


	//�ж��������Ȼ����ù��캯�����ж������ٱ�Ȼ�������������
	//�ж��ٸ���������ͻ���ö��ٴι��캯�����ж��ٸ��������پͻ���ö��ٴ���������

	Maker3 m2(10);
}

int main()
{
	test03();

	system("pause");
	return EXIT_SUCCESS;
}

