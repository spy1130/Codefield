#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
/*
cin.get() //һ��ֻ�ܶ�ȡһ���ַ�
cin.get(һ������) //��һ���ַ�
cin.get(��������) //���Զ��ַ���
cin.getline()//ȡһ�У����з�����
cin.ignore()//����
cin.peek()//͵��
cin.putback()//�Ż�

*/
void test01()
{
	//����������as
	//char c = cin.get();//a
	//cout << c << endl;

	//c = cin.get();//s
	//cout << c << endl;

	//c = cin.get();//����
	//cout << c << endl;

	//c = cin.get();//����
	//cout << c << endl;

	//char c1 = '0';
	//cin.get(c1);

	//cout << c1 << endl;

	char ch1, ch2, ch3, ch4;
	cin.get(ch1).get(ch2).get(ch3).get(ch4);
	cout << ch1 << ch2 << ch3 << ch4 << endl;

}
void test02()
{
	char buf[1024] = { 0 };
	//cin.get(buf, 1024);//��ȡ������ʱ�����в�����
	//char c = cin.get();
	//if (c == '\n')
	//{
	//	cout << "���л��ڻ�����" << endl;
	//}
	//cout << buf << endl;

	cin.getline(buf, 1024);//��ȡ�����һ�����ݣ����з�����
	char c = cin.get();//����
	if (c == '\n')
	{
		cout << "���л��ڻ�����" << endl;
	}
	cout << buf << endl;
}
//���ԣ�������N����ʾ����N���ַ������û�в�������ʾ����һ���ַ�
void test03()
{
	/*cin.ignore();
	char c = cin.get();
	cout << c << endl;*/

	cin.ignore(3);
	char c1 = cin.get();
	cout << c1 << endl;
}

void test04()
{
	char c = cin.peek();//͵����һ���ַ�
	cout << c << endl;

	char c2 = cin.get();
	cout << c2 << endl;
}

void test05()
{
	char c = cin.get();
	cout << c << endl;
	cin.putback(c);//�Ż�

	char buf[1024] = { 0 };
	cin.getline(buf, 1024);
	cout << buf << endl;
}

//�ж��û���������ַ�����������
void test06()
{
	cout << "������һ���ַ���������" << endl;

	char c=cin.peek();

	if (c >= '0'&&c <= '9')
	{
		int num;
		cin >> num;
		cout << "�����������:" << num << endl;
	}
	else
	{
		char buf[1024] = { 0 };
		cin >> buf;
		cout << "������ַ�����:" <<buf << endl;
	}
}

//����һ��0��10�����֣�ֱ��������ȷΪֹ
void test07()
{
	int num;

	while (1)
	{
		cin >> num;
		if (num >= 0 && num <= 10)
		{
			cout << "������ȷ" << endl;
			break;
		}
		cout << "�������룺" << endl;
		//���ñ�־λ
		cin.clear();
		//��ջ�����
		//cin.sync();
		//2015
		char buf[1024] = { 0 };
		cin.getline(buf, 1024);

		//��ӡ��־λ
		cout << cin.fail() << endl;

	}
}

int main()
{
	test07();

	system("pause");
	return EXIT_SUCCESS;
}

