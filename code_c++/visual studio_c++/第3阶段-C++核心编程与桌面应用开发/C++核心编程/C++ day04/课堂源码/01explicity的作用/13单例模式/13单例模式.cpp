#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//����ģʽ��һ����ֻ��ʵ����һ������
class Maker
{
	//1.�ѹ��캯��˽�л�
private:
	Maker()
	{
	}
	Maker(const Maker &m)
	{
		
	}
public:
	//��̬��Ա���������ʾ�̬��Ա����
	//4.��ָ���Ȩ������Ϊ˽�У�Ȼ���ṩһ����̬��Ա�����������ȡ���ָ��
	static Maker* getMaker()
	{
		return pMaker;
	}

private:
	//2.����һ�����ڵľ�̬��Աָ��
	static Maker *pMaker;
};
//3.�������ʼ��ʱ��newһ������
Maker *Maker::pMaker = new Maker;//�������new����Ϊ��Maker::�����򣬱�������ʱ��������������

void test()
{
	//Maker m;
	Maker *m = Maker::getMaker();
	Maker *m2 = Maker::getMaker();
	cout << "m=" << m << endl;
	cout << "m2=" << m2 << endl;

	//Maker m3 = *m;//���ÿ�������,���������¶�������Ҫ�ѿ�������Ҳ˽�л�
	//cout << "m3=" << &m3 << endl;
	
}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

