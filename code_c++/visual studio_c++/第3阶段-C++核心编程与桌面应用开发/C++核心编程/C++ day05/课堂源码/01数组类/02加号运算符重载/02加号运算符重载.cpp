#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:
	Maker(int id, int age)
	{
		this->id = id;
		this->age = age;
	}
	//д�ɳ�Ա��������ôֻ��Ҫһ����������������ǼӺŵ��ұ�
	Maker operator+(Maker &m2)
	{
		Maker temp(this->id + m2.id, this->age + m2.age);

		return temp;
	}
public:
	int id;
	int age;
};

//ȫ�ַ�ʽ //2.�����������������
//Maker operator+(Maker &p1,Maker &p2)//3.�������������Ƿ��Ӧ����һ�������Ǽӵ���ߣ��ڶ������ǼӺŵ��ұ�
//{
//	Maker temp(p1.id + p2.id, p1.age + p2.age);
//
//	return temp;
//}

void test01()
{
	Maker m1(1, 20);
	Maker m2(2, 22);
	//+Ҳ��˫Ŀ�����
	Maker m3=m1 + m2;//1.��������������������ӣ���ô��������ȥ����û�н�operator+�ĺ���

	cout << "id:" << m3.id << " age:" << m3.age << endl;

	//������
	Maker m4 = m1 + m2 + m3;
	cout << "id:" << m4.id << " age:" << m4.age << endl;
}

int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

