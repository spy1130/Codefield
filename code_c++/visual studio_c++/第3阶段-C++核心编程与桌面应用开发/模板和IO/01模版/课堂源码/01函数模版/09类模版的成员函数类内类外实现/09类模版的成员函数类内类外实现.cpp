#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>

template<class NameType, class AgeType>
class Maker
{
public:
	Maker(NameType name, AgeType age);
	/*{
		this->name = name;
		this->age = age;
	}*/

	void printMaker();
	/*{
		cout << "Name:" << this->name << " Age:" << this->age << endl;
	}*/
public:
	NameType name;
	AgeType age;
};

//��ģ��ĳ�Ա��������ʵ��
//Ҫд�ɺ���ģ��
template<class NameType, class AgeType>
Maker<NameType, AgeType>::Maker(NameType name, AgeType age)
{

	cout << "���캯��" << endl;
	this->name = name;
	this->age = age;
}


template<class NameType, class AgeType>
void Maker<NameType, AgeType>::printMaker()
{
	cout << "Name:" << this->name << " Age:" << this->age << endl;
}

int main()
{
	Maker<string, int> m("haha", 20);
	m.printMaker();
	system("pause");
	return EXIT_SUCCESS;
}

