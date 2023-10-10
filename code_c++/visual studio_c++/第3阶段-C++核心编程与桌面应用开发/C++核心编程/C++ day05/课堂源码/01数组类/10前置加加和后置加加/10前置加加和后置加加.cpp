#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

void test()
{
	int a = 1;
	cout << ++a << endl;
	cout << a++ << endl;
	cout << a << endl;

	++(++a);
}

class Maker
{
	friend ostream &operator<<(ostream &out, Maker &m);
public:
	Maker(int a)
	{
		this->a = a;
	}
	//����ǰ�üӼ�
	Maker &operator++()
	{
		++this->a;
		return *this;
	}

	//���üӼ�,
	Maker operator++(int)//ռλ������������int
	{
		//���üӼӣ��ȷ��أ���Ӽ�
		Maker tmp(*this);//1.*this�����ֵa�ǵ���2,��������
		++this->a;//��������a��3
		return tmp;
	}
private:
	int a;
};

ostream &operator<<(ostream &out, Maker &m)
{
	out << m.a << endl;
	return out;
}

void test02()
{
	Maker m1(1);
	cout << m1 << endl;//1
	cout << ++m1 << endl;//2
	//++(++m1);
	cout << m1++ << endl;//2  ���ﷵ�صĿ�����tmp����
	cout << m1 << endl;//3 ���ﷵ�ص���++this->a�Ķ���

	//ͬ�������£�����ʹ��ǰ�üӼӣ�����Ҫ�����µĶ���͵��ÿ�������

}

int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

