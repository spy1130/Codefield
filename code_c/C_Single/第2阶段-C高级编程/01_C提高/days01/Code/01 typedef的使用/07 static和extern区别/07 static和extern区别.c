#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//1����̬����
static int a = 10; //�ص㣺ֻ��ʼ��һ�Σ��ڱ���׶ξͷ����ڴ棬�����ڲ��������ԣ�ֻ���ڵ�ǰ�ļ���ʹ��

void test01()
{
	static int b = 20; //�ֲ���̬����,������ֻ���ڵ�ǰtest01��

	//a �� b������������һ����
}


//2��ȫ�ֱ���

extern int g_a = 100; //��C������ ȫ�ֱ���ǰ�����ؼ��˹ؼ���  extern�������ⲿ��������

void test02()
{
	extern int g_b;//���߱����� g_b���ⲿ�������Ա�����������ʹ���������ʱ��Ҫ����

	printf("g_b = %d\n", g_b);

}

int main(){
	test02();
	

	system("pause");
	return EXIT_SUCCESS;
}