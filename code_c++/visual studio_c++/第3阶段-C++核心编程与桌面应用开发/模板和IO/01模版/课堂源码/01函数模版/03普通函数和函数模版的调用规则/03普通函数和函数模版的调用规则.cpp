#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//��ͨ����
void myPlus(int a, int b)
{
	cout << "��ͨ����" << endl;
}

template<class T>
void myPlus(T a, T b)
{
	cout << "����ģ��" << endl;
}

template<class T>
void myPlus(T a, T b, T c)
{
	cout << "����ģ�� T c" << endl;

}
//1.����ģ�����ͨ������������
void test()
{
	int a = 10;
	int b = 20;
	//2.�����ͨ�����ͺ���ģ�涼����ʵ�ֵĹ��ܣ���ͨ�������ȵ���
	myPlus(a, b);

	//3.����ʹ��<>�ղ����б�ǿ�Ƶ��ú���ģ��
	myPlus<>(a, b);

	//4.����ģ��֮��Ҳ���Խ�������


	//5.�������ģ����Բ������õ�ƥ�䣬��ô����ʹ�ú���ģ��
	char c1 = 'a';
	char c2 = 'b';
	myPlus(c1, c2);
}


int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

