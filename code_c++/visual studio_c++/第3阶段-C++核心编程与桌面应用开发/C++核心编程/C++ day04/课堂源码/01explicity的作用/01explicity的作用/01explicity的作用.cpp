#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Maker
{
public:
	//explicitֻ�ܷ��ڹ��캯��ǰ�棬���캯��ֻ��һ������������������Ĭ��ֵʱ
	explicit Maker(int n)//��ֹ�������Ż�Maker m=10;���ָ�ʽ
	{

	}
};

int main()
{
	Maker m = 10;
	system("pause");
	return EXIT_SUCCESS;
}

