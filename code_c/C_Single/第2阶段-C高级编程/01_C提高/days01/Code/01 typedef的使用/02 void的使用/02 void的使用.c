#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//1���������ǲ����Դ���������
void test01()
{
	//void a = 10; //������ֱ�ӱ�����Ϊ��֪����a��������ڴ�ռ�
}


//2�������޶���������ֵ
void func()
{
	//return 10;
}

void test02()
{
	//func();
	//printf("%d\n", func());
}


//3���޶����������б�
int func2(void)
{
	return 10;
}
void test03()
{
	//printf("%d\n", func2(10));
}


//4��void *  ����ָ��
void test04()
{
	void * p = NULL;

	int  * pInt = NULL;
	char * pChar = NULL;

	//pChar = (char *)pInt;
	pChar = p; //����ָ��  ���Բ���Ҫǿ������ת���Ϳ��Ը��Ⱥ���߸�ֵ

	printf("size of void *   = %d\n", sizeof(p));

}

int main(){
	//test02();
	//test03();
	test04();
	system("pause");
	return EXIT_SUCCESS;
}