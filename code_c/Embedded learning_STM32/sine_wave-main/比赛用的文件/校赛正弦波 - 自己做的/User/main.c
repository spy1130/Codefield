#include "Tim.h"
#include "delay.h"
#include "sys.h"
#include "dma.h"
#include "dac.h"
#include "key.h"
#include "lcd.h"
#include "dac.h"
#include "math.h"


//输入显示屏的字符串变量,必须是char类型不能是unsigned char型
u16 SineWave[4096]; 
#define pi  3.1415926535
u16 nums=100;
u16 fre=10;//频率
u8 fre_flag = 0;
u8 mode=1;//初始状态为1hz模式
typedef enum{
	 mode1_1 = 1,
	 mode2_10 = 2,
	 mode3_1000 = 3
}Mode_key;



/**
   * @brief 
   * @param maxval纵坐标数字量实时DAC输出的数字量 0<maxval<2048
   * @param samples每个周期采样点个数
   * @param 正弦波公式 y = Asin(wx+φ) +b
   * @retval 无
   * @fuctper函数功能 输出正弦波
   w = 2π/samples 采样频率
   最大的数字量能使完成半个周期的采样。及就是samples/2<maxval
   最大采样点个数为4096个
   */

void creat_sin_wave(uint16_t maxval,uint16_t samples)//发生正弦波
{
	uint8_t i;
	float digltal_value;//数字量存储到SineWave_Value数组中
	float w = (2*pi) / samples;// 周期/采样点个数=采样频率
//	if(maxval<=((samples / 2))) return;//采样过多，需要示波器验证结果
	for(i = 0; i< samples;i++){
		digltal_value = maxval*sin(w*i)+maxval;//向上偏移半个幅值。使SineWave_Value数组中的数字量为正
		if(digltal_value > 4095)
		digltal_value = 4095;/*上限限定*/
		SineWave[i] = digltal_value;//接受数字量
	}
}



void output_sin_wave(uint16_t psc,uint16_t arr,u16 nums)
{
	creat_sin_wave(2048,nums);
	dac_dma_wave_enable(psc,arr);
}
u8 key_up=0;//松手标志位
u16 psc=71;
u16 arr;
void Key_Proc()//按键控制模式变换
{
	u8 key;
	key = KEY_Scan(0);
    if(key==0&&key_up==WKUP_PRES)//三种模式切换
	{
		mode++;
		if(mode>3){
			mode=1;
		LCD_Clear(WHITE);//清屏
		}
	}
	if(mode==1)
	{//模式1，每次变1hz
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
	else if(mode==2){//模式2，每次变100hz
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
	else if(mode==3){//模式3，每次变1000hz
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
	if(fre>10000)//设置频率上下限
	{
		fre = 10000;
	}
	if(fre<10)
	{
		fre = 10;
	}
	
	LCD_ShowxNum(40,70,fre,16,16,0);//显示频率
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
	key_up = key;//松手
}


int main(void)
{
	
	delay_init();
	
	LCD_Init();
	DAC_Config();
	KEY_Init();
	DMA_Config();
	POINT_COLOR=RED;//设置字体为红色
	output_sin_wave(72-1,1000-1,100);//初始状态10hz
	while(1){
	Key_Proc();
	}
}
