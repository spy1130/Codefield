#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:
	Maker()
	{
		cout << "�޲ι��캯��" << endl;
	}
	Maker(int a)
	{
		cout << "�вι��캯��" << endl;
	}
	Maker(const Maker &maker)
	{
		cout << "�������캯��" << endl;
	}
	~Maker()
	{
		cout << "��������" << endl;
	}
};
void test01()
{
	Maker();//������������������ڵ�ǰ��
	Maker(10);
	

	//ע�⣬��������������������ӣ���ô�Ͳ�����������

	Maker m1 = Maker();

	cout << "test01()��������" << endl;
}

int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

