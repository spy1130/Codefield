#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:
	Maker(int id,int age)
	{
		mId = id;
		mAge = age;
	}
public:
	int mId;
	int mAge;
};

void test01()
{
	Maker m1(1, 18);
	Maker m2(m1);

	cout << "m1.id" << m1.mId << " m1.age:" << m1.mAge << endl;
	cout << "m2.id" << m2.mId << " m2.age:" << m2.mAge << endl;

	/*
	m2.mId=m1.mId;
	m2.mAge=m1.mAge;
	*/

}

class Student
{
public:
	Student(const char *name, int Age)
	{
		pName = (char*)malloc(strlen(name) + 1);
		strcpy(pName, name);
		age = Age;
	}
	//���
	Student(const Student &stu)
	{
		cout << "�Լ��Ŀ������캯��" << endl;
		//1.����ռ�
		pName = (char*)malloc(strlen(stu.pName) + 1);
		//2.��������
		strcpy(pName, stu.pName);
		age = stu.age;
	}

	~Student()
	{
		cout << "��������" <<endl;
		if (pName != NULL)
		{
			free(pName);
			pName = NULL;
		}
	}
public:
	char *pName;
	int age;
};

void test02()
{
	Student s1("С��", 18);
	Student s2(s1);

	cout << "s1 Name=" << s1.pName << " s1 age=" << s1.age << endl;
	cout << "s2 Name=" << s2.pName << " s2 age=" << s2.age << endl;
}
int main2()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

