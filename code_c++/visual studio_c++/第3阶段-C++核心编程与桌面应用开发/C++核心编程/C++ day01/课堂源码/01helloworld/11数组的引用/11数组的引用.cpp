#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


int main()
{
	int arr[] = { 1, 2, 3, 4, 5 };
	//��һ�ַ���
	//1.������������
	typedef int(MY_ARR)[5];//��������
	//2.��������
	MY_ARR &arref = arr;//�������ã�int &b=a;

	//�ڶ��ַ���
	//ֱ�Ӷ�������
	int(&arref2)[5] = arr;// int &b=a

	//�����ַ���
	typedef int(&MY_ARR3)[5];//����������������
	MY_ARR3 arref3 = arr;

	for (int i = 0; i < 5; i++)
	{
		cout << arref[i] << endl;
	}
	cout << endl;
	for (int i = 0; i < 5; i++)
	{
		arref2[i] = 100 + i;
		cout << arref2[i] << endl;
	}

	system("pause");
	return EXIT_SUCCESS;
}

