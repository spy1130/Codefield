#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>

class Maker
{
public:
	//д
	void setName(string Name)
	{
		name = Name;
	}
	//��
	string getName()
	{
		return name;
	}
	//д
	void setAge(int Age)
	{
		//���Ա������ԵĺϷ���
		if (Age > 0 && Age < 120)
		{
			age = Age;
		}
		
	}
	//��
	int getId()
	{
		return id;
	}
private:
	string name;
	int age;
	int id;

};






int main()
{
	Maker m;
	m.setAge(1000);
	system("pause");
	return EXIT_SUCCESS;
}

