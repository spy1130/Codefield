//#define _CRT_SECURE_NO_WARNINGS  //VS������ʹ�ô�ͳ�⺯��,�����������꣬�����һ������ţ�C4996
#include<stdio.h>   // std ��׼ i input  ����   o  output ��� 
#include<string.h>  // strcpy   strcmp  strcat  strstr
#include<stdlib.h>  // malloc  free


//1��typedefʹ��  �򻯽ṹ��ؼ��� struct
//struct Person
//{
//	char name[64];
//	int age;
//};
//typedef struct Person  myPerson;

//��Ҫ��;  �����������
//�﷨  typedef  ԭ��  ����
typedef struct Person
{
	char name[64];
	int age;
}myPerson;

void test01()
{
	struct Person p1 = { "����", 19 };

	myPerson p2 = { "����", 20 };
}


// 2��������������
void test02()
{
	//char * p1, p2;  //p1��char *  �� p2 ��char


	typedef char * PCHAR;
	PCHAR p1, p2;


	char *p3, *p4; // p3 �� p4����char *
}


//3����ߴ�����ֲ��
typedef int MYINT; //typedef long long MYINT; ֻ��Ҫ�滻 long long �Ϳ�����
void test03()
{
	MYINT a = 10;

	MYINT a2 = 10;
}


//�������
int main1(){

	


	system("pause"); // ���������ͣ  ��������

	return EXIT_SUCCESS; //���� �����˳�ֵ  0
}