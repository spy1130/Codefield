#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker
{
public:
	Maker()
	{
		id = 0;
		age = 0;
	}
	Maker(int id, int age)
	{
		this->id = id;
		this->age = age;
	}
public:
	int id;
	int age;
};

void test()
{
	Maker m1(10, 20);
	Maker m2;

	m2 = m1;//��ֵ����
	//Ĭ�ϵĸ�ֵ��������غ��������˼򵥵ĸ�ֵ����
	cout << m2.id << " " << m2.age << endl;
}

class Student
{
public:
	Student(const char *name)
	{
		pName = new char[strlen(name) + 1];
		strcpy(pName, name);
	}
	//��ֹǳ����
	Student(const Student &stu)
	{
		cout << "��������" << endl;
		pName = new char[strlen(stu.pName) + 1];
		strcpy(pName, stu.pName);
	}
	//��д��ֵ��������غ���
	Student &operator=(const Student &stu)
	{
		//1.����ȷ��this->pNameָ��Ŀռ��Ƿ���װ��stu�е����ݣ��������ͷ�this->pNameָ��Ŀռ�
		if (this->pName != NULL)
		{
			delete[] this->pName;
			this->pName = NULL;
		}

		//2.��������ռ䣬��С��stu����
		this->pName = new char[strlen(stu.pName) + 1];
		//3.��������
		strcpy(this->pName, stu.pName);

		//4.���ض�����
		return *this;
	}	

	~Student()
	{
		if (pName != NULL)
		{
			delete[] pName;
			pName = NULL;
		}
	}

	void printStudent()
	{
		cout << "Name:" << pName << endl;
	}
public:
	char *pName;
};

void test02()
{
	Student s1("���");
	Student s2("С��");

	s1.printStudent();
	s2.printStudent();

	s1 = s2;//��ֵ����

	s1.printStudent();
	s2.printStudent();
	
	//�������㲻�����
	//s1 = s2 = s3;
}
//��ֵ�����������ΪʲôҪ��������
void test03()
{
	Student s1("a");
	Student s2("b");
	Student s3("c");

	s1 = s2 = s3;//s3��ֵs2,s2��ֵ��s1

	cout << &(s2 = s3) << endl;
	cout << &s2 << endl;
	//������ص���ֵ��s2=s3������ʽ�����һ���µĶ���
	//s1=s2=s3,��ֵ�����������Ԣ�⣬��s3��ֵs2,s2��ֵ��s1
	//Ҳ����˵s2=s3������ʽҪ����s2�����������Ҫ��������

}

int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

