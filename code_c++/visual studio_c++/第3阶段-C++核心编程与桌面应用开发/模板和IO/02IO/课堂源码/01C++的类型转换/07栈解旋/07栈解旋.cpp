#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Maker
{
public:
	Maker()
	{
		cout << "Maker�Ĺ���" << endl;
	}
	Maker(const Maker &m)
	{
		cout << "Maker�Ŀ�������" << endl;
	}
	~Maker()
	{
		cout << "Maker������" << endl;
	}
};

void func()
{
	//���׳��쳣�ĺ����У�����׳��쳣֮�󣬵�����û�н�������ʱ��ջ������Ķ��󶼻ᱻ�ͷ�
	//��ͽ�ջ����
	Maker m;
	throw m;//���m��Maker m����һ�ݵ�

	cout << "func��������" << endl;
}

void test()
{
	try
	{
		func();
		cout << "func()�����" << endl;
	}
	catch (Maker)
	{
		cout << "����һ��Maker���͵��쳣" << endl;
	}

	cout << "....." << endl;
}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

