#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>

//�Զ�����������
class Maker
{
public:
	void Init()//��ʼ������
	{
		name = "�仨";
		age = 18;
	}
	//set get����
	void setN(string mName)
	{
		name = mName;
	}
	string getN()
	{
		return name;
	}
	void setA(int mAge)
	{
		if (mAge >= 0 && mAge <= 100)
		{
			age = mAge;
		}
	}
	int getA()
	{
		return age;
	}

	//��ӡ����
	void printMaker()
	{
		cout << "name:" << name << " age:" << age << endl;
	}
private:
	string name;
	int age;
};

int main()
{
	//Maker���࣬m�Ķ���
	Maker m;

	m.Init();
	m.printMaker();
	m.setN("����");
	m.setA(30);
	m.printMaker();
	cout << "name:" << m.getN() << " age:" << m.getA() << endl;;
	

	system("pause");
	return EXIT_SUCCESS;
}

