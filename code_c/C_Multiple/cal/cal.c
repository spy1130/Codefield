#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

void Print_Factorial(const int N);

int main()
{
    int N;
    LARGE_INTEGER lpPerformanceCount; // ��ʱ���õ�
    QueryPerformanceFrequency(&lpPerformanceCount);
    int c1, c2;

    scanf("%d", &N);
    QueryPerformanceCounter(&lpPerformanceCount); // ��õ�ǰʱ�䣬��λ��΢��
    c1 = lpPerformanceCount.QuadPart;
    Print_Factorial(N);
    QueryPerformanceCounter(&lpPerformanceCount);
    c2 = lpPerformanceCount.QuadPart;
    printf("\n%ld us\n", c2-c1);
    return 0;
}
void Print_Factorial(const int N)
{
    int num[10000] = {0};
    int ans = 1, list_max = 0, carry = 0;
    if (N < 0) // ��С��0ʱ
        printf("Invalid input\n");
    else if (N == 0) // ����0ʱ
        printf("1\n");
    else if (N > 0 && N <= 12) // С��12ʱ
    {
        for (int m = 1; m <= 12; m++)
        {
            ans *= m;
        }
        printf("%d", ans);
    }
    else // ����12��1000֮��
    {
        num[0] = 1;
        for (int i = 2; i <= N; i++) // �˵���
        {
            for (int n = 0; n <= list_max; n++)
            {
                ans = num[n] * i + carry; // ��λ��i����Ͻ�λ����ʱֵ
                num[n] = ans % 10;
                carry = ans / 10; // ��¼��λ
            }
            while (carry > 0) // ���н�λʱǰ��û��λ��,����λ����ѭ��ҲҲ���ǽ�λ�����ܻ���ֳ�����λ������
            {
                list_max++;
                num[list_max] = carry % 10;
                carry /= 10;
            }
        }
        for (int k = list_max; k >= 0; k--) // ��ӡ
        {
            printf("%d", num[k]);
        }
    }
}