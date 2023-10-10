#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <stddef.h>
struct Person
{
	char a; // 0 ~ 3
	int b;  // 4 ~ 7
};


void test01()
{
	struct Person p1;
	struct Person * p = &p1;

	printf("b��ƫ����Ϊ�� %d\n",  (int)&(p->b) - (int)p   );
	printf("b��ƫ����Ϊ�� %d\n", offsetof(struct Person, b));

}

//ͨ��ƫ���� ��ȡ������
void test02()
{
	struct Person p1 = {'a' , 10};

	printf("p.b = %d\n", *(int *)((char*)&p1 + offsetof(struct Person, b)));

	printf("p.b = %d\n", *(int *)((int*)&p1 + 1 ));
}


//�ṹ��Ƕ�׽ṹ��
struct Person2
{
	char a;
	int b;
	struct Person c;
};
void test03()
{
	struct Person2 p = { 'a', 10, 'b', 20 };

	int offset1 = offsetof(struct Person2, c);
	int offset2 = offsetof(struct Person, b);

	printf("%d\n", *(int*)((char*)&p + offset1 + offset2));


	printf("%d\n",   ((struct Person*) ((char*)&p + offset1))->b  );
}

int main(){

	//test01();
	//test02();
	test03();
	system("pause");
	return EXIT_SUCCESS;
}