#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>

//���󣬻�ȡ��ӡ��ʹ�õĴ���
class Printer
{
private:
	//1.���޲ι���Ϳ�������˽�л�
	Printer()
	{
		mcount = 0;
	}
	Printer(const Printer &p)
	{

	}
public:
	static Printer *getPrinter()
	{
		return p;
	}

	void printPrinter(string name)
	{
		cout << name << ":��ӡ" << endl;
		mcount++;
	}
	int getCount()
	{
		return mcount;
	}
private:
	int mcount;//��¼��ӡ��ӡ�Ĵ���
	//2.���徲̬��Աָ��
	static Printer *p;
};
//3.������г�ʼ����new����
Printer *Printer::p = new Printer;

void test()
{
	//���۲�
	Printer *p1 = Printer::getPrinter();
	p1->printPrinter("���۲�");

	//������
	Printer *p2 = Printer::getPrinter();
	p2->printPrinter("������");

	//���ز�
	Printer *p3 = Printer::getPrinter();
	p3->printPrinter("���ز�");

	Printer *p4 = Printer::getPrinter();
	cout << "��ӡʹ�õĴ���:"<<p4->getCount() << endl;

}

int main()
{
	test();

	system("pause");
	return EXIT_SUCCESS;
}

