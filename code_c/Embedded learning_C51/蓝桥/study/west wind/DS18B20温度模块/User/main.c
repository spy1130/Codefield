#include "main.h"
/* LED��ʾ */
uchar ucLed[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/* �������ʾ */
uchar Seg_Slow_Down;                                // ����ܼ���
uchar Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10}; // �������ʾ��ֵ
uchar Seg_Pos;                                      // �����ָʾ
uchar Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};      // ĳλ�Ƿ���ʾС����

/* ���̷��� */
uchar Key_Slow_Down;

/* ���� */
uint temperature_value_10x; // �¶�����
bit C_F_flag;               // ���϶�ת���϶�

#define N 10
uint data_array[N]; // ���ڴ�С
uint sum_temp;      // �ܺ�
uchar index_temp;   // ����
uchar arr_count;    // ������������

uint filter(uint new_data)
{
    sum_temp -= data_array[index_temp];
    data_array[index_temp] = new_data;
    sum_temp += data_array[index_temp];
    index_temp = (++index_temp) % N;                    // ��֤index_temp��0~N-1֮����ת
    arr_count = (++arr_count == N + 1) ? N : arr_count; // ���������е�Ԫ�ظ���
    return sum_temp / arr_count;
}

/* ���̴����� */
void Key_Proc()
{
    static uchar Key_Val, Key_Down, Key_Up, Key_Old;
    if (Key_Slow_Down)
        return;
    Key_Slow_Down = 1;

    Key_Val = Key_Read();
    Key_Down = Key_Val & (Key_Old ^ Key_Val);
    Key_Up = ~Key_Val & (Key_Old ^ Key_Val);
    Key_Old = Key_Val;
    if (Key_Down == 4)
        C_F_flag ^= 1;
}
/* ����ܴ����� */
void Seg_Proc()
{
    if (Seg_Slow_Down)
        return;
    Seg_Slow_Down = 1;
    temperature_value_10x = filter(rd_temperature() * 10);
    if (C_F_flag)
        temperature_value_10x = temperature_value_10x * 1.8 + 320; // ���϶�
    Seg_Buf[0] = temperature_value_10x / 1000 % 10;
    Seg_Buf[1] = temperature_value_10x / 100 % 10;
    Seg_Buf[2] = temperature_value_10x / 10 % 10;
    Seg_Buf[3] = temperature_value_10x % 10;
    Seg_Buf[4] = (C_F_flag) ? 12 : 11;
		Seg_Point[2] = 1;
}

/* LED������ */
void Led_Proc()
{
}

/* ��ʱ��0�жϳ�ʼ�� */
void Timer0_Init(void) // 1����@12.000MHz
{
    AUXR &= 0x7F; // ��ʱ��ʱ��12Tģʽ
    TMOD &= 0xF0; // ���ö�ʱ��ģʽ
    TL0 = 0x18;   // ���ö�ʱ��ʼֵ
    TH0 = 0xFC;   // ���ö�ʱ��ʼֵ
    TF0 = 0;      // ���TF0��־
    TR0 = 1;      // ��ʱ��0��ʼ��ʱ
    ET0 = 1;
    EA = 1;
}

/* ��ʱ��0�жϺ��� */
void Timer0_ISR(void) interrupt 1
{
    if (++Key_Slow_Down == 10)
        Key_Slow_Down = 0;
    if (++Seg_Slow_Down == 500)
        Seg_Slow_Down = 0;
    if (++Seg_Pos == 8)
        Seg_Pos = 0;
    Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
    Led_Disp(Seg_Pos, ucLed[Seg_Pos]);
}

void Delay750ms(void) //@12.000MHz
{
    unsigned char data i, j, k;

    _nop_();
    _nop_();
    i = 35;
    j = 51;
    k = 182;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void main()
{
    System_Init();
    Timer0_Init();
    rd_temperature();
    Delay750ms();
    while (1)
    {
        Key_Proc();
        Seg_Proc();
        Led_Proc();
    }
}