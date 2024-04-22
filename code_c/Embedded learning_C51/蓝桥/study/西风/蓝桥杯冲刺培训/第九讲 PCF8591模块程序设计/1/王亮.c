/*ͷ�ļ�������*/
#include <STC15F2K60S2.H>//��Ƭ���Ĵ���ר��ͷ�ļ�
#include <init.H>//��ʼ���ײ�����ר��ͷ�ļ�
#include <key.H>//�����ײ�����ר��ͷ�ļ�
#include <led.H>//LED�ײ�����ר��ͷ�ļ�
#include <seg.H>//����ܵײ�����ר��ͷ�ļ�
#include <iic.h>//PCF8591�ײ�����ר��ͷ�ļ�

/*����������*/
unsigned char key_slow_down,seg_slow_down;//����ר�ñ���
unsigned char key_val,key_down,key_up,key_old;//�������ר�ñ���
unsigned char seg_buf[8]={10,10,10,10,10,10,10,10};//�������ʾ���ݴ������
unsigned char seg_point[8]={0,0,0,0,0,1,0,0};//С������ʾ���ݴ������
unsigned char led_buf[8]={0,0,0,0,0,0,0,0};//LED�������ݴ������
unsigned char pos;//���ݴ������ר��ָ��
bit seg_mord;//�������ʾ�����־λ 0-��ѹ��ʾ���� 1-��ѹ�������
float dat;//������ѹֵר�ñ���
bit dat_flag;//��ѹֵ��־λ 0-2.00 1-RB2
bit led_flag=1;//LEDʹ�ܱ�־λ 0-�ر� 1-����
bit seg_flag=1;//�����ʹ�ܱ�־λ 0-�ر� 1-����

/*����������*/
void key_proc()
{
	//�������ٳ���
	if(key_slow_down)	return;
	key_slow_down = 1;
	
	//����������
	key_val = key_read();//ʵʱ��ȡ����ֵ
	key_down = key_val&(key_val^key_old);//���񰴼��½���
	key_up = ~key_val&(key_val^key_old);//���񰴼�������
	key_old = key_val;//����ɨ�谴��
	
	//�������ó���
	switch(key_down)
	{
		case 4:
			seg_mord ^= 1;
		break;
		case 5:
			dat_flag ^= 1;
		break;
		case 6:
			led_flag ^= 1;
		break;
		case 7:
			seg_flag ^= 1;
		break;
	}
}

/*��Ϣ������*/
void seg_proc()
{
	//����ܼ��ٳ���
	if(seg_slow_down)	return;
	seg_slow_down = 1;
	
	//PCF8591���ó���
	dat = ad_read(0x43)/51.0;//���ɼ������ݸ�����ѹֵ
	
	//�������ʾ����
	if(seg_mord)//���ڵ�ѹ�������
	{
		seg_buf[0] = 12;//��ʶ����ʾU
		if(dat_flag)
		{
			seg_buf[5] = (unsigned char)dat;
			seg_buf[6] = (unsigned char)(dat*100)/10%10;
			seg_buf[7] = (unsigned char)(dat*100)%10;//�������ʾRB2
		}
		else
		{
			seg_buf[5] = 2;
			seg_buf[6] = seg_buf[7] = 0;//�������ʾ2.00
		}
	}
	else//���ڵ�ѹ��ʾ����
	{
		seg_buf[0] = 11;//��ʶ����ʾF
		seg_buf[5] = (unsigned char)dat;
		seg_buf[6] = (unsigned char)(dat*100)/10%10;
		seg_buf[7] = (unsigned char)(dat*100)%10;//�������ʾ������ѹֵ
	}
}

/*������ʾ����*/
void led_proc()
{
	led_buf[0] = seg_mord?0:1;//�ж��������ʾ����,ȷ��L1�Ƿ����
	led_buf[1] = !led_buf[0];//L2״̬��L1�෴
	led_buf[2] = (dat < 1.5||(dat<3.5&&dat>=2.5))?0:1;//�ж������ѹ��С,ȷ��L3�Ƿ����
	led_buf[3] = dat_flag?1:0;//�ж������ѹ״̬,ȷ��L4�Ƿ����

}

/*��ʱ��0��ʼ������*/
void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x18;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;//��ʱ��0�жϴ�
	EA = 1;//���жϴ�
}

/*��ʱ��0�жϷ�����*/
void timer0_server() interrupt 1
{
	if(++key_slow_down == 10)	key_slow_down = 0;//��������ר��
	if(++seg_slow_down == 500)	seg_slow_down = 0;//����ܼ���ר��
	if(++pos == 8)	pos = 0;//���ݴ������ɨ��ר��
	
	//�������ʾר��
	if(seg_flag)
		seg_disp(pos,seg_buf[pos],seg_point[pos]);
	else
		seg_disp(pos,10,0);
	
	//LED����ר��
	if(led_flag)
		led_disp(pos,led_buf[pos]);
	else
		led_disp(pos,0);
}

/*������*/
void main()
{
	system_init();
	Timer0Init();
	while(1)
	{
		key_proc();
		seg_proc();
		led_proc();
	}
}