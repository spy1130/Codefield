#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
const int aa = 10;//û���ڴ�

void test01()
{
	//�����˳����۵�
	cout << "aa=" << aa << endl;//�ڱ���׶Σ���������cout<<"aa="<<10<<endl;

	//��ֹ�Ż�volatile
	//volatile const int bb = 20;//ջ��
	 const int bb = 20;
	int *p = (int*)&bb;
	*p = 200;
	cout << "bb=" << bb << endl;//cout << "bb=" << 20 << endl;
	cout << "*p=" << *p << endl;

	cout << "a�ĵ�ַ=" << (int)&bb << endl;
	cout << "pָ��ĵ�ַ" << (int)p << endl;


}
int main()
{
	test01();

	//C++������const���ε�ȫ�ֱ��������ڲ���������
	extern const int c;
	cout << "c=" << c << endl;
	system("pause");
	return EXIT_SUCCESS;
}

