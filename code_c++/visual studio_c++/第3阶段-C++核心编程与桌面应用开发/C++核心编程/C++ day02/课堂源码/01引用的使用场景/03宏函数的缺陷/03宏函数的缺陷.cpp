#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#define ADD(x,y) x+y
//����ͨ����ǰ�����inline��������������Ϊ��������
//ע�⣺��inline���ܳ�Ϊ�������������ܲ���Ϊ��������
inline int Add(int x, int y)
{
	return x + y;
}

void test()
{
	//10+20*2
	int ref = ADD(10, 20) * 2;
	cout << "ref=" << ref << endl;

	int ref2 = Add(10, 20) * 2;
	cout << "ref2=" << ref2 << endl;
}

#define COMAPD(x,y) ((x)<(y)?(x):(y))
inline int func(int x, int y)
{
	return x < y ? x : y;
}
void test02()
{
	int a = 1;
	int b = 3;
	//                        ((++a)<(b)?(++a):(b))      
	//cout << "COMAPD(x,y)=" << COMAPD(++a, b) << endl;//3
	cout << "func=" << func(++a, b) << endl;//2
}

int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

