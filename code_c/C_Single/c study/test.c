//6．输入一个正整数x（可以是1-9位的正整数），将其转换为逆序正整数y输出，例如输入x的值为3456，则输出y的值6543。
#include <stdio.h>
int main()
{
    int x, y = 0;
    printf("请输入一个正整数：");
    scanf("%d", &x);
    while (x > 0)
    {
        y = y * 10 + x % 10;
        x = x / 10;
    }
    printf("逆序正整数为：%d\n", y);
    return 0;
}


