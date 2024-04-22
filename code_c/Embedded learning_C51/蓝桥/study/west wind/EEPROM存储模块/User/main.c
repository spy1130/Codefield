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
uchar read_data;
uchar write_data;
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
    if (Key_Down > 4)
        write_data = Key_Down;
    if (Key_Down == 4)
    {
        EEPROM_Write(&write_data, 0, 1);
    }
}
/* ����ܴ����� */
void Seg_Proc()
{
    if (Seg_Slow_Down)
        return;
    Seg_Slow_Down = 1;
    Seg_Buf[0] = read_data / 100 % 10;
    Seg_Buf[1] = read_data / 10 % 10;
    Seg_Buf[2] = read_data % 10;
    Seg_Buf[5] = write_data / 100 % 10;
    Seg_Buf[6] = write_data / 10 % 10;
    Seg_Buf[7] = write_data % 10;
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

void main()
{
    System_Init();
    Timer0_Init();
    EEPROM_Read(&read_data, 0, 1);
    while (1)
    {
        Key_Proc();
        Seg_Proc();
        Led_Proc();
    }
}