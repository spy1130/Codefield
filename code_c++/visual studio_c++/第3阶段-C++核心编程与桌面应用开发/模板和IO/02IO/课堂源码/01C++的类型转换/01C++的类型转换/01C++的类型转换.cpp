#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//��̬ת��
//1.��������ת
void test01()
{
	char a = 'a';
	//char->double;
	//static_cast<Ҫת��������>����˭ת����
	double d = static_cast<double>(a);

	//double d1 = (double)a;
}
//2.�в�ι�ϵ���ָ�������ת��
class Father
{

};
class Son :public Father
{

};

class Other
{

};
void test02()
{
	Father *f = NULL;
	Son *s = NULL;

	//����ת������ȫ
	Son* s1=static_cast<Son*>(f);
	//����ת������ȫ
	Father* f1 = static_cast<Father*>(s);
	//û�м̳й�ϵ����֮���ָ�벻��ת��
	//Other *o = static_cast<Other*>(s);
}
//����ת��
void test03()
{
	Father f;
	Son s;

	Father &ref_f = f;
	Son &ref_s = s;
	//���ϣ���ȫ
	static_cast<Father&>(ref_s);
	//���£�����ȫ
	static_cast<Son&>(ref_f);
}

//��̬ת��
void test04()
{
	//char a = 'a';
	//dynamic_cast<double>(a);�������Ͳ���ʹ�ö�̬ת��

	Father *f = NULL;
	Son *s = NULL;
	//���ϣ���ȫ
	Father *f1 = dynamic_cast<Father*>(s);
	//���£�����ȫ������
	//Son *s1 = dynamic_cast<Son*>(f);
}
//������̬ʱ������ת������̬ת���Ϳ���
class Father2
{
public:
	virtual void func(){};
};
class Son2 :public Father2
{
public:
	virtual void func(){};
};
void test05()
{
	Father2* f = new Son2;

	dynamic_cast<Son2*>(f);
}

//����ת��
void test06()
{
	const int *p = NULL;
	//const-->����const
	int *newP = const_cast<int*>(p);

	int *pp = NULL;
	const int* newPP = const_cast<const int*>(pp);
}
//���½���ת��
void test07()
{
	int a = 10;
	int *p = reinterpret_cast<int *>(a);

	Father *f = NULL;
	Other *o = reinterpret_cast<Other*>(f);
}

int main()
{
	test05();
	system("pause");
	return EXIT_SUCCESS;
}

