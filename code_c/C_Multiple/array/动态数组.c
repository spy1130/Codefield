#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"DynamicArray.h"
void test01()
{
        //��ʼ����̬����
        Dynamic_Array*myArray = Init_Array();
        //��ӡ����
        printf("����������%d\n",Capacity_Array(myArray));
        printf("�����С��%d\n", Size_Array(myArray));
        //����Ԫ��
        for (int i = 0; i < 30; i++)
        {
               PushBack_Array(myArray, i);
        }
        printf("����������%d\n", Capacity_Array(myArray));
        printf("�����С��%d\n", Size_Array(myArray));
        //��ӡ
        Print_Array(myArray);
        //ɾ��
        RemoveByPos_Array(myArray, 0);
        RemoveByValue_Array(myArray, 27);
        //��ӡ
        Print_Array(myArray);
        //����5��λ��
        int pos = Find_Array(myArray,5);
        printf("5���ҵ�:pos%d %d\n",pos,At_Array(myArray,pos));
        //����
        FreeSpace_Array(myArray);
}
int main()
{
        test01();
        return EXIT_SUCCESS;
}
