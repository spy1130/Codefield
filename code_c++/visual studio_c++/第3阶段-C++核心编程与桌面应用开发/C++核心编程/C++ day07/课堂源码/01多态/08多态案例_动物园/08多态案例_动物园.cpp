#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;
//1.��������Ļ���
class Animal
{
public:
	virtual void speak() = 0;
};

//2.��������
class Dog :public Animal
{
public:
	Dog(string name)
	{
		this->name = name;
	}
	virtual void speak()
	{
		cout << "С��" << name << "������" << endl;
	}
private:
	string name;
};

class Dark :public Animal
{
public:
	Dark(string name,string type)
	{
		this->name = name;
		this->type = type;
	}
	virtual void speak()
	{
		cout <<type<<"Ʒ�Ƶ�"<< "СѼ" << name << "�¸¸�" << endl;
	}
private:
	string name;
	string type;
};

class Tiger :public Animal
{
public:
	Tiger(string name,int age)
	{
		this->name = name;
		this->age = age;
	}
	virtual void speak()
	{
		cout << age << "��" << "�ϻ�" << name << "���" << endl;
	}
private:
	string name;
	int age;

};

//3.��������԰
class Zoo
{
public:
	Zoo()
	{
		mCapacity = 1024;
		mSize = 0;
		//����ռ䣬�洢Animal*�Ŀռ䣬ָ������
		this->p = new Animal*[mCapacity];
	}

	//���Ӷ���
	int AddAnimal(Animal* animal)
	{
		if (mCapacity == mSize)
		{
			return -1;
		}
		//��ָ��洢��ָ������
		this->p[mSize] = animal;
		mSize++;

		return 0;
	}

	void StartSpeak()
	{
		for (int i = 0; i < mSize; i++)
		{
			p[i]->speak();
		}
	}

	//��������
	~Zoo()
	{
		//���ͷ�ָ��������ָ��ָ��Ķ����ռ�
		for (int i = 0; i < mSize; i++)
		{
			if (p[i] != NULL)
			{
				delete p[i];
				p[i] = NULL;
			}
		}

		//�ͷ�ָ������
		delete[] p;
		p = NULL;
	}


private:
	Animal* *p;
	int mCapacity;//����
	int mSize;

};

class Cat :public Animal
{
public:
	Cat(string name)
	{
		this->name = name;
	}
	virtual void speak()
	{
		cout << "Сè" << name << "����" << endl;
	}
private:
	string name;
};

void test()
{
	//��������԰
	Zoo *zoo = new Zoo;

	//��Ӷ���
	zoo->AddAnimal(new Dog("����"));
	zoo->AddAnimal(new Dark("��Ѽ", "�ܺ�Ѽ"));
	zoo->AddAnimal(new Tiger("����", 35));

	zoo->AddAnimal(new Cat("С��"));
	zoo->StartSpeak();

	//���˶���԰
	delete zoo;
}
//��ѧԱ����
//����ѧԱ,ѧԱ���ԣ����������䣬�Ա��س�
//�����༶
//����


int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

