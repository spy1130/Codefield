#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
class Maker
{
public:
	Maker(){}
	/*Maker(const char*name,int age)
	{
		this->age = age;
		strcpy(this->name, name);
	}*/
	Maker(string name, int age)
	{
		this->name = name;
		this->age = age;
	}
public:
	string name;
	//string������һ����Աָ��char*,��ָ��ָ��洢�ַ����Ŀռ�
	//�����ǰ�string������ݴ洢���ļ��У��ٶ�����ʱ�����ܱ�ָ֤����Ч

	//string ���ٵĿռ��������16���ֽھ��ڶ�����С�ھ���ջ��
	int age;
};

//д�ļ�
void test01()
{
	Maker m1("���",18);
	Maker m2("������", 22);

	ofstream ofs;
	ofs.open("test.txt", ios::out | ios::trunc | ios::binary);
	if (!ofs.is_open())
	{
		cout << "��ʧ��" << endl;
	}
	//д
	ofs.write((const char *)&m1, sizeof(Maker));
	ofs.write((const char *)&m2, sizeof(Maker));

	ofs.close();
}
//���ļ�
void test02()
{
	ifstream ifs;
	ifs.open("test.txt", ios::in | ios::binary);
	if (!ifs.is_open())
	{
		cout << "��ʧ��" << endl;
	}

	//��
	Maker m1;
	Maker m2;

	ifs.read((char*)&m1, sizeof(Maker));
	ifs.read((char*)&m2, sizeof(Maker));

	cout << "Name:" << m1.name << " Age:" << m1.age << endl;
	cout << "Name:" << m2.name << " Age:" << m2.age << endl;

	ifs.close();
}

int main()
{
	test01();
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

