#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>

//��װ��1,�����Ժͷ����ŵ�����,2.����Щ���ݸ���Ȩ��
class Maker
{
public://����Ȩ��
	void set(string Name,int Id)
	{
		id = Id;
		name = Name;
	}
	void printMaker()
	{
		cout << "id=" << id << " name=" << name << endl;
	}
	void get()
	{
		cout << "Maker a=" << a << endl;
	}
private://˽��
	int id;
	string name;

protected://����
	int a;
};
//�̳У����м̳�
class Son :public Maker//���� :�̳з�ʽ ����
{
public:
	void func()
	{
		//�������a�ǴӸ��ิ�ƹ�����
		a = 20;//��������ڿ��Է�������ı���Ȩ�޵ĳ�Ա
		
	}
	void getS()
	{
		cout << "Son a=" << a << endl;
	}

};

void test01()
{
	Maker m;
	Son s;
	s.func();
	m.get();
	s.getS();

}
//���ⲻ�ܷ���˽��Ȩ�޵ĳ�Ա
//������Է��ʹ���Ȩ�޵ĳ�Ա
//���ⲻ�ܷ��ʱ���Ȩ�޵ĳ�Ա
//��������ڿ��Է�������ı���Ȩ�޵ĳ�Ա
//������û��Ȩ��֮��

void test()
{
	Maker m;
	m.set("С��",1);
	m.printMaker();
	

}

int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

