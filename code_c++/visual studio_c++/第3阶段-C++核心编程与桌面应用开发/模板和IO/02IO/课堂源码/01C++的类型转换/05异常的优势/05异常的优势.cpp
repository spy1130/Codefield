#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Maker
{
public:
	void printMaker()
	{
		cout << "��������Ϊ0" << endl;
	}
};

int func(int a, int b)
{
	if (b == 0)
	{
		//Maker m;
		//throw m;//�׳�һ��Maker���͵��쳣
		throw 20.22;//�׳�һ��double���͵��쳣
		
	}

	return a / b;
}
void test()
{
	int a = 10;
	int b = 0;

	try
	{
		func(a, b);
		
	}
	catch (int)
	{
		cout << "����һ��int���͵��쳣" << endl;
	}
	catch (Maker maker)
	{
		cout << "����һ��Maker���͵��쳣" << endl;
		maker.printMaker();
	}
	catch (double s)
	{
		//���봦���쳣�����������׳����׸����ñ������ĺ���
		throw;
	}


}

int main()
{
	try
	{
		test();
	}
	catch (double d)
	{
		cout << "����һ��double���͵��쳣" << endl;
	}




	system("pause");
	return EXIT_SUCCESS;
}

