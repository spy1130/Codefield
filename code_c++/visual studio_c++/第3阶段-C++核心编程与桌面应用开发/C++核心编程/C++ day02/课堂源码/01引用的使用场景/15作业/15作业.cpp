#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<string>

class Car
{
public:
	void init()
	{
		type = "BMW";
		Size = 5;
		Color = "��ɫ";
	}
	void printCar()
	{
		cout << "Ʒ��:" << type << " ��С:" << Size << " ��ɫ:" << Color << endl;
	}
private:
	string type;
	int Size;
	string Color;

};

class SonCar :public Car
{
public:
	//д
	void setType(string Type)
	{
		type = Type;

	}
	void setMyT(int t)
	{
		MyT = t;
	}
	void setMynum(int num)
	{
		Mynum = num;
	}

	//��
	string getT()
	{
		return type;
	}
	int getMyT()
	{
		return MyT;
	}
	int getMynum()
	{
		return Mynum;
	}
	void printSonCar()
	{
		cout << "Ʒ��:" << type << " ��λ:" << MyT << " ���Ӹ���:" << Mynum << endl;
		Car::init();
		Car::printCar();
	}
private:
	string type;
	int MyT;
	int Mynum;
};

void test01()
{
	Car c;
	c.init();
	c.printCar();

}

void test02()
{
	SonCar sc;
	sc.setType("����");
	sc.setMyT(20);
	sc.setMynum(8);

	sc.printSonCar();
}
int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

