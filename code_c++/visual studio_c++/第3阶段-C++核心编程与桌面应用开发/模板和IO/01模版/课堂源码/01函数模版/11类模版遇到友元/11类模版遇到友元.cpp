#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>


template<class NameType, class AgeType>
class Maker
{
	friend void printMaker(Maker<NameType, AgeType> &p)
	{
		cout << "����ʵ��" << p.name << " " << p.age << endl;
	}
public:
	Maker(NameType name, AgeType age)
	{
		this->name = name;
		this->age = age;
	}

	
private:
	NameType name;
	AgeType age;
};

void test01()
{
	Maker<string, int> m("���", 18);

	printMaker(m);
}

template<class NameType, class AgeType>
class Maker2;

//���߱�����������printMaker2��ʵ��
template<class NameType, class AgeType>
void printMaker2(Maker2<NameType, AgeType> &p);

template<class NameType, class AgeType>
class Maker2
{
	//1.�ں�������()֮�����<>��
	friend void printMaker2<>(Maker2<NameType, AgeType> &p);
	//2.��������֪��printMaker2������û��ʵ�֣���Ҫ֪�������Ľṹ
public:
	Maker2(NameType name, AgeType age)
	{
		this->name = name;
		this->age = age;
	}


private:
	NameType name;
	AgeType age;
};

//��Ԫ������ʵ��Ҫд�ɺ���ģ��
template<class NameType, class AgeType>
void printMaker2(Maker2<NameType, AgeType> &p)
{
	cout << "����ʵ�ֵ���Ԫ���� " << p.name << " " << p.age << endl;
}

void test02()
{
	Maker2<string, int> m("������", 18);

	printMaker2(m);
}

int main()
{
	
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

