#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const int a = 10;//��������һ����ʼ���������޸�

void test()
{
	//a = 200;ȫ�ֵ�const����ֱ���޸�
	int *p = (int*)&a;
	//*p = 100;//ȫ�ֵ�const���ܼ���޸�
	printf("%d\n", a);

}
int main2()
{
	const int b = 20;//ջ��
	//b = 200;�ֲ���const���εı�������ֱ���޸�
	int *p = (int*)&b;
	*p = 200;//���Լ���޸�
	printf("%d\n", b);

	//ʹ�ñ���ļ���ȫ��const���εı�����Ҫ����
	extern const int c;
	printf("c=%d\n", c);

	system("pause");
	return EXIT_SUCCESS;
}

