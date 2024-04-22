#include "main.h"
/* LED��ʾ */
uchar ucLed[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/* �������ʾ */
uchar Seg_Slow_Down;                                // ����ܼ���
uchar Seg_Buf[8] = {5, 10, 10, 10, 10, 10, 10, 10}; // �������ʾ��ֵ
uchar Seg_Pos;                                      // �����ָʾ
uchar Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};      // ĳλ�Ƿ���ʾС����

/* ������ʾ */
uchar Seg_show_mode; // 0 ��� 1 ���� 2 ��¼
uchar Control_mode;  // 0 ���� 1 ��ť
/* ���̷��� */
uchar Key_Slow_Down;
/* ���� */
uchar Distance_value; // �����������
uchar Para_max = 60;  // ��������
uchar Para_min = 10;  // ��������
uchar Wring_count;    // ��������
bit Wring_flag;       // ������־
uchar Para_mode;      // 0 ������ 1 ���� 2 ����
uchar Vol_level;      // ��ѹ�ȼ������ڿ��������� 1 2 3 4
bit Led_show_flag;    // LED��˸
/* ʱ�� */
uchar time_100ms;
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
        Seg_show_mode = (++Seg_show_mode) % 3;

    if (Key_Down == 5)
    {
        if (Seg_show_mode == 1)
            Control_mode = (++Control_mode) % 2;
        if (Seg_show_mode == 2)
            Wring_count = 0;
        Para_mode = 0; // �����л���ʱ����ť������������ģʽ���ã���ֹ����bug
    }
    if (Seg_show_mode == 1)
    { // ��������ģʽ
        if (Control_mode == 0)
        {
            if (Key_Down == 9)
                Para_max = (Para_max + 10 > 90) ? 50
                                                : Para_max + 10;
            if (Key_Down == 8)
                Para_min = (Para_min + 10 > 40) ? 0
                                                : Para_min + 10;
        }
        // ��ť����ģʽ
        else
        {
            if (Key_Down == 9)
                Para_mode = 1;
            if (Key_Down == 8)
                Para_mode = 2;
        }
    }
}
/* ����ܴ����� */
void Seg_Proc()
{
    uchar i;
    if (Seg_Slow_Down)
        return;
    Seg_Slow_Down = 1;
    Distance_value = Ut_Wave_Data();
    // ����������
    if ((Distance_value < Para_min || Distance_value > Para_max))
    {
        // û�б�����ʱ��
        if (Wring_flag == 0)
        {
            Wring_count++;
            Wring_flag = 1;
        }
    }
    // û�г��������ޣ����ͱ���flag��׼����һ�α���
    else
    {
        Wring_flag = 0;
    }
    // �����Ǵ�����ť���ƲŽ��вɼ���ѹ
    if (Control_mode == 1)
    {
        Vol_level = Ad_Read(0x03) / 51;
        Vol_level = (Vol_level >= 4) ? 4 : Vol_level; // ����һ�£���ֹ�����ж�5�ļ������
        if (Para_mode == 1)
            Para_max = Vol_level * 10 + 50;
        else if (Para_mode == 2)
            Para_min = Vol_level * 10;
    }
    switch (Seg_show_mode)
    {
    case 0:
        /* ������ */
        Seg_Buf[0] = 11; // A
        for (i = 1; i < 5; i++)
        {
            Seg_Buf[i] = 10;
        }
        Seg_Buf[5] = (Distance_value / 100 % 10 == 0) ? 10
                                                      : Distance_value / 100 % 10;
        Seg_Buf[6] = ((Distance_value / 10 % 10 == 0) && (Seg_Buf[5] == 10)) ? 10
                                                                             : Distance_value / 10 % 10;
        Seg_Buf[7] = Distance_value % 10;
        break;

    case 1:
        /* �������� */
        Seg_Buf[0] = 12; // P
        Seg_Buf[1] = Control_mode + 1;
        Seg_Buf[2] = 10;
        Seg_Buf[3] = Para_min / 10 % 10;
        Seg_Buf[4] = Para_min % 10;
        Seg_Buf[5] = 13; //-
        Seg_Buf[6] = Para_max / 10 % 10;
        Seg_Buf[7] = Para_max % 10;
        break;

    case 2:
        /* ��¼���� */
        Seg_Buf[0] = 14; // E
        for (i = 1; i < 7; i++)
        {
            Seg_Buf[i] = 10;
        }
        Seg_Buf[7] = (Wring_count > 9) ? 13
                                       : Wring_count;
        break;
    }
}

/* LED������ */
void Led_Proc()
{
    ucLed[0] = (Seg_show_mode == 0);
    ucLed[1] = (Seg_show_mode == 1);
    ucLed[2] = (Seg_show_mode == 2);
    ucLed[7] = (Wring_flag == 0) ? 1
                                 : Led_show_flag;
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
    if (Wring_flag)
    {
        if (++time_100ms == 100)
        {
            time_100ms = 0;
            Led_show_flag ^= 1;
        }
    }
    else
    {
        time_100ms = 0;
    }
    Seg_Disp(Seg_Pos, Seg_Buf[Seg_Pos], Seg_Point[Seg_Pos]);
    Led_Disp(Seg_Pos, ucLed[Seg_Pos]);
}

void main()
{
    System_Init();
    Timer0_Init();
    while (1)
    {
        Key_Proc();
        Seg_Proc();
        Led_Proc();
    }
}