#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#pragma pack(show) //�鿴����ģ��
//#pragma pack(1) //Ĭ�϶���ģ��Ϊ 8  ����ģ�����Ը�Ϊ 2��n�η�


//�����Զ�����������  �ڴ������� ���£�
//1���ӵ�һ�����Կ�ʼ  ƫ��Ϊ0 
//2���ڶ������Կ�ʼ����ַҪ����  ������������   �� ����ģ����  ȡС��ֵ ����������
//3�����е����Զ���������������������ζ��룬������Ҫ����������������� �� ����ģ���� ȡС��ֵ����������
typedef struct _STUDENT{
	//       ����ģ��8      ����ģ��1
	int a;   // 0 ~ 3        0  ~ 3
	char b;  // 4 ~ 7        4 
	double c;// 8  ~ 15      5  ~ 12
	float d; // 16 ~ 19      13 ~ 16
}Student;


void test01()
{

	printf("size of =  %d\n", sizeof(Student));

}


//���ṹ��Ƕ�׽ṹ��ʱ��ֻ��Ҫ���ӽṹ��������������;Ϳ�����
typedef struct _STUDENT2{
	char a; //  0 ~  7
	Student b;  // 8 ~ 31
	double c;   // 32 ~ 39
}Student2;


void test02()
{
	printf("size of =  %d\n", sizeof(Student2));

}


int main(){

	//test01();
	test02();
	system("pause");
	return EXIT_SUCCESS;
}