#include "main.h"
/* LED��ʾ */
uchar ucLed[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/* �������ʾ */
uchar Seg_Buf[8] = {10, 10, 10, 10, 10, 10, 10, 10}; // �������ʾ��ֵ
uchar Seg_Pos;                                       // �����ָʾ
uchar Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};       // ĳλ�Ƿ���ʾС����

/* ʱ�䷽�� */
uchar ucRtc[3] = {0x13, 0x11, 0x11}; // ��ʼ��ʱ��13:11:11

/* ʱ�䷽�� */
uint time_all_1s;
uchar time_100ms;
/* ��ʾ */
uchar Seg_show_mode; // 0 �ź� 1 �¶� 2 ����
/* ���� */
uint freq;
uint T_value_10x;
uchar T_para = 25;
uchar V_out_amp;
/* �ж� */
bit Wring_flag;
bit Hot_flag;
bit Led_blink_show;
/* ���ݴ����� */
void Data_Proc()
{
    if (time_all_1s % 500 == 0)
    {
        // �¶ȶ�ȡ
        T_value_10x = rd_temperature() * 10;
        Wring_flag = (T_value_10x > T_para * 10);
        Hot_flag = (T_value_10x > 300);
    }
}
/* ���̴����� */
void Key_Proc()
{
    static uchar Key_Val, Key_Down, Key_Up, Key_Old;
    if (time_all_1s % 10)
        return;
    Key_Val = Key_Read();
    Key_Down = Key_Val & (Key_Old ^ Key_Val);
    Key_Up = ~Key_Val & (Key_Old ^ Key_Val);
    Key_Old = Key_Val;
    if (Key_Down == 4)
        Seg_show_mode = (++Seg_show_mode) % 3;
    if (Seg_show_mode == 2)
    {
        if (Key_Down == 9)
            T_para = (T_para == 35) ? 20 : T_para + 1;
        else if (Key_Down == 8)
            T_para = (T_para == 20) ? 35 : T_para - 1;
    }
}
/* ����ܴ����� */
void Seg_Proc()
{
    uchar i;
    if (time_all_1s % 20)
        return;
    switch (Seg_show_mode)
    {
    case 0:
        /* �źŽ��� */
        Seg_Point[6] = 0;
        Seg_Buf[0] = 11; // P
        Seg_Buf[1] = 10;
        Seg_Buf[2] = freq / 100000 % 10;
        Seg_Buf[3] = freq / 10000 % 10;
        Seg_Buf[4] = freq / 1000 % 10;
        Seg_Buf[5] = freq / 100 % 10;
        Seg_Buf[6] = freq / 10 % 10;
        Seg_Buf[7] = freq % 10;
        i = 2;
        while (Seg_Buf[i]==0)
        {
            Seg_Buf[i]=10��
			if(++i=7)break;
        }
        break;

    case 1:
        /* �¶Ƚ��� */
        Seg_Point[6] = 1;
        Seg_Buf[0] = 12; // C
        Seg_Buf[1] = Seg_Buf[2] = Seg_Buf[3] = Seg_Buf[4] = 10;
        Seg_Buf[5] = (T_value_10x / 100 % 10 == 0) ? 10
                                                   : T_value_10x / 100 % 10;
        Seg_Buf[6] = T_value_10x / 10 % 10;
        Seg_Buf[7] = T_value_10x % 10;
        break;
    case 2:
        /* �������� */
        Seg_Point[6] = 0;
        Seg_Buf[0] = 13; // U
        Seg_Buf[1] = Seg_Buf[2] = Seg_Buf[3] = Seg_Buf[4] = 10;
        Seg_Buf[5] = T_para / 100 % 10;
        Seg_Buf[6] = T_para / 10 % 10;
        Seg_Buf[7] = T_para % 10;
        i = 5;
        while (i < 7)
        {
            if (Seg_Buf[i - 1] == 10 && Seg_Buf[i] == 0)
                Seg_Buf[i] = 10;
            else
                break;
        }
        break;
    }
}

/* LED������ */
void Led_Proc()
{
    Da_Write(V_out_amp);
    Relay(Wring_flag);
    ucLed[0] = (Seg_show_mode == 0);
    ucLed[1] = (Seg_show_mode == 1);
    ucLed[7] = Led_blink_show;
}

/* ��ʱ��0�жϳ�ʼ�� */
void Timer0_Init(void) // 1����@12.000MHz
{
    AUXR &= 0x7F; // ��ʱ��ʱ��12Tģʽ
    TMOD &= 0xF0; // ���ö�ʱ��ģʽ
    TMOD |= 0x05;
    TL0 = 0x18; // ���ö�ʱ��ʼֵ
    TH0 = 0xFC; // ���ö�ʱ��ʼֵ
    TF0 = 0;    // ���TF0��־
    TR0 = 1;    // ��ʱ��0��ʼ��ʱ
}

void Timer1_Init(void) // 1����@12.000MHz
{
    AUXR &= 0xBF; // ��ʱ��ʱ��12Tģʽ
    TMOD &= 0x0F; // ���ö�ʱ��ģʽ
    TL1 = 0x18;   // ���ö�ʱ��ʼֵ
    TH1 = 0xFC;   // ���ö�ʱ��ʼֵ
    TF1 = 0;      // ���TF1��־
    TR1 = 1;      // ��ʱ��1��ʼ��ʱ
    ET1 = 1;      // ʹ�ܶ�ʱ��1�ж�
    EA = 1;
}
void Timer1_Isr(void) interrupt 3
{
    if (++time_all_1s == 1000)
    {
        time_all_1s = 0;
        freq = TH0 << 8 | TL0;
        if (freq > 2000)
            V_out_amp = 4.5f * 51;
        else if (freq < 200)
            V_out_amp = 0.5f * 51;
        else
            V_out_amp = ((float)(freq - 200) / 450.0f + 0.5f) * 51;
        TH0 = TL0 = 0;
    }
    if (++Seg_Pos == 8)
        Seg_Pos = 0;
    if (Hot_flag)
    {
        if (++time_100ms == 100)
        {
            Led_blink_show ^= 1;
            time_100ms = 0;
        }
    }
    else
    {
        time_100ms = 0;
        Led_blink_show = 0;
    }
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
    Timer1_Init();
    Timer0_Init();

    T_value_10x = rd_temperature() * 10;
    Delay750ms();
    while (1)
    {
        Data_Proc();
        Key_Proc();
        Seg_Proc();
        Led_Proc();
    }
}