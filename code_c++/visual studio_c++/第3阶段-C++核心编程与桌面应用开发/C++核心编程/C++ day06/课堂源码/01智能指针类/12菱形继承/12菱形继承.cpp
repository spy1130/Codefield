#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//үү
//����̳еĻ���������
class Animal
{
public:
	Animal()
	{
		
	}
public:
	
};
//����̳еķ���������μ̳��еĶ���������
//��̳�
class Sheep : virtual public Animal
{

};
//��̳�
class Camel :virtual public Animal
{
	/*
	class Camel     size(8):
	+---
	0      | {vbptr}
	+---
	+--- (virtual base Animal)
	4      | mA
	+---

	Camel::$vbtable@:
	0      | 0
	1      | 4 (Cameld(Camel+0)Animal)
	*/
};

class SheepCamel :public Sheep, public Camel
{
public:
	
};

void test()
{
	Camel c;
	cout << sizeof(c) << endl;
	

}

void test02()
{
	Sheep s;
	

	//1.&s;
	//2.(int*)&s;ǿתΪint*����
	//3.*(int*)&s;//��ȡָ���еĵ�ַ
	//4.(int *)*(int*)&s;//ָ�����׵�ַ
	//5.(int *)*(int*)&s+1;//ָ���˱�洢ƫ�����ĵ�ַ
	//6.*((int *)*(int*)&s+1)//�����ƫ����
	cout << *((int *)*(int*)&s + 1) << endl;

	//1.&s
	//2.(char*)&s;
	//3.(char*)&s+*((int *)*(int*)&s + 1)
	//4.������ת��ΪAnimalָ������
	//cout << ((Animal*)((char*)&s + *((int *)*(int*)&s + 1)))->mA << endl;


}
class A
{
public:
	int a;
};
class B :public A
{
public:

};
class C :public B
{

};
void test03()
{
	C c;
	c.a;
}
int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

