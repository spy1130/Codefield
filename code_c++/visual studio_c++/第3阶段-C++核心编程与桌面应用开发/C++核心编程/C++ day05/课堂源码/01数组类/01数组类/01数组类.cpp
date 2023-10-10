#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include"MyArray.h"

void printMyArray(MyArray &arr)
{
	for (int i = 0; i < arr.Size(); i++)
	{
		cout << arr.Get(i) << " ";
	}
	cout << endl;
}

void test01()
{
	MyArray arr(20, 1);
	printMyArray(arr);
	//�޸������е�ֵ
	for (int i = 0; i < arr.Size(); i++)
	{
		arr.Get(i) = i + 100;
	}
	printMyArray(arr);

	//ָ��λ���޸�ֵ
	arr.Set(2, 0);
	printMyArray(arr);

	//�����Ƿ���ǳ����
	MyArray arr2 = arr;
	printMyArray(arr2);
}

void test02()
{
	MyArray arr;

	//���Ԫ��
	for (int i = 0; i < 10; i++)
	{
		//β��
		arr.PushBack(i + 10);
	}
	for (int i = 0; i < 9; i++)
	{
		//ͷ��
		arr.PushFront(i + 20);
	}

	//ָ��λ�ò���
	arr.Insert(10, 100);

	printMyArray(arr);

	arr.PopBack();
	arr.PopFront();
	printMyArray(arr);

}


int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

