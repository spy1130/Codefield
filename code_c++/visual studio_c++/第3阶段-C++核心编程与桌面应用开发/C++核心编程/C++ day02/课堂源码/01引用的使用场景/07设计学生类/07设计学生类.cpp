#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>


class Student
{
public://����
	//��������
	 void setName(string Name)//��Ա��������Ա����
	{
		name = Name;
	}
	//����ID
	void setId(int Id)
	{
		id = Id;
	}

	void myprint()
	{
		cout << "����:" << name << "\nѧ��:" << id << endl;
	}
private://˽��Ȩ��
	string name;//��Ա����
	int id;//��Ա����

};

int main()
{
	Student s;
	s.setName("������");
	s.setId(1);
	s.myprint();

	
	system("pause");
	return EXIT_SUCCESS;
}

