#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

void test01()
{
	char* p = "�仨";
	char* &p1 = p;
	cout << p1 << endl;
}
//��������
void func(char* &tmp)
{
	char *p;
	p=(char*)malloc(64);
	memset(p, 0, 64);
	strcpy(p, "С��");
	tmp = p;//ʡ��*
}
//��������
void test02()
{
	char* mp = NULL;
	func(mp);//ʡ��&
	cout << mp << endl;
}

int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

