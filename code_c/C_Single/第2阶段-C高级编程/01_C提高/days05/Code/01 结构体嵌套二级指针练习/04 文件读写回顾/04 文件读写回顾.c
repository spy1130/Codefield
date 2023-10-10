#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//1���ַ��Ķ�д�ع� fgetc(), fputc()
void test01()
{
	//д�ļ�
	FILE * f_write = fopen("./test1.txt", "w");

	if (f_write == NULL)
	{
		return;
	}


	char buf[] = "hello world";

	for (int i = 0; i < strlen(buf);i++)
	{
		fputc(buf[i], f_write);
	}

	fclose(f_write);

	//���ļ�
	FILE * f_read = fopen("./test1.txt", "r");
	if (f_read == NULL)
	{
		return;
	}

	char ch;

	while (  (ch = fgetc(f_read)) != EOF) // EOF  END  OF  FILE
	{
		printf("%c", ch);
	}

	fclose(f_read);
}



//2�����н��ж�д
void test02()
{
	//д�ļ�
	FILE * f_write = fopen("./test2.txt", "w+");

	if (f_write == NULL)
	{
		return;
	}

	char * buf[]=
	{
		"�����յ���\n",
		"���κ�����\n",
		"˭֪���в�\n",
		"����������\n"
	};

	for (int i = 0; i < 4;i++)
	{
		fputs(buf[i], f_write);
	}

	fclose(f_write);


	//���ļ�
	FILE * f_read = fopen("./test2.txt", "r");
	if (f_read == NULL)
	{
		return;
	}
	while ( !feof(f_read))
	{
		char temp[1024] = { 0 };

		fgets(temp, 1024, f_read);

		printf("%s", temp);

	}

	fclose(f_read);

}


//3��������ж�д

struct Hero
{
	char name[64];
	int age;
};
void test03()
{
	//д�ļ�
	FILE * f_wirte = fopen("./test3.txt", "wb"); 
	if (f_wirte == NULL)
	{
		return;
	}

	struct Hero heros[] = 
	{
		{ "�����", 999 },
		{ "��ɪ", 20 },
		{ "�ܲ�", 80 },
		{ "³��", 5 },
	};

	for (int i = 0; i < 4;i++)
	{
		//����1 ���ݵ�ַ   ����2  ���С   ����3  �����   ����4  �ļ�ָ��
		fwrite(&heros[i], sizeof(struct Hero), 1, f_wirte);
	}

	fclose(f_wirte);


	//���ļ�
	FILE * f_read = fopen("./test3.txt", "rb");

	if (f_read == NULL)
	{
		return;
	}

	struct Hero temp[4];
	fread(&temp, sizeof(struct Hero), 4, f_read);
	for (int i = 0; i < 4;i++)
	{
		printf("������ %s  ���䣺%d\n", temp[i].name, temp[i].age);
	}

	fclose(f_read);

}

//4����ʽ����д�ع�
void test04()
{
	
	//д�ļ�
	FILE *f_write = fopen("./test4.txt", "w");
	if (f_write == NULL)
	{
		return;
	}

	fprintf(f_write, "hello world %s", "abcd");

	fclose(f_write);


	//���ļ�
	FILE * f_read = fopen("./test4.txt", "r");
	if (f_read == NULL)
	{
		return;
	}
	char temp[1204] = { 0 };
	while (! feof(f_read ))
	{
		fscanf(f_read, "%s", temp);
		printf("%s\n", temp);
	}

	fclose(f_read);
}

void test05()
{
	//д�ļ�
	FILE * f_wirte = fopen("./test5.txt", "wb");
	if (f_wirte == NULL)
	{
		return;
	}

	struct Hero heros[] =
	{
		{ "�����", 999 },
		{ "��ɪ", 20 },
		{ "�ܲ�", 80 },
		{ "³��", 5 },
	};

	for (int i = 0; i < 4; i++)
	{
		//����1 ���ݵ�ַ   ����2  ���С   ����3  �����   ����4  �ļ�ָ��
		fwrite(&heros[i], sizeof(struct Hero), 1, f_wirte);
	}

	fclose(f_wirte);


	//���ļ�
	FILE * f_read = fopen("./test51.txt", "rb");
	if (f_read == NULL)
	{
		//error ��
		//printf("�ļ�����ʧ��\n");
		perror("�ļ�����ʧ��"); //�û���ʾ��Ϣ + ϵͳ��ʾ��Ϣ

		return;
	}
	struct Hero tempHero;
	//�ƶ����
	// ����1  �ļ�ָ��   ����2 ƫ�ƴ�С    ����3  ��ʼλ��   
	// SEEK_SET �ӿ�ʼ  SEEK_END �ӽ�β   SEEK_CUR �ӵ�ǰλ��
	//fseek(f_read, sizeof(struct Hero) * 2, SEEK_SET);

	fseek(f_read, - (long)sizeof(struct Hero) * 2, SEEK_END);

	rewind(f_read); //���ļ��������

	fread(&tempHero, sizeof(struct Hero), 1, f_read);

	printf(" ������ %s , ���䣺 %d\n", tempHero.name, tempHero.age);


	fclose(f_read);
}


int main(){
	//test01();
	//test02();
	//test03();
	//test04();
	test05();


	system("pause");
	return EXIT_SUCCESS;
}