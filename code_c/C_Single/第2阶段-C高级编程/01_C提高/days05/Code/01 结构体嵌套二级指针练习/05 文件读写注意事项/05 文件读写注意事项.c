#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void test01()
{
	FILE * file = fopen("./test.txt", "r");

	if (file == NULL)
	{
		return;
	}

#if 0
	char ch;
	while ( !feof (file))
	{
		ch = fgetc(file);

		//�ͺ���
		if (feof(file))
		{
			break;
		}

		printf("%c", ch);
	}
#endif


	char ch;

	while ((ch = fgetc(file)) != EOF)
	{
		printf("%c", ch);
	}

	fclose(file);
}


//ע������2
struct Person
{
	char * name; //��Ҫ��ָ�� д�뵽�ļ���
	int age;
};


int main(){
	test01();

	//printf("%c",EOF);
	
	system("pause");
	return EXIT_SUCCESS;
}