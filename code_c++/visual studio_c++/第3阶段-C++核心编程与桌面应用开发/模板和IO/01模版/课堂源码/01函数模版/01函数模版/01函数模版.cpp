#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

void mySwapint(int &a, int &b)
{
	int tmp = a;
	a = b;
	b = tmp;
}

void mySwapdouble(double &a, double &b)
{
	double tmp = a;
	a = b;
	b = tmp;
}

void test01()
{
	int a = 10;
	int b = 20;
	//mySwap(a, b);
}
//T�����͵��������ͣ�����ֻ��дT��
template<class T>//�ñ�����������仰��������ŵĺ�������T��Ҫ����
void mySwap(T &a,T &b)//��һ�α���
{
	T tmp = a;
	a= b;
	b = tmp;
}
//�����������庯��ģ��
template<typename T>
void func2(T a,T b)
{

}

template<class T>
void mySwap2()
{

}

//ʹ�ú���ģ��
void test02()
{
	int a = 10;
	int b = 20;

	//1.�����������ʵ�����Զ��Ƶ���������
	mySwap(a,b);//�������ں���ģ�汻����ʱ�����еڶ��α���
	/*
	void mySwap(int &a,int &b)
	{
		int tmp = a;
		a= b;
		b = tmp;
	}
	*/
	cout << "a=" << a << endl;
	cout << "b=" << b << endl;
	char c = 'c';
	//mySwap(a, c);err,��������Ҫһ��

	//2.��ʾ��ָ������
	mySwap<int>(a, b);//<>�ò����б���߱�������ֻ��int��
	//mySwap<double>(a, b);//ע�⣺ָ�����ͣ�����ʱ���ܲ�һ��

	mySwap<>(a,b);
	//mySwap2<>();//����ʱ�������ñ�����֪������T������ʲô����

	/*
	template<class T>//�ñ�����������仰��������ŵĺ�������T��Ҫ����
	void mySwap(T &a,T &b)//��һ�α���
	{
		T tmp = a;
		a= b;
		b = tmp;
	}
	*/
	int a1 = 10;
	double b1 = 20;
	//mySwap<int>(a1, b1);err
}

template<class T>
T func(T a, T b)
{
	return a + b;
}
void test03()
{
	int a = 10;
	double b = 20.2;
	//���ʹ�ò����б�ָ���������ͣ���ôʵ���п�����ʽת����
	//���ת���ɹ����͵��ã�ת�����ɹ��ͱ���
	cout << func<int>(a,b) << endl;
}

//����
template<class T>//�ñ�����������仰��������ŵĺ�������T��Ҫ����
void mySwap3(T &a, T &b)//��һ�α���
{
	T tmp = a;
	a = b;
	b = tmp;
}

template<class T>
void mySort(T arr[],int len)
{
	for (int i = 0; i < len; i++)
	{
		int max = i;
		for (int j = i + 1; j < len; j++)
		{
			if (arr[max] < arr[j])
			{
				max = j;
			}
		}

		if (max != i)
		{
			//���н���
			mySwap3(arr[max], arr[i]);
		}
	}
}

template<class T>
void myPrint(T arr[], int len)
{
	for (int i = 0; i < len; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

void test04()
{
	char arrchar[] = "hello world";
	int len = sizeof(arrchar) / sizeof(char);
	mySort(arrchar, len);
	myPrint(arrchar, len);

	int arrInt[] = { 3, 8, 4, 2, 1, 9 };
	len = sizeof(arrInt) / sizeof(int);
	mySort(arrInt, len);
	myPrint(arrInt, len);
}
int main()
{
	test04();
	system("pause");
	return EXIT_SUCCESS;
}

