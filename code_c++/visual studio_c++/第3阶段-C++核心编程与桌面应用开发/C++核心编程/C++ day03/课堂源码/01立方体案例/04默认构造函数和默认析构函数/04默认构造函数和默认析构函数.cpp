#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:
	Maker()//Ĭ�ϵĹ��캯�����������ǿյ�
	{

	}
	~Maker()//Ĭ�ϵ�����������������Ҳ�ǿ�
	{

	}
	//������Ĭ���ṩĬ�ϵĹ��캯������������
	void printfMaker()
	{
		a = 100;
		cout << "a=" << a << endl;
	}
private:
	int a;
};

void test01()
{
	Maker m;
	m.printfMaker();
}
int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

