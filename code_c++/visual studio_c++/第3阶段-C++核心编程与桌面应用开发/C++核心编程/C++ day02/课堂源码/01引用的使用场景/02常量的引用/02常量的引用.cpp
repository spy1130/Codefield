#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


int main()
{
	//��ͨ����
	int a = 10;
	int &ref = a;
	ref = 20;

	//int &ret2 = 10;//���ܸ�������ȡ���� err
	const int &ref3 = 10;//���Ը�const���ε����ø���������
	//const���η����ε����õ�ԭ��
	//�������������Ĵ����Ϊ��int tmp=10;const int &ref3=tmp;
	//ref3 = 200;err

	//bool����
	//bool���Ͷ���ı���ֻ������ֵ��true��false,��ͼ٣�1��0
	bool is = 0;//ע�⣺is��ֵ��0���⣬������
	if (is)
	{
		cout << "��" << endl;
	}
	else
	{
		cout << "��" << endl;
	}
	system("pause");
	return EXIT_SUCCESS;
}

