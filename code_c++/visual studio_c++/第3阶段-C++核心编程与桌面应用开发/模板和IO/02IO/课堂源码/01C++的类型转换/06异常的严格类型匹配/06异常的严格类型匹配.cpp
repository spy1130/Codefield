#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
int func(int a, int b)
{
	if (b == 0)
	{
		//Maker m;
		//throw m;//�׳�һ��Maker���͵��쳣
		//throw 20.22;//�׳�һ��double���͵��쳣
		//throw 'c';
		throw 20.0f;

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
	catch (double s)
	{
		cout << "����һ��double���͵��쳣" << endl;
	}
	catch (char)
	{
		cout << "����һ��char���͵��쳣" << endl;
	}
	catch (...)//�����������͵��쳣
	{
		cout << "����һ���������͵��쳣" << endl;
	}


}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

