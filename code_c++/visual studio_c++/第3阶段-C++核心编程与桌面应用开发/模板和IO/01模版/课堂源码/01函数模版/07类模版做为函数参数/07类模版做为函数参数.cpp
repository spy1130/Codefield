#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>
template<class NameType, class AgeType>
class Maker
{
public:
	Maker(NameType name, AgeType age)
	{
		this->name = name;
		this->age = age;
	}

	void printMaker()
	{
		cout << "Name:" << this->name << " Age:" << this->age << endl;
	}
public:
	NameType name;
	AgeType age;
};

//1.ָ���������������
void func(Maker<string,int> &m)
{
	m.printMaker();
}

//2.����ģ�滯�����ã�
template<class T1,class T2>
void func2(Maker<T1,T2> &m)
{
	m.printMaker();
}

//3.������ ģ�滯
template<class T>
void func3(T &m)
{
	m.printMaker();
}

void test()
{
	Maker<string, int> m1("���", 18);
	func(m1);

	func2(m1);//����ʱ
	/*//�ڶ��α���
	void func2(Maker<string,int> &m)
	{
		m.printMaker();
	}
	*/

	func3(m1);
}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

