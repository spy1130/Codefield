#include "main.h"

/* ���� */
/* ���̷��� */
uchar Key_Slow_Down;

/* LED ����*/
uchar ucLed[8] = {0, 0, 0, 0, 0, 0, 0, 0};
/* ����ܷ��� */
uint Seg_Slow_Down;
uchar Seg_Pos;
uchar Seg_Buf[8] = {5, 10, 10, 10, 10, 10, 10, 10};
uchar Seg_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/* ʱ�� */
uchar ucRtc[3] = {0x11, 0x59, 0x56};
uint time_1s;
uint time_2s;
uint time_3s;
uchar time_100ms;
/* ���� */
uchar Seg_show_mode; // 0 ʱ�� 1 ���� 2 ����
uchar re_show_mode;  // 0 �¶� 1 ʪ�� 2 ʱ��

/* ���� */
uchar max_temperature;       // ����¶�
uint aver_temperature_10x;   // ƽ���¶�
uint old_temperature_10x;    // ��һ�ε��¶�
uchar max_humidity;          // ���ʪ��
uint aver_humidity_10x;      // ƽ��ʪ��
uint old_humidity_10x;       // ��һ�ε�ʪ��
uchar trigger_count;         // ��������
uchar trigger_time[3];       // ����ʱ��
uchar para_temperature = 30; // �¶Ȳ���
uint freq;

/* ����жϲ��� */
uchar old_light_value;   // ��һ�εĹ�ǿ����
bit trigger_flag;        // ������־
bit long_press_flag;     // ������־
bit wring_flag;          // �¶ȳ�����������
bit Led_light_flag;      // ��˸
bit error_humidity_flag; // ʪ���Ƿ���Ч����ЧΪ1
bit data_up;             // ��ʪ�Ⱦ�����

/* ���� */
/* ������� */
void Clear_Data(void)
{
    uchar i;
    max_temperature = 0;      // ����¶�
    aver_temperature_10x = 0; // ƽ���¶�
    max_humidity = 0;         // ���ʪ��
    aver_humidity_10x = 0;    // ƽ��ʪ��
    trigger_count = 0;        // ��������
    for (i = 0; i < 3; i++)
        trigger_time[i] = 0; // ����ʱ��
}
/* ����ܸ�λ */
void Seg_Reset(void)
{
    uchar i;
    for (i = 0; i < 8; i++)
    {
        Seg_Buf[i] = 10;
        Seg_Point[i] = 0;
    }
}
/* ʪ�ȼ��� */
float rd_humidity(void)
{
    float humidity_value;
    if (freq < 200 || freq > 2000)
        return 0;
    humidity_value = (freq - 200) * 2 / 45 + 10;
    return humidity_value;
}

/* ���̴��� */
void Key_Porc(void)
{
    static uchar Key_Val, Key_Down, Key_Old, Key_Up;
    if (Key_Slow_Down)
        return;
    Key_Slow_Down = 1;

    Key_Val = Key_Read();
    Key_Down = Key_Val & (Key_Old ^ Key_Val);
    Key_Up = ~Key_Val & (Key_Old ^ Key_Val);
    Key_Old = Key_Val;

    if (Key_Down == 4)
    {
        Seg_Reset();
        Seg_show_mode = (++Seg_show_mode) % 3;
        re_show_mode = 0;
    }
    if (Seg_show_mode == 1 && Key_Down == 5)
    {
        Seg_Reset();
        re_show_mode = (++re_show_mode) % 3;
    }
    if (Seg_show_mode == 2)
    {
        if (Key_Down == 8)
            para_temperature = (++para_temperature > 99) ? 99 : para_temperature;
        else if (Key_Down == 9)
            para_temperature = (--para_temperature < 0) ? 0 : para_temperature;
    }
    if (Seg_show_mode == 1 && re_show_mode == 2)
    {
        if (Key_Down == 9)
        {
            long_press_flag = 1;
        }
        if (Key_Up == 9)
        {
            if (time_2s >= 2000)
                Clear_Data();
            long_press_flag = 0;
        }
    }
}

/* ����ܴ��� */
void Seg_Porc(void)
{
    uint temp_temperature_10x, temp_humidity_10x;
    uchar temp_light;
    if (Seg_Slow_Down)
        return;
    Seg_Slow_Down = 1;
    temp_light = Ad_Read(0x01);
    // �����ظ�����
    if ((old_light_value > 50) &&
        (temp_light < 50) &&
        (trigger_flag == 0))
    {
        trigger_flag = 1;
        if (++trigger_count == 100)
            trigger_count = 99;
    }

    // �����������ҵ���3s
    else if (trigger_flag == 1 && time_3s >= 3000)
        trigger_flag = 0;
    old_light_value = temp_light;
    if (trigger_flag)
    {
        Seg_Buf[1] = Seg_Buf[2] = 10;
        Seg_Point[6] = 0;
        /* ��ʪ�Ƚ��� */
        Seg_Buf[0] = 16; // E
        temp_temperature_10x = rd_temperature() * 10;
        temp_humidity_10x = rd_humidity() * 10;
        Seg_Buf[3] = temp_temperature_10x / 100 % 10;
        Seg_Buf[4] = temp_temperature_10x / 10 % 10;
        Seg_Buf[5] = 11; //-
        // ��Ч����
        if (temp_humidity_10x == 0)
        {
            trigger_count--;
            error_humidity_flag = 1;
            Seg_Buf[6] = 17; // A
            Seg_Buf[7] = 17; // A
        }
        // ��Ч����
        else
        {
            error_humidity_flag = 0; // ������Ч
            wring_flag = (temp_temperature_10x > para_temperature * 10);
            if ((trigger_count >= 2) &&
                (temp_humidity_10x > old_humidity_10x) &&
                (temp_temperature_10x > old_temperature_10x))
                data_up = 1;
            else
                data_up = 0;
            old_humidity_10x = temp_humidity_10x;
            old_temperature_10x = temp_temperature_10x;
            Seg_Buf[6] = temp_humidity_10x / 100 % 10;
            Seg_Buf[7] = temp_humidity_10x / 10 % 10;
            max_temperature = (max_temperature > temp_temperature_10x) ? max_temperature : temp_temperature_10x;
            max_humidity = (max_humidity > temp_humidity_10x) ? max_humidity : temp_humidity_10x;
            aver_temperature_10x = (aver_temperature_10x * (trigger_count - 1) + temp_temperature_10x) / (trigger_count);
            aver_humidity_10x = (aver_humidity_10x * (trigger_count - 1) + temp_humidity_10x) / (trigger_count);
        }
    }
    else
    {
        switch (Seg_show_mode)
        {
        case 0:
            /* ʱ�� */
            Read_Rtc(ucRtc);
            Seg_Buf[0] = ucRtc[0] / 16;
            Seg_Buf[1] = ucRtc[0] % 16;
            Seg_Buf[2] = 11; //-
            Seg_Buf[3] = ucRtc[1] / 16;
            Seg_Buf[4] = ucRtc[1] % 16;
            Seg_Buf[5] = 11; //-
            Seg_Buf[6] = ucRtc[2] / 16;
            Seg_Buf[7] = ucRtc[2] % 16;
            break;
        case 1:
            /* ���� */
            switch (re_show_mode)
            {
            case 0:
                /* �¶� */
                Seg_Buf[0] = 12; // C
                if (trigger_count != 0)
                {
                    Seg_Buf[2] = max_temperature / 10 % 10;
                    Seg_Buf[3] = max_temperature % 10;
                    Seg_Buf[4] = 11; //-
                    Seg_Buf[5] = aver_temperature_10x / 100 % 10;
                    Seg_Buf[6] = aver_temperature_10x / 10 % 10;
                    Seg_Buf[7] = aver_temperature_10x % 10;
                    Seg_Point[6] = 1;
                }
                break;
            case 1:
                /* ʪ�� */
                Seg_Buf[0] = 13; // H
                if (trigger_count != 0)
                {
                    Seg_Buf[2] = max_humidity / 10 % 10;
                    Seg_Buf[3] = max_humidity % 10;
                    Seg_Buf[4] = 11; //-
                    Seg_Buf[5] = aver_humidity_10x / 100 % 10;
                    Seg_Buf[6] = aver_humidity_10x / 10 % 10;
                    Seg_Buf[7] = aver_humidity_10x % 10;
                    Seg_Point[6] = 1;
                }
                break;
            case 2:
                /* ʱ�� */
                Seg_Buf[0] = 14; // F
                Seg_Buf[1] = trigger_count / 10 % 10;
                Seg_Buf[2] = trigger_count % 10;
                if (trigger_count != 0)
                {
                    Seg_Buf[3] = trigger_time[0] / 16;
                    Seg_Buf[4] = trigger_time[0] % 16;
                    Seg_Buf[5] = 11; //-
                    Seg_Buf[6] = trigger_time[1] / 16;
                    Seg_Buf[7] = trigger_time[1] % 16;
                }
                break;
            }
            break;
        case 2:
            /* ���� */
            Seg_Buf[0] = 15; // P
            Seg_Buf[6] = para_temperature / 10 % 10;
            Seg_Buf[7] = para_temperature % 10;
            break;
        }
    }
}

/* LED���� */
void Led_Porc(void)
{
    /* ���� */
    ucLed[0] = (Seg_show_mode == 0);
    ucLed[1] = (Seg_show_mode == 1);
    ucLed[2] = (Seg_show_mode == 2);
    /* ���� */
    ucLed[3] = (wring_flag) ? Led_light_flag : 0;
    ucLed[4] = error_humidity_flag;
    ucLed[5] = data_up;
}

/* ��ʱ��/���������� */
/* ������ */
void Timer0_Init(void) // 1����@12.000MHz
{
    AUXR &= 0x7F; // ��ʱ��ʱ��12Tģʽ
    TMOD &= 0xF0; // ���ö�ʱ��ģʽ
    TMOD |= 0x05; // ���ö�ʱ��ģʽ
    TL0 = 0;      // ���ö�ʱ��ʼֵ
    TH0 = 0;      // ���ö�ʱ��ʼֵ
    TF0 = 0;      // ���TF0��־
    TR0 = 1;      // ��ʱ��0��ʼ��ʱ
}
/* ��ʱ�� */
void Timer1_Init(void) // 1����@12.000MHz
{
    AUXR &= 0xBF; // ��ʱ��ʱ��12Tģʽ
    TMOD &= 0x0F; // ���ö�ʱ��ģʽ
    TL1 = 0x18;   // ���ö�ʱ��ʼֵ
    TH1 = 0xFC;   // ���ö�ʱ��ʼֵ
    TF1 = 0;      // ���TF1��־
    TR1 = 1;      // ��ʱ��1��ʼ��ʱ
    ET1 = 1;      // ����ʱ��1�ж�
    EA = 1;       // �������ж�
}
/* ��ʱ�������� */
void Timer1_ISR(void) interrupt 3
{
    if (++Key_Slow_Down == 10)
        Key_Slow_Down = 0;
    if (++Seg_Slow_Down == 200)
        Seg_Slow_Down = 0;
    if (++Seg_Pos == 8)
        Seg_Pos = 0;
    if (++time_1s == 1000)
    {
        time_1s = 0;
        freq = TH0 << 8 | TL0;
        TH0 = 0;
        TL0 = 0;
    }
    if (trigger_flag)
    {
        if (++time_3s >= 3000)
        {
            time_3s = 3001;
        }
    }
    else
        time_3s = 0;
    // ��ʼ������ʱ
    if (long_press_flag)
    {
        if (++time_2s >= 2000)
        {
            time_2s = 2001;
        }
    }
    else
        time_2s = 0;
    // ���ڱ���

    if (++time_100ms == 100)
    {
        time_100ms = 0;
        Led_light_flag ^= 1;
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
    Timer0_Init();
    Timer1_Init();
    Set_Rtc(ucRtc);
    Delay750ms();
    rd_temperature();
    while (1)
    {
        Key_Porc();
        Seg_Porc();
        Led_Porc();
    }
}