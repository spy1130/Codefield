#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void insertSort(int arr[], int len)
{
	for (int i = 1; i < len;i++)
	{
		if (arr[i] < arr[i-1])
		{
			int temp = arr[i]; //�����ݱ���ס
			int  j = i - 1;
			for (; j >= 0 && temp < arr[j];j--)
			{
				//���ݺ���
				arr[j + 1] = arr[j];
			}

			//��j+1 ��λ������Ϊ temp
			arr[j + 1] = temp;
		}
	}

}

void test01()
{
	int arr[] = { 5, 2, 1, 4, 3 };
	int len = sizeof(arr) / sizeof(int);
	insertSort(arr, len);

	for (int i = 0; i < len;i++)
	{
		printf("%d\n", arr[i]);
	}

}

int main(){

	test01();

	system("pause");
	return EXIT_SUCCESS;
}