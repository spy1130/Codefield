#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Father
{
public:
	Father()
	{
		a = 10;
	}

	void func()
	{
		cout << "Father func" << endl;
	}
	void func(int a)
	{
		cout << "Father func(int a)" << endl;
	}

	void func(int a,int b)
	{
		cout << "Father func(int a,int b)" << endl;
	}


public:
	int a;
};
class Son :public Father
{
public:
	Son()
	{
		a = 20;
	}
	void func()
	{
		cout << "Son func" << endl;
	}
public:
	int a;
};
//������͸�����ͬ����Աʱ�������ͬ����Ա�����ظ����ͬ����Ա
void test()
{
	Son s;
	cout << s.a << endl;
	cout << sizeof(Son) << endl;//8
	Father f;
	s.Father::a = 200;
	//ͨ����������������������
	cout<<s.Father::a << endl;
	
	cout << f.a << endl;


}
//�������к͸���ͬ������ʱ����������к������ض��ᱻ����
void test02()
{
	Son s;
	s.func();
	//s.func(10);err
	//s.func(10, 20);err
	//ͨ�����������������صĸ��ຯ��
	s.Father::func(10);
	s.Father::func(10, 20);
}
int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

