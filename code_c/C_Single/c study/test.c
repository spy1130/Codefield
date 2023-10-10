//用c画一个十字架
#include <stdio.h>
int main()
{
    int i, j;
    for (i = 1; i <= 9; i++)
    {
        for (j = 1; j <= 9; j++) //循环9次
        {
            if (i == 5 || j == 5) //如果i或者j等于5
            {
                printf(" * "); //输出*
            }
            else
            {
                printf("   "); //输出空格
            }
        }
        printf("\n"); //换行
    }
    return 0;
}


