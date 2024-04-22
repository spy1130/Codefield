#include "Tim.h"
#include "delay.h"
#include "sys.h"
#include "dma.h"
#include "dac.h"
#include "key.h"
#include "lcd.h"
#include "dac.h"
#include "math.h"


//������ʾ�����ַ�������,������char���Ͳ�����unsigned char��
u16 SineWave[4096]; 
#define pi  3.1415926535
u16 nums=100;
u16 fre=10;//Ƶ��
u8 fre_flag = 0;
u8 mode=1;//��ʼ״̬Ϊ1hzģʽ
typedef enum{
	 mode1_1 = 1,
	 mode2_10 = 2,
	 mode3_1000 = 3
}Mode_key;



/**
   * @brief 
   * @param maxval������������ʵʱDAC����������� 0<maxval<2048
   * @param samplesÿ�����ڲ��������
   * @param ���Ҳ���ʽ y = Asin(wx+��) +b
   * @retval ��
   * @fuctper�������� ������Ҳ�
   w = 2��/samples ����Ƶ��
   ������������ʹ��ɰ�����ڵĲ�����������samples/2<maxval
   �����������Ϊ4096��
   */

void creat_sin_wave(uint16_t maxval,uint16_t samples)//�������Ҳ�
{
	uint8_t i;
	float digltal_value;//�������洢��SineWave_Value������
	float w = (2*pi) / samples;// ����/���������=����Ƶ��
//	if(maxval<=((samples / 2))) return;//�������࣬��Ҫʾ������֤���
	for(i = 0; i< samples;i++){
		digltal_value = maxval*sin(w*i)+maxval;//����ƫ�ư����ֵ��ʹSineWave_Value�����е�������Ϊ��
		if(digltal_value > 4095)
		digltal_value = 4095;/*�����޶�*/
		SineWave[i] = digltal_value;//����������
	}
}



void output_sin_wave(uint16_t psc,uint16_t arr,u16 nums)
{
	creat_sin_wave(2048,nums);
	dac_dma_wave_enable(psc,arr);
}
u8 key_up=0;//���ֱ�־λ
u16 psc=71;
u16 arr;
void Key_Proc()//��������ģʽ�任
{
	u8 key;
	key = KEY_Scan(0);
    if(key==0&&key_up==WKUP_PRES)//����ģʽ�л�
	{
		mode++;
		if(mode>3){
			mode=1;
		LCD_Clear(WHITE);//����
		}
	}
	if(mode==1)
	{//ģʽ1��ÿ�α�1hz
		LCD_ShowString(30,50,200,16,16,"Fre_var:1hz");
		if(key==KEY0_PRES)
		{
			fre++;
			fre_flag = 1;
			}
		 if(key==KEY1_PRES)
		{
			fre--;
			fre_flag = 1;
			}
	}
	else if(mode==2){//ģʽ2��ÿ�α�100hz
		LCD_ShowString(30,50,200,16,16,"Fre_var:100hz");
		if(key==KEY0_PRES)
		{
			fre+=100;
			fre_flag = 1;
			}
		 if(key==KEY1_PRES)
		{
			fre-=100;
			fre_flag = 1;
			}
		}
	else if(mode==3){//ģʽ3��ÿ�α�1000hz
		LCD_ShowString(30,50,200,16,16,"Fre_var:1000hz");
		if(key==KEY0_PRES)
		{
			fre+=1000;
			fre_flag = 1;
			}
		 if(key==KEY1_PRES)
		{
			fre-=1000;
			fre_flag = 1;
			}
		}
	if(fre>10000)//����Ƶ��������
	{
		fre = 10000;
	}
	if(fre<10)
	{
		fre = 10;
	}
	
	LCD_ShowxNum(40,70,fre,16,16,0);//��ʾƵ��
	LCD_ShowString(16,70,200,16,16,"fre:");
	if(fre_flag)
	{
		if(fre<=2500)
		{	
			nums = 100;
			arr = 10000/fre -1;
		}
		else if(fre>2500)
		{
			arr = 4-1;
			nums = 250000/fre;
		}
		output_sin_wave(psc,arr,nums);
		fre_flag =0;
	}
	key_up = key;//����
}


int main(void)
{
	
	delay_init();
	
	LCD_Init();
	DAC_Config();
	KEY_Init();
	DMA_Config();
	POINT_COLOR=RED;//��������Ϊ��ɫ
	output_sin_wave(72-1,1000-1,100);//��ʼ״̬10hz
	while(1){
	Key_Proc();
	}
}
