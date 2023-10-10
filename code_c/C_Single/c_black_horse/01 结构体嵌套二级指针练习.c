#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Teacher
{
	char * name;
	char ** Students;
};


void allocateSpace( struct Teacher *** teachers)
{

	struct Teacher ** pArray = malloc(sizeof(struct Teacher *) * 3);

	for (int i = 0; i < 3;i++)
	{
		//��ÿ����ʦ����ռ�
		pArray[i] =  malloc(sizeof(struct Teacher));

		//��ÿ����ʦ���� ����ռ�
		pArray[i]->name = malloc(sizeof(char)* 64);
		sprintf(pArray[i]->name, "Teacher_%d", i + 1);

		//����ʦ����ѧ�����������ռ�
		pArray[i]->Students =  malloc(sizeof(char *)* 4);

		//��4��ѧ���ڷ����ڴ棬���Ҹ�ֵ
		for (int j = 0; j < 4;j++)
		{
			pArray[i]->Students[j] = malloc(sizeof(char)* 64);
			sprintf(pArray[i]->Students[j], "%s_Student_%d", pArray[i]->name, j + 1);
		}
	}


	*teachers = pArray;
}


void showArray(struct Teacher ** pArray, int len)
{
	for (int i = 0; i < len;i++)
	{
		printf("%s\n", pArray[i]->name);
		for (int j = 0; j < 4;j++)
		{
			printf("     %s\n", pArray[i]->Students[j]);
		}
	}
}

void freeSpace(struct Teacher ** pArray , int len)
{
	for (int i = 0; i < len;i++)
	{
		//�ͷ���ʦ����
		if (pArray[i]->name != NULL)
		{
			free(pArray[i]->name);
			pArray[i]->name = NULL;
		}

		//�ͷ�ÿ��ѧ��
		for (int j = 0; j < 4;j++)
		{
			if (pArray[i]->Students[j] != NULL)
			{
				free(pArray[i]->Students[j]);
				pArray[i]->Students[j] = NULL;
			}
		}

		//�ͷ�ѧ������
		free(pArray[i]->Students);
		pArray[i]->Students = NULL;


		//�ͷ���ʦ
		free(pArray[i]);
		pArray[i] = NULL;
	}


	//�ͷ���ʦ����
	free(pArray);
	pArray = NULL;
}

void test01()
{

	struct Teacher ** pArray = NULL;
	//�����ڴ�
	allocateSpace(&pArray);

	//��ӡ����
	showArray(pArray , 3);


	//�ͷ��ڴ�
	freeSpace(pArray,3);
	pArray = NULL;

}


int main(){
	test01();


	system("pause");
	return EXIT_SUCCESS;
}