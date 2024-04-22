/*ͷ�ļ���*/
#include <STC15F2K60S2.H>
#include <Init.H>//��Ƭ����ʼ���ײ�
#include <Led.H>//Led�������ײ�
#include <Key.H>//���������ײ�
#include <Seg.H>//����������ײ�
#include <iic.H>//EEPROM����ģת�������ײ�

/*�Զ������*/
unsigned char Key_Slow_Down;//��������ר�ñ���
unsigned char Key_Val,Key_Down,Key_Old;
unsigned int Seg_Slow_Down;//����ܼ���ר�ñ���
unsigned char Pos;//ɨ�����
unsigned char Seg_Buf[8] = {10,10,10,10,10,10,10,10};//����ܶ�ѡֵ�洢����
unsigned char ucLed[8] = {0,0,0,0,0,0,0,0};//Led��״ֵ̬�洢����
unsigned char Led_Mode;//Led��ģʽ���� 0-ģʽ1 1-ģʽ2 2-ģʽ3 3-ģʽ4
bit Led_Flag ;//Led����ת��־λ 0-��ͣ 1-����
unsigned int STC_Timer;//ϵͳ��ʱ����
unsigned int Led_Timer_Sto[4] = {400,400,400,400};//��תʱ��洢���� 400-1200
unsigned int Led_Timer_Set[4];//��תʱ����������
unsigned char Timer_Set_Index;//��ת�������ָ��
unsigned int Timer_400ms;//400����ʱ�����
bit Seg_Star_Flag;//�������˸��־λ
unsigned char EEPROM_String[4] = {4,4,4,4};//EEPROM���ݴ������
unsigned char dat;//����ADת����ֵ
unsigned char Led_Rank;//���ȵȼ�
unsigned char Led_Count;//����ͳ��
bit Key_Flag;//������־λ


/*����ɨ�躯��*/
void Key_Proc()
{
	unsigned char k;
	
	if(Key_Slow_Down) return;
	else Key_Slow_Down = 1;//�������ٳ���
	
	Key_Val = Key_Read();//��ȡ����ֵ
	Key_Down = Key_Val ^ (Key_Val & Key_Old);//����½���
	Key_Old = Key_Val;//��������
	
	if(Timer_Set_Index == 0 && Key_Old == 4)
	{
		Key_Flag = 1;
	}
	else
		Key_Flag = 0;
	
	switch(Key_Down)
	{
		case 7://����/ֹͣ��ť
			Led_Flag ^= 1;
		break;
		case 6://����
			if(++Timer_Set_Index == 3)
				Timer_Set_Index = 0;
			if(Timer_Set_Index == 1)//������ת������ý���
			{
				for( k = 0; k < 4; k++)
				   Led_Timer_Set[k] = Led_Timer_Sto[k];
			}
			else if(Timer_Set_Index == 0)//�˳���ת������ý���
			{
				for( k = 0; k < 4; k++)
				{
				    Led_Timer_Sto[k] = Led_Timer_Set[k];
					EEPROM_String[0] = Led_Timer_Set[0] / 100;
				}
				EEPROM_Write(EEPROM_String,0,4);				
			}		
		break;
		case 5://��
			if(Timer_Set_Index)//���ý���
			{
				if(Timer_Set_Index == 1)
				{
					if(++Led_Mode == 4)
						Led_Mode = 3;
				}
				else if(Timer_Set_Index == 2)
				{	
					if(Led_Timer_Set[Led_Mode] == 1200)
						Led_Timer_Set[Led_Mode] = 1200;
					else
						Led_Timer_Set[Led_Mode] += 100;					
				}
		   }
		break;
		case 4://��
			if(Timer_Set_Index)//���ý���
			{				
				if(Timer_Set_Index == 1)
				{
					if(--Led_Mode == 255)
						Led_Mode = 0;
				}
				else if(Timer_Set_Index == 2)
				{
					if(Led_Timer_Set[Led_Mode] == 400)
						Led_Timer_Set[Led_Mode] = 400;
					else
						Led_Timer_Set[Led_Mode] -= 100;
				}
		    }			
		break;
	}
}

/*��Ϣ������*/
void Seg_Proc()
{
	if(Seg_Slow_Down) return;
	else Seg_Slow_Down = 1;//����ܼ��ٳ���
	
	/*ʵʱ��ȡADֵ*/
	dat = AD_Read(0x43);	
	
    /*�������ʾ����*/
	if(Key_Flag == 0)
	{
		Seg_Buf[0] = Seg_Buf[2] = 11;//-
		Seg_Buf[1] = Led_Mode + 1;
		Seg_Buf[4] = Led_Timer_Set[Led_Mode] / 1000 ? Led_Timer_Set[Led_Mode] / 1000 : 10;
		Seg_Buf[5] = Led_Timer_Set[Led_Mode] /100 % 10;
		Seg_Buf[6] = Led_Timer_Set[Led_Mode] /10 % 10;	
		Seg_Buf[7] = Led_Timer_Set[Led_Mode] % 10;
		
		if(Timer_Set_Index == 1)
			Seg_Buf[1] = Seg_Star_Flag ? Led_Mode + 1 : 10;	
		else if(Timer_Set_Index == 2)
		{
			Seg_Buf[4] = Seg_Star_Flag ? (Led_Timer_Set[Led_Mode] / 1000 ? Led_Timer_Set[Led_Mode] / 1000 : 10) : 10;
			Seg_Buf[5] = Seg_Star_Flag ? (Led_Timer_Set[Led_Mode] /100 % 10) : 10;
			Seg_Buf[6] = Seg_Star_Flag ? (Led_Timer_Set[Led_Mode] /10 % 10) : 10;	
			Seg_Buf[7] = Seg_Star_Flag ? (Led_Timer_Set[Led_Mode] % 10) : 10;
		}
	}	
	else
	{
		Seg_Buf[0] = 10;
		Seg_Buf[1] = 10;
		Seg_Buf[2] = 10;
		Seg_Buf[4] = 10;
		Seg_Buf[5] = 10;		
		Seg_Buf[6] = 11;	
		Seg_Buf[7] = Led_Rank / 3;
		
	}		
	
}

/*������ʾ����*/
void Led_Proc()
{
	unsigned char i;
	static unsigned char j = 0;
	
	if((float)dat / 63.75 >= 3) Led_Rank = 12;
	if((float)dat / 63.75 >= 2 && (float)dat / 63.75 < 3) Led_Rank = 9;
	if((float)dat / 63.75 >= 1 && (float)dat / 63.75 < 2) Led_Rank = 6;
	if((float)dat / 63.75 >= 0 && (float)dat / 63.75 < 1) Led_Rank = 3;	
		
	if(STC_Timer == Led_Timer_Sto[Led_Mode])//ϵͳ��ʱ���ڶ�Ӧ��תģʽ����תʱ��
	{
		STC_Timer = 0;
		switch(Led_Mode)
		{
			case 0://ģʽ1
				for(i = 0; i < 8; i++)
				{
					ucLed[i] = (i == j);
				}
				j++;				
				if(j == 8)
				{
					j--;					
					Led_Mode = 1;
				}				
			break;
			case 1://ģʽ2				
				for(i = 0; i < 8; i++)
				{
					ucLed[i] = (i == j);
				}
				j--;
				if(j == 255)
				{
					j = 0;
					Led_Mode = 2;
				}
			break;
			case 2://ģʽ3
				for(i = 0; i < 4; i++)
				{
					ucLed[i] = (i == j);
					ucLed[7-i] = (i == j);
				}
				j++;				
				if(j == 4)
				{
					j--;					
					Led_Mode = 3;
				}					
			break;
			case 3://ģʽ4
				for(i = 0; i < 4; i++)
				{
					ucLed[i] = (i == j);
					ucLed[7-i] = (i == j);					
				}
				j--;
				if(j == 255)
				{
					j = 0;
					Led_Mode = 0;
				}			
			break;
		}
	}
}



/*��ʱ��0�жϳ�ʼ������*/
void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x18;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
	EA = 1;
}

/*��ʱ���жϷ�����*/
void Timer_Seriver() interrupt 1
{
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;
	
	if(++Pos == 8) Pos = 0;
	if(Timer_Set_Index || Key_Flag == 1)
	   Seg_Disp(Pos,Seg_Buf[Pos]);
	else
	   Seg_Disp(Pos,10);
	
	if(Led_Flag == 1)
		STC_Timer++;
	
	if(++Timer_400ms == 400)
	{
		Timer_400ms = 0;
		Seg_Star_Flag ^= 1;
	}
	
	if(++Led_Count == 12)
		Led_Count = 0;
	if(Led_Count < Led_Rank)
	    Led_Disp(Pos,ucLed[Pos]);
	else
	    Led_Disp(Pos,0);
	
}

/*������*/
void main()
{
	unsigned char k;	
	EEPROM_Read(EEPROM_String,0,4);
	for( k = 0; k < 4; k++)
	    Led_Timer_Sto[k] = EEPROM_String[k] * 100;
	Init_STC();
	Timer0Init();
	while(1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}
	
