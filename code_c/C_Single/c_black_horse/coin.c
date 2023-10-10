#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int money;
    scanf("%d", &money);

    // ��ֵΪ 0 ��������⴦��
    if (money == 0) {
        printf("a");
        return 0;
    }

    /*
     * ��ȡ money �ĸ�λ���ֲ����δ�� digits �����У�
     * �� money = 813227345 ����һǧ���ٶ�ʮ������ǧ������ʮ�壬�� digits[10] = {5, 4, 3, 7, 2, 2, 3, 1, 8}
     */
    int len = 0;
    int digits[9] = {0};
    while (money) {
        digits[len++] = money % 10;
        money /= 10;
    }

    /*
     * ��ֵ��λ��S��B��Q��W��Y �ֱ����ʰ���١�Ǫ�����ڣ�
     * ����������� digits ����Ĺ�ϵ�����ѿ������������ digits ����ʱ��digits ������±�ǡ�� currencyUnit ��λ����
     *  digits[10]    =     {5,   4,   3,   7,   2,   2,   3,   1,   8}  | 813227345
     *  currencyUnit[9] = {'\0', 'S', 'B', 'Q', 'W', 'S', 'B', 'Q', 'Y'} | ����һǧ���ٶ�ʮ������ǧ������ʮ��
     */
    char currencyUnit[9] = {'\0', 'S', 'B', 'Q', 'W', 'S', 'B', 'Q', 'Y'};

    // �� digits ����β��ͷ�����������Ӧ�������ֵ
    for (int i = len - 1; i >= 0; i--) {
        if (digits[i]) {
            // ��ĸ a-j ��Ӧ���� 0-9���˴���� money ֵ����λ�������±� i ���ֵ��λ���� currencyUnit �Ķ�Ӧ��ϵ
            printf("%c%c", digits[i] + 'a', currencyUnit[i]);
        } else if (i == 4) {
            // �ж��Ƿ���Ҫ�����ֵ��λ�򣬵��ҽ��� i == 4 ���Ҵ���ʮ�� | ���� | ǧ��ʱ����� W
            if (digits[5] || digits[6] || digits[7]) {
                printf("W");
            }
        } else if (i - 1 >= 0 && digits[i - 1]) {
            /*
             * ��ĿҪ�󣺡��㡱 ���÷������������ϰ�ߣ�
             * ��ǰ��һλ�ж��Ƿ������ǰ 0 ��Ӧ�� a�����ҽ�����ǰ 0 ���� money ��ĩλ������ 0 ��ǰһ�����ֲ�Ϊ 0 ʱ����� a��
             * �� 6900����ǧ�Ű� gQjB digits = {0, 0, 9, 6, 0...}�� ����Ҫ��� 0��
             * �� 6090����ǧ���ʮ gQajS digits = {0, 9, 0, 6, 0...}������Ҫ��� 6 �� 9 �м�� 0
             */
            printf("a");
        }
    }

    return 0;
}

