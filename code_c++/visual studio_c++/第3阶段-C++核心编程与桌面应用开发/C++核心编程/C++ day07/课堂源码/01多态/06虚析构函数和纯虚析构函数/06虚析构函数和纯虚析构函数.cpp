#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include <cstring>
class Animal
{
public:
	Animal()
	{
		cout << "Animal�Ĺ���" << endl;
	}

	//����������������������������
	/*virtual ~Animal()
	{
		cout << "Animal������" << endl;
	}*/

	//����������������������������Ҫ������ʵ��
	virtual ~Animal() = 0;
};

//������ʵ�ִ�����������
Animal::~Animal()
{
	cout << "Animal�Ĵ�������" << endl;
}

class Son :public Animal
{
public:
	Son()
	{
		cout << "Son�Ĺ���" << endl;
		pName = new char[64];
		memset(pName, 0, 64);
		strcpy(pName, "�绨");
	}

	~Son()
	{
		
		if (pName != NULL)
		{
			delete[] pName;
			pName = NULL;
		}
		cout << "Son������" << endl;
	}

public:
	char *pName;
};

void test()
{
	Animal *animal = new Son;
	delete animal;
}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

