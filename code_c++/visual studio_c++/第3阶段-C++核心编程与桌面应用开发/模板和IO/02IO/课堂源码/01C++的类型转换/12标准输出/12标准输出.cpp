#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<iomanip>
using namespace std;

void test01()
{
	cout.put('c');
	//��ʽ���
	cout.put('a').put('b').put('c');
}

void test02()
{
	char buf[] = "hello world";

	cout.write(buf, strlen(buf));
}
//ͨ������Ա����ʵ�ָ�ʽ�������
void test03()
{
	int num = 99;
	cout.width(20);//���ÿ��
	cout.fill('*');//���
	cout.setf(ios::left);//�����������
	cout.unsetf(ios::dec);//ж��ʮ����
	cout.setf(ios::hex);//��װʮ������
	cout.setf(ios::showbase);//��ʾ����
	cout.unsetf(ios::hex);//ж��ʮ������
	cout.setf(ios::oct);//��װ�˽���
	cout << num << endl;
}
//ͨ�����Ʒ�����ʽ�����������ͷ�ļ�iomanip
void test04()
{
	int num = 99;
	cout << setw(20);//���ÿ��
	cout << setfill('~');
	cout << setiosflags(ios::showbase);//��ʾ����
	cout << setiosflags(ios::left);//�����������
	cout << hex;//ʮ������
	cout << oct;//�˽���
	cout << dec;//ʮ����
	cout << num << endl;
}

void test05()
{
	double d = 20.22;
	cout << setiosflags(ios::fixed);//������ʾ������
	cout << setprecision(10);//��ʾС�����10λ
	cout << d << endl;
}
int main()
{
	test05();

	system("pause");
	return EXIT_SUCCESS;
}

