#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Maker//�������
{
public:
	int a;//��Ա���ԣ���Ա������
	void func()//��Ա��������Ա������
	{
		cout << "func" << endl;
	}
};



int main()
{
	Maker m;//m���Ƕ���

	system("pause");
	return EXIT_SUCCESS;
}

