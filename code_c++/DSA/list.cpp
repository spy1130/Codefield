#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#define MAX_ARRAY_DIM 8
#define MAX_ARRAY_DIM 8�� //�������ά��
typedef struct
{
    int *base;      // ��������Ԫ�ش洢�ռ��׵�ַ
    int dim;        // ��������ά��
    int *bounds;    // ָ�򱣴��άά��Ĵ洢�ռ�
    int *constants; // ָ�򱣴���ַ�����Ĵ洢�ռ�
} Array;

// ϡ������ϡ��������ѹ���洢ʱ����ֻ�������з���Ԫ��ͬʱ����ָ������Ԫ����������λ�ü��ܵ�������������ʹ����Ԫ����д洢
#define MAXSIZE 1000 // ����Ԫ������
typedef struct
{
    int i; // ���±�
    int j; // ���±�
    int e; // ����Ԫֵ
} Triple;  // ��Ԫ������
//��Ԫ�鶨�壺
typedef struct
{
    Triple data[MAXSIZE]; // ��Ԫ���
    int m;                // ����
    int n;                // ����
    int t;                // ����Ԫ����
} TSMatrix;
//https://blog.csdn.net/weixin_42437114/article/details/107755449
//�ԣ��������д��ȷʵnb
int main()
{

    system("pause");
    return EXIT_SUCCESS;
}