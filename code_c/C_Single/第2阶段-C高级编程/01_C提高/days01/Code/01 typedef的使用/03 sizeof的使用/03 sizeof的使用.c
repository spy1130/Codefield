#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//1��sizeof���ʣ� �ǲ���һ������������ ���Ǻ�����ֻ��һ��������������+-*/
void test01()
{
	//������������ ��sizeof������()ȥʹ�ã����Ƕ��ڱ��������Բ���()
	printf("size of int = %d\n", sizeof(int));

	double d = 3.14;

	printf("size of d = %d\n", sizeof d );

}

//2��sizeof�ķ���ֵ������ʲô ��  unsigned int �޷�������
void test02()
{
	//unsigned int a = 10;
	//if (a - 20 > 0) //��unsigned int��int�������������㣬�������Ὣ�������Ͷ�תΪunsigned int
	//{
	//	printf("���� 0 \n");
	//}
	//else
	//{
	//	printf("С�� 0 \n");
	//}


	if ( sizeof(int) - 5 > 0 )
	{
		printf("���� 0 %u \n", sizeof(int)-5);
	}
	else
	{
		printf("С�� 0 \n");
	}

}


//3��sizeof����ͳ�����鳤��
//����������Ϊ��������ʱ�򣬻��˻�Ϊָ�룬ָ�������е�һ��Ԫ��
void calculateArray( int arr[] )
{
	printf("arr�����鳤�ȣ� %d\n", sizeof(arr));
}

void test03()
{
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

	//printf("arr�����鳤�ȣ� %d\n", sizeof(arr));

	calculateArray(arr);
}

int main(){
	//test01();
	//test02();
	test03();


	system("pause");
	return EXIT_SUCCESS;
}