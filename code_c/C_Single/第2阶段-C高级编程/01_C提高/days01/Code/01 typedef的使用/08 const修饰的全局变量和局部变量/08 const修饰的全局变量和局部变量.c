#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//1��const���ε�ȫ�ֱ���,��ʹ�﷨ͨ������������ʱ���ܵ��������ı���������ʧ��
const int a = 10; //���ڳ�����

void test01()
{
	//a = 100; //ֱ���޸� ʧ��

	int * p = &a;
	*p = 100;

	printf("%d\n", a);

}

//2��const���εľֲ�����
void test02()
{
	const int b = 10; //���䵽ջ��
	//b = 100; //ֱ���޸�ʧ�ܵ�


	//����޸� �ɹ�
	//C������ ��Ϊα����
	int * p = &b;
	*p = 100;
	printf("b = %d\n", b);

	//int a[b];//α�����ǲ����Գ�ʼ�������
}


//�ַ�������
void test03()
{
	char * p1 = "hello world";
	char * p2 = "hello world";
	char * p3 = "hello world";

	printf("%d\n", p1);
	printf("%d\n", p2);
	printf("%d\n", p3);
	printf("%d\n", &"hello world");


	p1[0] = 'z'; //�������޸��ַ�������
	printf("%c\n", p1[0]);

}


int main(){
	//test01();
	//test02();
	test03();
	system("pause");
	return EXIT_SUCCESS;
}