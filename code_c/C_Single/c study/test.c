//��c��һ��ʮ�ּ�
#include <stdio.h>
int main()
{
    int i, j;
    for (i = 1; i <= 9; i++)
    {
        for (j = 1; j <= 9; j++) //ѭ��9��
        {
            if (i == 5 || j == 5) //���i����j����5
            {
                printf(" * "); //���*
            }
            else
            {
                printf("   "); //����ո�
            }
        }
        printf("\n"); //����
    }
    return 0;
}


