#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Maker
{
public:
	Maker()
	{
		a = 0;
	}

	void SetA(int val)
	{
		a = val;
	}
	//û�з���ֵ��Ҳû��void
	operator bool()
	{
		if (a <= 0)
		{
			return false;
		}

		return true;
	}

	bool operator!()
	{
		if (a <= 0)
		{
			return true;
		}

		return false;
	}

	operator int()
	{
		return 10;
	}
public:
	int a;
};

void test()
{
	Maker m;
	m.SetA(10);
	if (m)
	{
		cout << "��" << endl;
	}
	else
	{
		cout << "��" << endl;
	}

	if (!m)
	{
		cout << "��" << endl;
	}
	else
	{
		cout << "��" << endl;
	}

	int b = (int)m;
	cout << b << endl;
}
int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

