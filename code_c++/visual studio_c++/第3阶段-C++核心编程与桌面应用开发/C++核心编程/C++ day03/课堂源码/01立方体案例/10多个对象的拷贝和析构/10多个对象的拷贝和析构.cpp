#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class BMW
{
public:

	BMW()
	{
		cout << "BMW�вι���" << endl;
	}

	~BMW()
	{
		cout << "BMW����" << endl;
	}
};

class Buick
{
public:
	Buick()
	{
		cout << "Buick����" << endl;
	}

	~Buick()
	{
		cout << "Buick����" << endl;
	}
};

class Maker
{
public:
	
	Maker() 
	{
		cout << "Maker����" << endl;
	}

	~Maker()
	{
		cout << "Maker����" << endl;
	}
private:
	
	Buick bui;//��Ա����
	BMW bmw;//��Ա����
};
//1.������г�Ա������ô�ȵ��ó�Ա����Ĺ��캯�����ٵ��ñ���Ĺ��캯��
//���������ĵ���˳��֮
//2.��Ա����Ĺ��캯�����úͶ���˳��һ��
//3.ע�⣬����г�Ա������ôʵ��������ʱ�����뱣֤��Ա����Ĺ���������ܱ�����
void test01()
{
	Maker m;
}

class BMW2
{
public:
	BMW2()
	{

	}
	BMW2(int a)
	{
		cout << "BMW2�вι���" << a << endl;
	}

	~BMW2()
	{
		cout << "BMW2����" << endl;
	}
};

class Buick2
{
public:
	Buick2()
	{

	}
	Buick2(int b,int c)
	{
		cout << "Buick2����" << endl;
	}

	~Buick2()
	{
		cout << "Buick2����" << endl;
	}
};

class Maker2
{
public:
	//��ʼ���б�
	//ע��1����ʼ���б�ֻ��д�ڹ��캯��
	/*Maker2() :bmw(10)
	{
		cout << "Maker2����" << endl;
	}*/
	//����ж��������Ҫָ������ĳ�����캯�����ö��Ÿ���
	Maker2(int a,int b,int c) :bmw(a), bui(b,c)
	{
		cout << "Maker2����" << endl;
	}
	//ע��2:���ʹ���˳�ʼ���б���ô���еĹ��캯����Ҫд��ʼ���б�
	Maker2(const Maker &m2) :bmw(40), bui(10,20)
	{

	}
	Maker2(int a,int b)
	{

	}
	/*void printMaker2() : bmw(10)
	{

	}*/

	~Maker2()
	{
		cout << "Maker2����" << endl;
	}
private:

	Buick2 bui;//��Ա����
	BMW2 bmw;//��Ա����
};

//��ʼ���б��ǵ��ó�Ա�����ָ�����캯��
void test02()
{
	Maker2 m(30,10,20);
}


int main()
{
	test05();

	system("pause");
	return EXIT_SUCCESS;
}

