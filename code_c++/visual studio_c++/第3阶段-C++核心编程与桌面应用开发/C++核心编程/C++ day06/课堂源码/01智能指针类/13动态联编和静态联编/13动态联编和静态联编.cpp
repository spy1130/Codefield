#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Animal
{
public:
	//�麯��
	
	virtual void speak()
	{
		cout << "Animal speak " << endl;
	}
};

class Dog :public Animal
{
public:
	 void speak()
	{
		cout << "Dog speak " << endl;
	}
};

class Dog1 :public Animal
{
public:
	void speak()
	{
		cout << "Dog1 speak " << endl;
	}
};

class Dog2 :public Animal
{
public:
	void speak()
	{
		cout << "���Ӽ��� " << endl;
	}
};

//ҵ���
void doLogin(Animal *animal)//Animal *animal=dog;
{
	animal->speak();
}
void test01()
{
	Animal an;
	an.speak();//�ڱ���׶ξ�ȷ���˵���speak�������
}
void test02()
{
	Dog *dog = new Dog;
	doLogin(dog);

	Dog1 *dog1 = new Dog1;
	doLogin(dog1);

	Dog2 *dog2 = new Dog2;
	doLogin(dog2);
}

int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

