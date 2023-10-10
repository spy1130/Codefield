#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

class Manito
{
public:
	virtual void show() = 0;
};

class Maker :public Manito
{
public:
	Maker(string name, int age, string sex, string Skill)
	{
		this->name = name;
		this->age = age;
		this->sex = sex;
		this->Skill = Skill;
	}
	virtual void show()
	{
		cout << name << "չ��" << Skill << endl;
	}

private:
	string name;
	int age;
	string sex;
	string Skill;

};

class Maker2 :public Manito
{
public:
	Maker2(string name, int age, string sex, string Skill)
	{
		this->name = name;
		this->age = age;
		this->sex = sex;
		this->Skill = Skill;
	}
	virtual void show()
	{
		cout << name << "չ��" << Skill << endl;
	}

private:
	string name;
	int age;
	string sex;
	string Skill;

};

//�༶
class MyClass
{
public:
	MyClass()
	{
		mCapacity = 50;
		mSize = 0;
	
		this->p = new Manito*[mCapacity];
	}

	//������Ա
	void AddManito(Manito* ma)
	{
		if (mSize == mCapacity)
		{
			return;
		}

		this->p[mSize] = ma;
		mSize++;
	}

	//�㼼��
	void StartShow()
	{
		for (int i = 0; i < mSize; i++)
		{
			p[i]->show();
		}
	}


	~MyClass()
	{
		for (int i = 0; i < mSize; i++)
		{
			if (p[i] != NULL)
			{
				delete p[i];
				p[i] = NULL;
			}

		}
		
		delete[] p;
		p = NULL;
	}



private:
	Manito* *p;
	int mCapacity;
	int mSize;
};

void test()
{
	MyClass *my = new MyClass;

	my->AddManito(new Maker("Maker", 18, "��", "����"));
	my->AddManito(new Maker2("Maker2", 16, "Ů", "����"));
	my->StartShow();

	delete my;

	
}
int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

