#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//��ͨ����
int myPlus(int a, int b)
{
	return a + b;
}

template<class T>
int myPlus2(T a, T b)
{
	return a + b;
}

void test()
{
	int a = 10;
	int b = 20;
	char c = 'a';
	//��ͨ�������Խ�����ʽת��
	myPlus(a, c);
	//����ģ�治��ֱ�ӵĽ�����ʽת��
	//myPlus2(a, c);
	myPlus2<int>(a, c);//���Ҫ��������ת����������ϲ����б�
}
int main()
{
	test();

	system("pause");
	return EXIT_SUCCESS;
}

