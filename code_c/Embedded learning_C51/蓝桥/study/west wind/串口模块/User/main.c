#include "main.h"
/* LED��ʾ */
uchar ucLed[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/* �������ʾ */
uchar Seg_Slow_Down;                                // ����ܼ���
uchar Seg_Buf[8] = {5, 10, 10, 10, 10, 10, 10, 10}; // �������ʾ��ֵ
uchar Seg_Pos;                                      // �����ָʾ
uchar Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};      // ĳλ�Ƿ���ʾС����

/* ���ڷ��� */
uchar Uart_Slow_Down;
uchar Uart_Buf[20];  // ���ڽ��յ�������
uchar Uart_Rx_Index; // ���ڽ��յ������ݵ�ָ��

/* ���̷��� */
uchar Key_Slow_Down;

/* ���� */
uchar input_data;
bit down_flag;
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
    if (Key_Down != 0)
    {
        input_data = Key_Down;
        down_flag = 1;
    }
}
/* ����ܴ����� */
void Seg_Proc()
{
    if (Seg_Slow_Down)
        return;
    Seg_Slow_Down = 1;
}

/* LED������ */
void Led_Proc()
{
}

/* ���ڴ����� */
void Uart_Proc()
{
    if (Uart_Slow_Down)
        return;
    Uart_Slow_Down = 1;
    if (down_flag)
    {
        printf(" You pressed it %bu", input_data);
        down_flag = 0;
    }
    if (Uart_Buf[0] == 'O' && Uart_Buf[1] == 'K')
    {
        // ִ����غ���
        printf("hello");
        memset(Uart_Buf, 0, 20);
        Uart_Rx_Index = 0;
    }
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
    if (++Uart_Slow_Down == 200)
        Uart_Slow_Down = 0;
    if (++Seg_Pos == 8)
        Seg_Pos = 0;
    Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
    Led_Disp(Seg_Pos, ucLed[Seg_Pos]);
}

/* �����жϷ����� */
void Uart_ISR(void) interrupt 4
{
    if (RI == 1) // ���ڽ��յ�����
    {
        Uart_Buf[Uart_Rx_Index] = SBUF;
        Uart_Rx_Index++;
        RI = 0;
    }
}
void main()
{
    System_Init();
    Timer0_Init();
    Uart1_Init();
    while (1)
    {
        Key_Proc();
        Seg_Proc();
        Uart_Proc();
        Led_Proc();
    }
}