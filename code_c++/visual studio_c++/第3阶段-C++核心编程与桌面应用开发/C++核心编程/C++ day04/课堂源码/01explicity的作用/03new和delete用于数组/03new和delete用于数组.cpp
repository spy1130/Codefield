#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//1.new�����������͵�����
void test01()
{
	//��������������͵�����
	int *pInt = new int[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};//���Ƽ�
	for (int i = 0; i < 10; i++)
	{
		pInt[i] = i + 1;
	}

	for (int i = 0; i < 10; i++)
	{
		cout << pInt[i] << " ";
	}

	cout << endl;

	char *pChar = new char[64];
	memset(pChar, 0, 64);
	strcpy(pChar, "С��");
	cout << pChar << endl;

	//ע�⣺���newʱ�������ţ���ôdeleteʱҲҪ��������
	delete[] pInt;
	delete[] pChar;

}

//2.new������������
class Maker
{
public:
	Maker()
	{
		cout << "���캯��" << endl;
	}

	Maker(int a)
	{
		cout << "�вι��캯��" << endl;
	}
	~Maker()
	{
		cout << "��������" << endl;
	}
};

void test02()
{
	Maker *ms = new Maker[2];//�����޲ι���

	delete[] ms;

	//�󲿷ֱ�������֧������д����(�ۺϳ�ʼ����
	//Maker *ms2 = new Maker[2]{Maker(10), Maker(20)};
}

//3.delete void*���ܳ���,������ö������������
void test03()
{
	void *m = new Maker;

	//�����void*����new�Ķ�����ôdeleteʱ���������������
	delete m;
	//�ڱ���׶Σ���ô��������ȷ�����˺����ĵ��õ�ַ��
	//C++����������ʶvoid*,��֪��void*ָ���Ǹ����������Բ��������������
	//���ֱ��뷽ʽ�о�̬����
}
//4.C��C++��������ͷŶ����ռ䲻Ҫ����
void test04()
{
	Maker *m = new Maker;

	free(m);
}

int main()
{
	test04();
	system("pause");
	return EXIT_SUCCESS;
}

