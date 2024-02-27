#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
#include "text.h"

#include "math.h"
#include "DataScope_DP.h"

#include "oled.h"
#include <string.h>
#include "main.h"


extern int counter_t2;//1# channel
extern int over_flow_t2;//1# channel

extern int counter_t3;//2# channel
extern int over_flow_t3;//2# channel

extern int counter_t4;//3# channel
extern int over_flow_t4;//3# channel

extern int counter_t1;//4# channel
extern int over_flow_t1;//4# channel

extern int counter_t8;//5# channel
extern int over_flow_t8;//5# channel

extern int black_frequency_1;//1# channel transmission poto 
extern int black_frequency_2;//2# channel
extern int black_frequency_3;//3# channel
extern int black_frequency_4;//4# channel
extern int black_frequency_5;//5# channel


extern int display_flag; 



/************************************************
 ALIENTEK精英STM32开发板实验13
 TFTLCD显示实验  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
char buffer[20];
int count_delay=0x00;

int chnanel_1_num=0x00;
int chnanel_1_num_end=0x00;
int chanenel_1_measure_end_flag=0x00;

int chnanel_2_num=0x00;
int chnanel_2_num_end=0x00;
int chanenel_2_measure_end_flag=0x00;

int chnanel_3_num=0x00;
int chnanel_3_num_end=0x00;
int chanenel_3_measure_end_flag=0x00;

int chnanel_4_num=0x00;
int chnanel_4_num_end=0x00;
int chanenel_4_measure_end_flag=0x00;

int chnanel_5_num=0x00;
int chnanel_5_num_end=0x00;
int chanenel_5_measure_end_flag=0x00;

u8 measure_end_flag=0;

u8 measure_start_flag=0;

u8 touch_left_flag=0;

int theta0_flag=0x00;//jump_down_flag

int i_clear=0;

double chanel_history_1[600]={0};
double chanel_history_2[600]={0};
double chanel_history_3[600]={0};
double chanel_history_4[600]={0};
double chanel_history_5[600]={0};

int jump_down_flag_1=0x00;
int jump_down_flag_2=0x00;
int jump_down_flag_3=0x00;
int jump_down_flag_4=0x00;
int jump_down_flag_5=0x00;

//double buffer_min;
//double buffer_max;

double black_value_max=0x00;
double black_value_min=0x100;

double black_value_max_1=0x00;
double black_value_min_1=0x100;

double black_value_max_2=0x00;
double black_value_min_2=0x100;

double black_value_max_3=0x00;
double black_value_min_3=0x100;

double black_value_max_4=0x00;
double black_value_min_4=0x100;

double black_value_max_5=0x00;
double black_value_min_5=0x100;

double black_value_1=0x00;//after adjust blace value
double black_value_2=0x00;
double black_value_3=0x00;
double black_value_4=0x00;
double black_value_5=0x00;

double black_value_1_last=0x00;
double black_value_2_last=0x00;
double black_value_3_last=0x00;
double black_value_4_last=0x00;
double black_value_5_last=0x00;

double black_value_original_1=0x00;//orginal black value
double black_value_original_2=0x00;
double black_value_original_3=0x00;
double black_value_original_4=0x00;
double black_value_original_5=0x00;

double black_frequency0_1=1;//dirrectly irradite frequency
double black_frequency0_2=1;
double black_frequency0_3=1;
double black_frequency0_4=1;
double black_frequency0_5=1;


int multify_100_flag_1=0x00;
int multify_100_flag_2=0x00;
int multify_100_flag_3=0x00;
int multify_100_flag_4=0x00;
int multify_100_flag_5=0x00;

int channel_1_aperture_change=0x00;
int channel_2_aperture_change=0x00;
int channel_3_aperture_change=0x00;
int channel_4_aperture_change=0x00;
int channel_5_aperture_change=0x00;


double  p1_1_1 =        0.03064 ;
double  p2_1_1 =       -0.2018;
double  p3_1_1 =        1.236;
double  p4_1_1 =        -0.1074 ;
double  p5_1_1 =        0;

double  p1_2_1 =        0.03064 ;
double  p2_2_1 =       -0.2018;
double  p3_2_1 =        1.236;
double  p4_2_1 =        -0.1074 ;
double  p5_2_1 =        0;

double  p1_1_2 =       0.02253 ;
double  p2_1_2 =       -0.1295;
double  p3_1_2 =     1.105 ;
double  p4_1_2 =       -0.006545 ;
double  p5_1_2 =        0;

double  p1_2_2 =       0.02253 ;
double  p2_2_2 =       -0.1295;
double  p3_2_2 =     1.105 ;
double  p4_2_2 =       -0.006545 ;
double  p5_2_2 =        0;

double  p1_1_3 =      0.03107  ;
double  p2_1_3 =      -0.2073;
double  p3_1_3 =        1.242 ;
double  p4_1_3 =       -0.1023 ;
double  p5_1_3=        0;

double  p1_2_3 =      0.03107  ;
double  p2_2_3 =      -0.2073;
double  p3_2_3 =        1.242 ;
double  p4_2_3 =       -0.1023 ;
double  p5_2_3=        0;


double  p1_1_4 =        0.02944  ;
double  p2_1_4 =       -0.1844;
double  p3_1_4 =        1.165  ;
double  p4_1_4 =        -0.05134 ;
double  p5_1_4=        0;

double  p1_2_4 =        0.02944  ;
double  p2_2_4 =       -0.1844;
double  p3_2_4 =        1.165  ;
double  p4_2_4 =        -0.05134 ;
double  p5_2_4=        0;


double  p1_1_5 =       0.03519  ;
double  p2_1_5 =        -0.2528;
double  p3_1_5 =        1.354 ;
double  p4_1_5 =      -0.2867;
double  p5_1_5=        0;

double  p1_2_5 =       0.03519  ;
double  p2_2_5 =        -0.2528;
double  p3_2_5 =        1.354 ;
double  p4_2_5 =      -0.2867;
double  p5_2_5=        0;

double  ps1_3 =     -0.09317;
double  ps2_3 =      1.088;
double  ps3_3 =       -0.04817;

int move_state=0;

unsigned char s_i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数

int send_measure_end_flag=0x00;
int	send_photto_begin_flag=0x00;

u8 clc_variable_flag=0x00;
int inf_x=0x000;
int speak_len,speak_i;

int speak_len1=0x00;//(sizeof(buf_xm)/sizeof(buf_xm[0]));

int str_num=0x00;
int count_state_5=0x00;

channel_1_judge_begin_end_save_data()
{
	if(black_value_1>0.346)
	{
		chnanel_1_num++;
		if(chnanel_1_num>300)
			chnanel_1_num=0;
		
		if(chnanel_1_num==0x01)
		{
			black_value_1_last=black_value_1;
			black_value_max_1=0;
			black_value_min_1=100;
			
			for(i_clear=0;i_clear<=599;i_clear++)
			{
				chanel_history_1[i_clear]=0;
			}
			
			speaker_context_1_start();
			
			
		}

		
	}
	
	if(chnanel_1_num>5)
	{
			
		if(black_value_1>0.446)
		{
			chanel_history_1[chnanel_1_num]=black_value_1;
//									chnanel_1_num++;
		}
		else
		{
				chnanel_1_num_end=chnanel_1_num-5;
				if(chanenel_1_measure_end_flag==0x00)
				{
					 chanenel_1_measure_end_flag=0x01;
					for(i_clear=6;i_clear<chnanel_1_num_end;i_clear++)
					{
						if(chanel_history_1[i_clear]>black_value_max_1)
						{
											
							black_value_max_1=chanel_history_1[i_clear];
						}
						
						if(chanel_history_1[i_clear]<black_value_min_1)
						{
					
							black_value_min_1=chanel_history_1[i_clear];
						}	
						
								
					}

						speaker_context_1_end();
					
//					measure_end_flag=0x01;
//					measure_start_flag=0x00;
					
				}
				else
				{
					chanenel_1_measure_end_flag=0x01;
				}
			  
			
				
		}
		


	}
	
}



channel_2_judge_begin_end_save_data()
{
	if(black_value_2>0.45)
	{
		chnanel_2_num++;
		if(chnanel_2_num>600)
			chnanel_2_num=0;
		if(chnanel_2_num==0x01)
		{
			black_value_2_last=black_value_2;
			black_value_max_2=0;
			black_value_min_2=100;
			
			for(i_clear=0;i_clear<=599;i_clear++)
			{
				chanel_history_2[i_clear]=0;
			}
			
			speaker_context_2_start();
		}

		
	}
	
	if(chnanel_2_num>5)
	{
			
		if(black_value_2>0.55)
		{
			chanel_history_2[chnanel_2_num]=black_value_2;
//									chnanel_1_num++;
		}
		else
		{
				chnanel_2_num_end=chnanel_2_num-5;
				
				if(chanenel_2_measure_end_flag==0x00)
				{
					 chanenel_2_measure_end_flag=0x01;
					 speaker_context_2_end();
					for(i_clear=6;i_clear<chnanel_2_num_end;i_clear++)
					{
						if(chanel_history_2[i_clear]>black_value_max_2)
						{
											
							black_value_max_2=chanel_history_2[i_clear];
						}
						
						if(chanel_history_2[i_clear]<black_value_min_2)
						{
					
							black_value_min_2=chanel_history_2[i_clear];
						}	
						
						
						
								
					}	
					
				}
				else
				{
					chanenel_2_measure_end_flag=0x01;
				}
			  
			
				
		}
		


	}
	
}

channel_3_judge_begin_end_save_data()
{
	if(black_value_3>0.43)
	{
		chnanel_3_num++;
		if(chnanel_3_num>600)
			chnanel_3_num=0;
		if(chnanel_3_num==0x01)
		{
			black_value_3_last=black_value_3;
			black_value_max_3=0;
			black_value_min_3=100;
			
			for(i_clear=0;i_clear<=599;i_clear++)
			{
				chanel_history_3[i_clear]=0;
			}
			speaker_context_3_start();
		}

		
	}
	
	if(chnanel_3_num>5)
	{
			
		if(black_value_3>0.53)
		{
			chanel_history_3[chnanel_3_num]=black_value_3;
//									chnanel_1_num++;
		}
		else
		{
				chnanel_3_num_end=chnanel_3_num-5;
				if(chanenel_3_measure_end_flag==0x00)
				{
					 chanenel_3_measure_end_flag=0x01;
					for(i_clear=6;i_clear<chnanel_3_num_end;i_clear++)
					{
						if(chanel_history_3[i_clear]>black_value_max_3)
						{
											
							black_value_max_3=chanel_history_3[i_clear];
						}
						
						if(chanel_history_3[i_clear]<black_value_min_3)
						{
					
							black_value_min_3=chanel_history_3[i_clear];
						}	
					}
					speaker_context_3_end();

					
					//得出最小值
						
					
				}
				else
				{
					chanenel_3_measure_end_flag=0x01;
				}
				
		}
	}
	
}


channel_4_judge_begin_end_save_data()
{
	if(black_value_4>0.368)
	{
		chnanel_4_num++;
		if(chnanel_4_num>600)
			chnanel_4_num=0;
		
		if(chnanel_4_num==0x01)
		{
			black_value_4_last=black_value_4;
			black_value_max_4=0;
			black_value_min_4=100;
			
			for(i_clear=0;i_clear<=599;i_clear++)
			{
				chanel_history_4[i_clear]=0;
			}
			
			speaker_context_4_start();
		}

		
	}
	
	if(chnanel_4_num>5)
	{
			
		if(black_value_4>0.468)
		{
			chanel_history_4[chnanel_4_num]=black_value_4;
//									chnanel_1_num++;
		}
		else
		{
				chnanel_4_num_end=chnanel_4_num-5;
				if(chanenel_4_measure_end_flag==0x00)
				{
					 chanenel_4_measure_end_flag=0x01;
					for(i_clear=6;i_clear<chnanel_4_num_end;i_clear++)
					{
						if(chanel_history_4[i_clear]>black_value_max_4)
						{
											
							black_value_max_4=chanel_history_4[i_clear];
						}
						
						if(chanel_history_4[i_clear]<black_value_min_4)
						{
					
							black_value_min_4=chanel_history_4[i_clear];
						}
						
								
					}	
					speaker_context_4_end();
					
				}
				else
				{
					chanenel_4_measure_end_flag=0x01;
				}
			  
			
				
		}
		


	}
	
}



channel_5_judge_begin_end_save_data()
{
	if(black_value_5>0.428)
	{
		chnanel_5_num++;
		if(chnanel_5_num>600)
			chnanel_5_num=0;
		
		if(chnanel_5_num==0x01)
		{
			black_value_5_last=black_value_5;
			black_value_max_5=0;
			black_value_min_5=100;
			
			for(i_clear=0;i_clear<=599;i_clear++)
			{
				chanel_history_5[i_clear]=0;
			}
			
			speaker_context_5_start();
		}

		
	}
	
	if(chnanel_5_num>5)
	{
			
		if(black_value_5>0.528)
		{
			chanel_history_5[chnanel_5_num]=black_value_5;
//									chnanel_1_num++;
		}
		else
		{
				chnanel_5_num_end=chnanel_5_num-5;
				if(chanenel_5_measure_end_flag==0x00)
				{
					 chanenel_5_measure_end_flag=0x01;
					for(i_clear=6;i_clear<chnanel_5_num_end;i_clear++)
					{
						if(chanel_history_5[i_clear]>black_value_max_5)
						{
											
							black_value_max_5=chanel_history_5[i_clear];
						}
						
						if(chanel_history_5[i_clear]<black_value_min_5)
						{
					
							black_value_min_5=chanel_history_5[i_clear];
						}	
						
								
					}	
					speaker_context_5_end();
					
					
					
				}
				else
				{
					chanenel_5_measure_end_flag=0x01;
				}
			  
			
				
		}
		


	}
	
}

///////////////////////////////////////////
//////////////
//channel_1_adjust_1_or_100
/////////////////
void channel_1_adjust_1_or_100(void)
{
	if(black_value_1>3.2)
	{
		//judge wheather ture off light *100
		
		s1_1=1;
		s0_1=1;
		if (multify_100_flag_1==0x01)
    {
			multify_100_flag_1=0x01;
    }
    else
    {
			multify_100_flag_1=0x01;
			channel_1_aperture_change=0x01;
    }
		
				
	}
	
	if(black_value_1<2.8)
	{
		//judge wheather ture off light *100
		
		s1_1=0;
		s0_1=1;
		
		if (multify_100_flag_1==0x00)
    {
			multify_100_flag_1=0x00;
    }
    else
    {
			multify_100_flag_1=0x00;
			channel_1_aperture_change=0x01;
    }
				
	}
	
	
}

void channel_2_adjust_1_or_100(void)
{
	if(black_value_2>3.2)
	{
		//judge wheather ture off light *100
		
		s1_2=1;
		s0_2=1;
		if (multify_100_flag_2==0x01)
    {
			multify_100_flag_2=0x01;
    }
    else
    {
			multify_100_flag_2=0x01;
			channel_2_aperture_change=0x01;
    }
		
				
	}
	
	if(black_value_2<2.8)
	{
		//judge wheather ture off light *100
		
		s1_2=0;
		s0_2=1;
		
		if (multify_100_flag_2==0x00)
    {
			multify_100_flag_2=0x00;
    }
    else
    {
			multify_100_flag_2=0x00;
			channel_2_aperture_change=0x01;
    }
				
	}
	
	
}

void channel_3_adjust_1_or_100(void)
{
	if(black_value_3>3.2)
	{
		//judge wheather ture off light *100
		
		s1_3=1;
		s0_3=1;
		if (multify_100_flag_3==0x01)
    {
			multify_100_flag_3=0x01;
    }
    else
    {
			multify_100_flag_3=0x01;
			channel_3_aperture_change=0x01;
    }
		
				
	}
	
	if(black_value_3<2.8)
	{
		//judge wheather ture off light *100
		
		s1_3=0;
		s0_3=1;
		
		if (multify_100_flag_3==0x00)
    {
			multify_100_flag_3=0x00;
    }
    else
    {
			multify_100_flag_3=0x00;
			channel_3_aperture_change=0x01;
    }
				
	}
	
	
}



void channel_4_adjust_1_or_100(void)
{
	if(black_value_4>3.2)
	{
		//judge wheather ture off light *100
		
		s1_4=1;
		s0_4=1;
		if (multify_100_flag_4==0x01)
    {
			multify_100_flag_4=0x01;
    }
    else
    {
			multify_100_flag_4=0x01;
			channel_4_aperture_change=0x01;
    }
		
				
	}
	
	if(black_value_4<2.8)
	{
		//judge wheather ture off light *100
		
		s1_4=0;
		s0_4=1;
		
		if (multify_100_flag_4==0x00)
    {
			multify_100_flag_4=0x00;
    }
    else
    {
			multify_100_flag_4=0x00;
			channel_4_aperture_change=0x01;
    }
				
	}
	
	
}



void channel_5_adjust_1_or_100(void)
{
	if(black_value_5>3.2)
	{
		//judge wheather ture off light *100
		
		s1_5=1;
		s0_5=1;
		if (multify_100_flag_5==0x01)
    {
			multify_100_flag_5=0x01;
    }
    else
    {
			multify_100_flag_5=0x01;
			channel_5_aperture_change=0x01;
    }
		
				
	}
	
	if(black_value_5<2.8)
	{
		//judge wheather ture off light *100
		
		s1_5=0;
		s0_5=1;
		
		if (multify_100_flag_5==0x00)
    {
			multify_100_flag_5=0x00;
    }
    else
    {
			multify_100_flag_5=0x00;
			channel_5_aperture_change=0x01;
    }
				
	}
	
	
}











////////////////////////////////////
//channel  1
///////////////////////////
void channel_1_calute_black_value_begin_end_save_data_adjust_aperture(void)
{
	//计算1#黑度值,修正1#黑度值，显示1#黑度值
		//zero should avoid in the denominator//分母不能为0
		if(black_frequency_1==0x00)
		{
			//give up this time caculate black and then chang the light to 100
			s1_1=1;
			s0_1=1;
			multify_100_flag_1=0x01;
			
			black_value_original_1=15;
			black_value_1=15;
							
		}
		else if(black_frequency0_1+100<black_frequency_1)
		{
			s1_1=0;
			s0_1=1;
			multify_100_flag_1=0x00;
			black_value_original_1=0x00;
			black_value_1=0x00;
			channel_1_judge_begin_end_save_data();
			
		}
		else if(channel_1_aperture_change==0x01)
		{
			channel_1_aperture_change=0x00;
			
		}
		else
		{
				black_value_original_1= log10(black_frequency0_1/black_frequency_1);
			
				inf_x=isfinite(black_value_original_1);
				if(inf_x==0)
				{
					black_value_original_1=-10;
				}
			
				//judge if the light is *100 .yes,black_value_original+2
				if(multify_100_flag_1==0x01)
				{
					black_value_original_1=black_value_original_1+2;
				}
				else
				{
					black_value_original_1=black_value_original_1+0;
				}
				
				
				//judge wheather ture off light *100
	//						if(black_value_original_1<=4.5)
	//							{
	//								s1_1=0;
	//								s0_1=1;
	//								multify_100_flag_1=0x00;
	//							
	//							}
					
					///judge wheather need adjust error
			
				if(black_value_original_1<0.25)
					{
						black_value_1=black_value_original_1;
					}	
					
				if((black_value_original_1>=0.25&&black_value_original_1<=3.0)||black_value_original_1==0)
					{
						black_value_1=p1_1_1*pow(black_value_original_1,3) + p2_1_1*pow(black_value_original_1,2) + p3_1_1*pow(black_value_original_1,1) + p4_1_1+0.355;
								
					}
				if(black_value_original_1>3.0)
					{
						
							black_value_1=p1_2_1*pow(black_value_original_1,3) + p2_2_1*pow(black_value_original_1,2) + p3_2_1*pow(black_value_original_1,1) + p4_2_1+0.355;		
					}
					
					///judge biggest and  smallest
					
					channel_1_judge_begin_end_save_data();
					
					channel_1_adjust_1_or_100();
					
				
				
		}
}
////////////////////////////////////
//channel  2
///////////////////////////

void channel_2_calute_black_value_begin_end_save_data_adjust_aperture(void)
{
					//计算2#黑度值,修正2#黑度值，显示2#黑度值
				//zero should avoid in the denominator//分母不能为0
	if(black_frequency_2==0x00)
	{
		//give up this time caculate black and then chang the light to 100
		s1_2=1;
		s0_2=1;
		multify_100_flag_2=0x01;
		
		black_value_original_2=0;
		black_value_2=0;
						
	}
	else if(black_frequency0_2+100<black_frequency_2)
	{
		s1_2=0;
		s0_2=1;
		multify_100_flag_2=0x00;
		black_value_original_2=0x00;
		black_value_2=0x00;
		channel_2_judge_begin_end_save_data();
		
	}
	else if(channel_2_aperture_change==0x01)
	{
		channel_2_aperture_change=0x00;
		
	}
	else
	{
			black_value_original_2= log10(black_frequency0_2/black_frequency_2);
		
			inf_x=isfinite(black_value_original_2);
			if(inf_x==0)
			{
				black_value_original_2=-10;
			}
		
			//judge if the light is *100 .yes,black_value_original+2
			if(multify_100_flag_2==0x01)
			{
				black_value_original_2=black_value_original_2+2;
			}
			else
			{
				black_value_original_2=black_value_original_2+0;
			}
			
			
			//judge wheather ture off light *100
//						if(black_value_original_1<=4.5)
//							{
//								s1_1=0;
//								s0_1=1;
//								multify_100_flag_1=0x00;
//							
//							}
				
				///judge wheather need adjust error
		
				
				
			if(black_value_original_2<0.25)
				{
					black_value_2=black_value_original_2;
				}	
				
			if((black_value_original_2>=0.25&&black_value_original_2<=3.0)||black_value_original_2==0)
				{
					black_value_2=p1_1_2*pow(black_value_original_2,3) + p2_1_2*pow(black_value_original_2,2) + p3_1_2*pow(black_value_original_2,1) + p4_1_2+0.255;
							
				}
			if(black_value_original_2>3.0)
				{
					
						black_value_2=p1_2_2*pow(black_value_original_2,3) + p2_2_2*pow(black_value_original_2,2) + p3_2_2*pow(black_value_original_2,1) + p4_2_2+0.255;		
				}
				///judge biggest and  smallest
				
				channel_2_judge_begin_end_save_data();
				
				channel_2_adjust_1_or_100();
			
	}
	
}
////////////////////////////////////
//channel  3
///////////////////////////

void channel_3_calute_black_value_begin_end_save_data_adjust_aperture(void)
{
	
			//计算3#黑度值,修正3#黑度值，显示3#黑度值
	//zero should avoid in the denominator//分母不能为0
	if(black_frequency_3==0x00)
	{
		//give up this time caculate black and then chang the light to 100
		s1_3=1;
		s0_3=1;
		multify_100_flag_3=0x01;
		
		black_value_original_3=0;
		black_value_3=0;
						
	}
	else if(black_frequency0_3+100<black_frequency_3)
	{
		s1_3=0;
		s0_3=1;
		multify_100_flag_3=0x00;
		black_value_original_3=0x00;
		black_value_3=0x00;
		channel_3_judge_begin_end_save_data();
		
	}
	else if(channel_3_aperture_change==0x01)
	{
		channel_3_aperture_change=0x00;
		
	}
	else
	{
			black_value_original_3= log10(black_frequency0_3/black_frequency_3);
		
			inf_x=isfinite(black_value_original_3);
			if(inf_x==0)
			{
				black_value_original_3=-10;
			}
		
			//judge if the light is *100 .yes,black_value_original+2
			if(multify_100_flag_3==0x01)
			{
				black_value_original_3=black_value_original_3+2;
			}
			else
			{
				black_value_original_3=black_value_original_3+0;
			}
			
			
			//judge wheather ture off light *100
//						if(black_value_original_1<=4.5)
//							{
//								s1_1=0;
//								s0_1=1;
//								multify_100_flag_1=0x00;
//							
//							}
				
				///judge wheather need adjust error
		
				
				
			if(black_value_original_3<0.25)
				{
					black_value_3=black_value_original_3;
				}	
				
			if((black_value_original_3>=0.25&&black_value_original_3<=3.0)||black_value_original_3==0)
				{
					black_value_3=p1_1_3*pow(black_value_original_3,3) + p2_1_3*pow(black_value_original_3,2) + p3_1_3*pow(black_value_original_3,1) + p4_1_3+0.272;
							
				}
			if(black_value_original_3>3.0)
				{
					
						black_value_3=p1_2_3*pow(black_value_original_3,3) + p2_2_3*pow(black_value_original_3,2) + p3_2_3*pow(black_value_original_3,1) + p4_2_3+0.272;		
				}
				
				///judge biggest and  smallest
				
				channel_3_judge_begin_end_save_data();
				
				channel_3_adjust_1_or_100();
				
			
			
	}
				
	
	
}







				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				

















////////////////////////////

////////////////////////////////////
//channel  4
///////////////////////////

void channel_4_calute_black_value_begin_end_save_data_adjust_aperture(void)
{
	//计算4#黑度值,修正4#黑度值，显示4#黑度值
	//zero should avoid in the denominator//分母不能为0
	if(black_frequency_4==0x00)
	{
		//give up this time caculate black and then chang the light to 100
		s1_4=1;
		s0_4=1;
		multify_100_flag_4=0x01;
		
		black_value_original_4=0;
		black_value_4=0;
						
	}
	else if(black_frequency0_4+100<black_frequency_4)
	{
		s1_4=0;
		s0_4=1;
		multify_100_flag_4=0x00;
		black_value_original_4=0x00;
		black_value_4=0x00;
		channel_4_judge_begin_end_save_data();
		
	}
	else if(channel_4_aperture_change==0x01)
	{
		channel_4_aperture_change=0x00;
		
	}
	else
	{
			black_value_original_4= log10(black_frequency0_4/black_frequency_4);
		
			inf_x=isfinite(black_value_original_4);
			if(inf_x==0)
			{
				black_value_original_4=-10;
			}
		
			//judge if the light is *100 .yes,black_value_original+2
			if(multify_100_flag_4==0x01)
			{
				black_value_original_4=black_value_original_4+2;
			}
			else
			{
				black_value_original_4=black_value_original_4+0;
			}
			
			
			//judge wheather ture off light *100
//						if(black_value_original_1<=4.5)
//							{
//								s1_1=0;
//								s0_1=1;
//								multify_100_flag_1=0x00;
//							
//							}
				
				///judge wheather need adjust error
		
				
				
			if(black_value_original_4<0.25)
				{
					black_value_4=black_value_original_4;
				}	
				
			if((black_value_original_4>=0.25&&black_value_original_4<=3.0)||black_value_original_4==0)
				{
					black_value_4=p1_1_4*pow(black_value_original_4,3) + p2_1_4*pow(black_value_original_4,2) + p3_1_4*pow(black_value_original_4,1) + p4_1_4+0.203;
							
				}
			if(black_value_original_4>3.0)
				{
					
						black_value_4=p1_2_4*pow(black_value_original_4,3) + p2_2_4*pow(black_value_original_4,2) + p3_2_4*pow(black_value_original_4,1) + p4_2_4+0.203;		
				}
				
				///judge biggest and  smallest
				
				channel_4_judge_begin_end_save_data();
				
				channel_4_adjust_1_or_100();
				
			
			
	}
	
}

////////////////////////////

////////////////////////////////////
//channel  5
///////////////////////////

void channel_5_calute_black_value_begin_end_save_data_adjust_aperture(void)
{
			//计算5#黑度值,修正5#黑度值，显示5#黑度值
				//zero should avoid in the denominator//分母不能为0
	if(black_frequency_5==0x00)
	{
		//give up this time caculate black and then chang the light to 100
		s1_5=1;
		s0_5=1;
		multify_100_flag_5=0x01;
		
		black_value_original_5=0;
		black_value_5=0;
						
	}
	else if(black_frequency0_5+100<black_frequency_5)
	{
		s1_5=0;
		s0_5=1;
		multify_100_flag_5=0x00;
		black_value_original_5=0x00;
		black_value_5=0x00;
		channel_5_judge_begin_end_save_data();
		
	}
	else if(channel_5_aperture_change==0x01)
	{
		channel_5_aperture_change=0x00;
		
	}
	else
	{
			black_value_original_5= log10(black_frequency0_5/black_frequency_5);
		
			inf_x=isfinite(black_value_original_5);
			if(inf_x==0)
			{
				black_value_original_5=-10;
			}
		
			//judge if the light is *100 .yes,black_value_original+2
			if(multify_100_flag_5==0x01)
			{
				black_value_original_5=black_value_original_5+2;
			}
			else
			{
				black_value_original_5=black_value_original_5+0;
			}
			
			
			//judge wheather ture off light *100
//						if(black_value_original_1<=4.5)
//							{
//								s1_1=0;
//								s0_1=1;
//								multify_100_flag_1=0x00;
//							
//							}
				
				///judge wheather need adjust error
		
				
				
			if(black_value_original_5<0.25)
				{
					black_value_5=black_value_original_5;
				}	
				
			if((black_value_original_5>=0.25&&black_value_original_5<=3.0)||black_value_original_5==0)
				{
					black_value_5=p1_1_5*pow(black_value_original_5,3) + p2_1_5*pow(black_value_original_5,2) + p3_1_5*pow(black_value_original_5,1) + p4_1_5+0.235;
							
				}
			if(black_value_original_5>3.0)
				{
					
						black_value_5=p1_2_5*pow(black_value_original_5,3) + p2_2_5*pow(black_value_original_5,2) + p3_2_5*pow(black_value_original_5,1) + p4_2_5+0.235;		
				}
				
				///judge biggest and  smallest
				
				channel_5_judge_begin_end_save_data();
				
				channel_5_adjust_1_or_100();
				
			
			
	}
	
			
				
	
	
}

////////////////////////////





////////////////////////////
//begin measure
////////////////////////
void begin_measure(void)
{
	
		
//		LED0=!LED0;		
		if (display_flag==0x01)
		{
			
			if(theta0_flag==0x00)
			{
					black_frequency0_1=(float)black_frequency_1;
					black_frequency0_2=(float)black_frequency_2;
					black_frequency0_3=(float)black_frequency_3;
					black_frequency0_4=(float)black_frequency_4;
					black_frequency0_5=(float)black_frequency_5;

				
				if((black_frequency0_1>100)&&(black_frequency0_2>100)&&(black_frequency0_3>100)&&(black_frequency0_4>100)&&(black_frequency0_5>100))
				{
					theta0_flag=0x01;
					speaker_context_get_thita0();
				}
				
				
				if(black_frequency0_1<100)
				{
					theta0_flag=0x00;
				}
				
				
					
				
			}
	
			channel_1_calute_black_value_begin_end_save_data_adjust_aperture();
			
			channel_2_calute_black_value_begin_end_save_data_adjust_aperture();
			
			channel_3_calute_black_value_begin_end_save_data_adjust_aperture();
			
			channel_4_calute_black_value_begin_end_save_data_adjust_aperture();
			
			channel_5_calute_black_value_begin_end_save_data_adjust_aperture();
			

			 
					
			DataScope_Get_Channel_Data((float)black_value_original_1, 1 );
			DataScope_Get_Channel_Data((float)black_value_original_2, 2 );
			DataScope_Get_Channel_Data((float)black_value_original_3, 3 );
			DataScope_Get_Channel_Data((float)black_value_original_4, 4 );
			DataScope_Get_Channel_Data((float)black_value_original_5, 5 );

	
			Send_Count = DataScope_Data_Generate(5);
			for( s_i = 0 ; s_i < Send_Count; s_i++) 
			{
				while((USART3->SR&0X40)==0);  
				USART3->DR = DataScope_OutPut_Buffer[s_i]; 
			}
				
			display_flag=0x00;
	}
	
}
////////////////////////////

////////////////////////////
//begin clear_all_variable_before_begin
////////////////////////
void clear_all_variable_before_begin(void)
{
		chnanel_1_num=0x00;
		chnanel_2_num=0x00;
		chnanel_3_num=0x00;
		chnanel_4_num=0x00;
		chnanel_5_num=0x00;


		
	
		theta0_flag=0x00;

//						jump_down_flag_1=0x00;
//						jump_down_flag_2=0x00;
//						jump_down_flag_3=0x00;
//						jump_down_flag_4=0x00;
//						jump_down_flag_5=0x00;
		
		chanenel_1_measure_end_flag=0x00;
		chanenel_2_measure_end_flag=0x00;
		chanenel_3_measure_end_flag=0x00;
		chanenel_4_measure_end_flag=0x00;
		chanenel_5_measure_end_flag=0x00;

		black_frequency0_1=0;
		black_frequency0_2=0;
		black_frequency0_3=0;
		black_frequency0_4=0;
		black_frequency0_5=0;

		
		

		measure_end_flag=0;
	
		
		measure_start_flag=0x00;
		touch_left_flag=0x00;
		send_measure_end_flag=0x00;
		send_photto_begin_flag=0x00;
		
//		USART_RX_success_flag=0x00;
		
		clc_variable_flag=0x00;
		
		count_state_5=0x00;
		
		
		chanenel_1_measure_end_flag=0x00;
		chanenel_2_measure_end_flag=0x00;
		chanenel_3_measure_end_flag=0x00;
		chanenel_4_measure_end_flag=0x00;
		chanenel_5_measure_end_flag=0x00;

	
}

////////////////////////////
//begin send_measure_is_over
////////////////////////

void send_black_measure_is_over(void)
{
	if (send_measure_end_flag==0x00)
	{
		
			send_measure_end_flag=0x01;
			
			black_value_max=black_value_max_1;

			if(black_value_max<black_value_max_2)
			{
				black_value_max=black_value_max_2;
			}
			
			if(black_value_max<black_value_max_3)
			{
				black_value_max=black_value_max_3;
			}
			
			if(black_value_max<black_value_max_4)
			{
				black_value_max=black_value_max_4;
			}
			
			if(black_value_max<black_value_max_5)
			{
				black_value_max=black_value_max_5;
			}
			
			
			
			
			
			
			black_value_min=black_value_min_1;
			
			if(black_value_min>black_value_min_2)
			{
				black_value_min=black_value_min_2;
			}
			
			if(black_value_min>black_value_min_3)
			{
				black_value_min=black_value_min_3;
			}
			
			if(black_value_min>black_value_min_4)
			{
				black_value_min=black_value_min_4;
			}
			
			if(black_value_min>black_value_min_5)
			{
				black_value_min=black_value_min_5;
			}
			
		
			
			
			
			DataScope_Get_Channel_Data2((float)black_value_max, 1 );
			DataScope_Get_Channel_Data2((float)black_value_min, 2 );
		
			DataScope_OutPut_Buffer2[0] = 0xa5;  //帧头
			DataScope_OutPut_Buffer2[1] = 0x5a;  //帧头
			DataScope_OutPut_Buffer2[2] = 0x0c;  //数据长度

			DataScope_OutPut_Buffer2[11] = 0x0d;  //帧尾
			DataScope_OutPut_Buffer2[12] = 0x0a;  //帧尾
		
	
			Send_Count = DataScope_Data_Generate2(2);
			for( s_i = 0 ; s_i < Send_Count; s_i++) 
			{
				while((USART1->SR&0X40)==0);  
				USART1->DR = DataScope_OutPut_Buffer2[s_i]; 
			}
			
			speaker_context_black_end();
			
			
			
			
			
	}
}



////////////////////////////
//begin test_usart1_3_command
////////////////////////
//2018.07.10 transplant
void test_usart1_3_command(void)
{	
	black_value_max=0;
	black_value_min=10;
	
	DataScope_Get_Channel_Data2((float)black_value_max, 1 );
	DataScope_Get_Channel_Data2((float)black_value_min, 2 );

	DataScope_OutPut_Buffer2[0] = 0xa5;  //帧头
	DataScope_OutPut_Buffer2[1] = 0x5a;  //帧头
	DataScope_OutPut_Buffer2[2] = 0x0d;  //数据长度

	DataScope_OutPut_Buffer2[11] = 0x0d;  //帧尾
	DataScope_OutPut_Buffer2[12] = 0x0a;  //帧尾


	Send_Count = DataScope_Data_Generate2(2);
	for( s_i = 0 ; s_i < Send_Count; s_i++) 
	{
//		while((USART3->SR&0X40)==0);  
//		USART3->DR = DataScope_OutPut_Buffer2[s_i]; 
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer2[s_i]; 
	}
	
	delay_ms(10);
	DataScope_OutPut_Buffer2[0] = 0xa5;  //帧头
	DataScope_OutPut_Buffer2[1] = 0x5a;  //帧头
	DataScope_OutPut_Buffer2[2] = 0x06;  //数据长度
	DataScope_OutPut_Buffer2[3] = 0x02;  //数据长度


	DataScope_OutPut_Buffer2[4] = 0x0d;  //帧尾
	DataScope_OutPut_Buffer2[5] = 0x0a;  //帧尾


	Send_Count =0x06;// DataScope_Data_Generate2(2);
	for( s_i = 0 ; s_i < Send_Count; s_i++) 
	{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer2[s_i]; 
	}
	
	
	delay_ms(10);
	DataScope_OutPut_Buffer2[0] = 0xa5;  //帧头
	DataScope_OutPut_Buffer2[1] = 0x5a;  //帧头
	DataScope_OutPut_Buffer2[2] = 0x06;  //数据长度
	DataScope_OutPut_Buffer2[3] = 0x01;  //数据长度


	DataScope_OutPut_Buffer2[4] = 0x0d;  //帧尾
	DataScope_OutPut_Buffer2[5] = 0x0a;  //帧尾


	Send_Count =0x06;// DataScope_Data_Generate2(2);
	for( s_i = 0 ; s_i < Send_Count; s_i++) 
	{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer2[s_i]; 
	}

}


show_basic_information()
{
	
	black_value_max=black_value_max_1;
	
	if(black_value_max<black_value_max_2)
	{
		black_value_max=black_value_max_2;
	}
	
	if(black_value_max<black_value_max_3)
	{
		black_value_max=black_value_max_3;
	}
	
	if(black_value_max<black_value_max_4)
	{
		black_value_max=black_value_max_4;
	}
	
	if(black_value_max<black_value_max_5)
	{
		black_value_max=black_value_max_5;
	}
	
	
	
	
	black_value_min=black_value_min_1;
	
	if(black_value_min>black_value_min_2)
	{
		black_value_min=black_value_min_2;
	}
	
	if(black_value_min>black_value_min_3)
	{
		black_value_min=black_value_min_3;
	}
	
	if(black_value_min>black_value_min_4)
	{
		black_value_min=black_value_min_4;
	}
	
	if(black_value_min>black_value_min_5)
	{
		black_value_min=black_value_min_5;
	}
	
	
	


}





//////////////////////
///begin show_mode_6_test_process
////////////////////	
void speak_context(u8 buf_xm[],int speak_len )
{
	//speak_len=(sizeof(buf_xm)/sizeof(buf_xm[0]));
	Send_Count = speak_len+5;

	DataScope_OutPut_Buffer[0]=0xfd;
	DataScope_OutPut_Buffer[1]=0x00;
	DataScope_OutPut_Buffer[2]=speak_len+2;
	
	DataScope_OutPut_Buffer[3]=0x01;
	DataScope_OutPut_Buffer[4]=0x00;//01 gbk //00 gb2312
	
	
	//说话内容
		
	for(speak_i=0;speak_i<speak_len;speak_i++)
	{
		DataScope_OutPut_Buffer[speak_i+5]=buf_xm[speak_i];
	}
	
	

	for( speak_i = 0 ; speak_i < Send_Count; speak_i++) 
	{
		while((USART2->SR&0X40)==0);  
		USART2->DR = DataScope_OutPut_Buffer[speak_i]; 
	}
}
///begin show_mode_6_test_process
////////////////////	
/////////////
////
////////////////

void speaker_context_clc_ram_begin(void)
{
	
	
//	u8 buf1[]={"[s9]变量清零，黑度测量开始"};//\[b1] 欢迎光临，请进
//	
//	
//	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
//	
//	speak_context(buf1,speak_len1);
	char buf1[20]={0};//\[b1] 欢迎光临，请进
//	
//	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
//	
//	speak_context(buf1,speak_len1);
	sprintf(buf1,"[s9][x1]sound203 ");//\[b1] 欢迎光临，请进
	speak_len1=strlen(( const char *)buf1);
	speak_context((u8*)buf1,speak_len1);
	delay_ms(10);
}

///begin show_mode_6_test_process
////////////////////	
/////////////
////
////////////////

void speaker_context_black_end(void)
{
	u8 buf1[]={"[s9]黑度测量结束,上传最值"};//\[b1] 欢迎光临，请进

	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
	
}

////////////////////	
/////////////
////
////////////////

void speaker_context_photo_start(void)
{
	
	
	u8 buf1[]={"照相机开启"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
}
























////////////////////	
/////////////
////
////////////////

void speaker_context_1_start(void)
{
	
	
	u8 buf1[]={"一号开始"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
}

/////////////
////
////////////////

void speaker_context_2_start(void)
{
	
	
	u8 buf1[]={"二号开始"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
}
/////////////
////
////////////////

void speaker_context_3_start(void)
{
	
	
	u8 buf1[]={"三号开始"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
}
/////////////
////
////////////////

void speaker_context_4_start(void)
{
	
	
	u8 buf1[]={"四号开始"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
}
/////////////
////
////////////////

void speaker_context_5_start(void)
{
	
	
	u8 buf1[]={"五号开始"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
}
/////////////
////
////////////////

void speaker_context_1_end(void)
{
	
	
	u8 buf1[]={"一号结束"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
}
/////////////
////
////////////////

void speaker_context_2_end(void)
{
	
	
	u8 buf1[]={"二号结束"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
}

/////////////
////
////////////////

void speaker_context_3_end(void)
{
	
	
	u8 buf1[]={"三号结束"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
}
/////////////
////
////////////////

void speaker_context_4_end(void)
{
	
	
	u8 buf1[]={"四号结束"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
}
/////////////
////
////////////////

void speaker_context_5_end(void)
{
	
	
	u8 buf1[]={"五号结束"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
}

/////////////
////
////////////////

void speaker_1_fail_no_start(void)
{
	
	
	u8 buf1[]={"一号没开始测量"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
	delay_ms(1860);
	delay_ms(1000);
}
/////////////
////
////////////////

void speaker_1_fail_no_end(void)
{
	
	
	u8 buf1[]={"一号没结束测量"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
	delay_ms(1860);
	delay_ms(1000);
}


/////////////
////
////////////////

void speaker_2_fail_no_start(void)
{
	
	
	u8 buf1[]={"二号没开始测量"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
	delay_ms(1860);
	delay_ms(1000);
}
/////////////
////
////////////////

void speaker_2_fail_no_end(void)
{
	
	
	u8 buf1[]={"二号没结束测量"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
	delay_ms(1860);
	delay_ms(1000);
}

////////////
////
////////////////

void speaker_3_fail_no_start(void)
{
	
	
	u8 buf1[]={"三号没开始测量"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
	delay_ms(1860);
	delay_ms(1000);
}
/////////////
////
////////////////

void speaker_3_fail_no_end(void)
{
	
	
	u8 buf1[]={"三号没结束测量"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
	delay_ms(1860);
	delay_ms(1000);
}

////////////
////
////////////////

void speaker_4_fail_no_start(void)
{
	
	
	u8 buf1[]={"四号没开始测量"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
	delay_ms(1860);
	delay_ms(1000);
}
/////////////
////
////////////////

void speaker_4_fail_no_end(void)
{
	
	
	u8 buf1[]={"四号没结束测量"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
	delay_ms(1860);
	delay_ms(1000);
}


////////////
////
////////////////

void speaker_5_fail_no_start(void)
{
	
	
	u8 buf1[]={"五号没开始测量"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
	delay_ms(1860);
	delay_ms(1000);
}
/////////////
////
////////////////

void speaker_5_fail_no_end(void)
{
	
	
	u8 buf1[]={"五号没结束测量"};//\[b1] 欢迎光临，请进
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
	delay_ms(1860);
	delay_ms(1000);
}
/////////////
////
////////////////

void speaker_context_get_thita0(void)
{
	
	
	char buf1[20]={0};//\[b1] 欢迎光临，请进
//	
//	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
//	
//	speak_context(buf1,speak_len1);
	sprintf(buf1,"[s9][x1]sound103 ");//\[b1] 欢迎光临，请进
	speak_len1=strlen(( const char *)buf1);
	speak_context((u8*)buf1,speak_len1);
}
























////////////////////	
/////////////
////
////////////////

void speaker_context_photo_end_ok(void)
{
	
	
	char buf1[150]={"照相机关闭，测量完成，变量清零，请放片"};//\[b1] 欢迎光临，请进
//	
//	
//	speak_len1=strlen(( const char *)buf1);
//	//speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
//	
//	speak_context((u8*)buf1,speak_len1);
	
	//speak max and min
	
	//speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	sprintf(buf1,"[s5]照相机关闭，测量完成,最大值为%4.2f,最小值为%4.2f,请放[p20]下一张片子",black_value_max,black_value_min);
	speak_len1=strlen(( const char *)buf1);
	speak_context((u8*)buf1,speak_len1);
}
////////////////////	
/////////////
////
////////////////

void speaker_context_photo_end_fail(void)
{
	
	
	u8 buf1[]={"照相机关闭，测量失败，变量清零，请重测,失败原因是"};//\[b1] 欢迎光临，请进
	
	
	speak_len1=(sizeof(buf1)/sizeof(buf1[0]));
	
	speak_context(buf1,speak_len1);
	delay_ms(1860);
	delay_ms(1860);
//	delay_ms(1860);
//	delay_ms(1860);
	delay_ms(1060);
	
}

/////////////////
////
///////////////
void speaker_test(void)
{
//	char buf1[150]={"照相机关闭，测量完成，变量清零，请放片"};//\[b1] 欢迎光临，请进
//	
//	sprintf(buf1,"[s5]照相机关闭，测量完成,最大值为%4.2f,最小值为%4.2f,请放[p90]下一张片子",black_value_max,black_value_min);
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
	
	
	
	u8 buf1[]={"[s9]欢迎使用数字底片测量系统,2.2版本,自检完成，请放片[x1]sound223 "};//\[b1] 欢迎光临，请进
	speak_len1=strlen(( const char *)buf1);
	speak_context((u8*)buf1,speak_len1);
//	char buf1[100]={"[s9][x1]sound101"};//\[b1] 欢迎光临，请进

//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	
//	sprintf(buf1,"[s9][x1]sound102 ");//\[b1] 欢迎光临，请进
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	
//	sprintf(buf1,"[s9][x1]sound103 ");//\[b1] 欢迎光临，请进
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	
//	sprintf(buf1,"[s9][x1]sound104 ");//\[b1] 欢迎光临，请进
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	
//	sprintf(buf1,"[s9][x1]sound105 ");//\[b1] 欢迎光临，请进
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	
//	sprintf(buf1,"[s9][x1]sound106 ");//\[b1] 欢迎光临，请进
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	
//	sprintf(buf1,"[s9][x1]sound107 ");//\[b1] 欢迎光临，请进
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	
//	
//	sprintf(buf1,"[s9][x1]sound108 ");//\[b1] 欢迎光临，请进
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	
//	sprintf(buf1,"[s9][x1]sound109 ");//\[b1] 欢迎光临，请进
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	
//	sprintf(buf1,"[s9][x1]sound110");//\[b1] 欢迎光临，请进
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	
//	sprintf(buf1,"[s9][x1]sound111 ");//\[b1] 欢迎光临，请进
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	
//	sprintf(buf1,"[s9][x1]sound112 ");//\[b1] 欢迎光临，请进
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	
//	sprintf(buf1,"[s9][x1]sound113 ");//\[b1] 欢迎光临，请进
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	delay_ms(1500);
//	speak_len=(sizeof(buf1)/sizeof(buf1[0]));
//	Send_Count = speak_len+5;
//	//%BB%F9%D3%DA%B5%E7%C8%DD%B4%AB%B8%D0%C6%F7%B5%C4%CA%D6%CA%C6%CA%B6%B1%F0%CF%B5%CD%B3
//	DataScope_OutPut_Buffer[0]=0xfd;
//	DataScope_OutPut_Buffer[1]=0x00;
//	DataScope_OutPut_Buffer[2]=speak_len+2;
//	
//	DataScope_OutPut_Buffer[3]=0x01;
//	DataScope_OutPut_Buffer[4]=0x00;//01 gbk //00 gb2312
//	
//	
//	//说话内容
//		
//	for(speak_i=0;speak_i<speak_len;speak_i++)
//	{
//		DataScope_OutPut_Buffer[speak_i+5]=buf1[speak_i];
//	}
//	
//	

//	for( speak_i = 0 ; speak_i < Send_Count; speak_i++) 
//	{
//		while((USART2->SR&0X40)==0);  
//		USART2->DR = DataScope_OutPut_Buffer[speak_i]; 
//	}
	
  for( speak_i = 0 ; speak_i < 1; speak_i++) 
	{
		delay_ms(1864);
	delay_ms(1864);
	//delay_ms(1864);
//	delay_ms(1864);
	}
	//
//	///////////////
//	//第二句
//	//////////
//		speak_len=(sizeof(buf2)/sizeof(buf2[0]));
//	Send_Count = speak_len+5;
//	//%BB%F9%D3%DA%B5%E7%C8%DD%B4%AB%B8%D0%C6%F7%B5%C4%CA%D6%CA%C6%CA%B6%B1%F0%CF%B5%CD%B3
//	DataScope_OutPut_Buffer[0]=0xfd;
//	DataScope_OutPut_Buffer[1]=0x00;
//	DataScope_OutPut_Buffer[2]=speak_len+2;
//	
//	DataScope_OutPut_Buffer[3]=0x01;
//	DataScope_OutPut_Buffer[4]=0x00;//01 gbk //00 gb2312
//	
//	
//	//说话内容
//		
//	for(speak_i=0;speak_i<speak_len;speak_i++)
//	{
//		DataScope_OutPut_Buffer[speak_i+5]=buf2[speak_i];
//	}
//	
//	

//	for( speak_i = 0 ; speak_i < Send_Count; speak_i++) 
//	{
//		while((USART2->SR&0X40)==0);  
//		USART2->DR = DataScope_OutPut_Buffer[speak_i]; 
//	}
//	
//  for( speak_i = 0 ; speak_i < 1; speak_i++) 
//	{
//		delay_ms(1864);
//	delay_ms(1864);
//	delay_ms(1864);
//	delay_ms(1864);
//	}


//	///////////////
//	//第3句
//	//////////
//		speak_len=(sizeof(buf3)/sizeof(buf3[0]));
//	Send_Count = speak_len+5;
//	//%BB%F9%D3%DA%B5%E7%C8%DD%B4%AB%B8%D0%C6%F7%B5%C4%CA%D6%CA%C6%CA%B6%B1%F0%CF%B5%CD%B3
//	DataScope_OutPut_Buffer[0]=0xfd;
//	DataScope_OutPut_Buffer[1]=0x00;
//	DataScope_OutPut_Buffer[2]=speak_len+2;
//	
//	DataScope_OutPut_Buffer[3]=0x01;
//	DataScope_OutPut_Buffer[4]=0x00;//01 gbk //00 gb2312
//	
//	
//	//说话内容
//		
//	for(speak_i=0;speak_i<speak_len;speak_i++)
//	{
//		DataScope_OutPut_Buffer[speak_i+5]=buf3[speak_i];
//	}
//	
//	

//	for( speak_i = 0 ; speak_i < Send_Count; speak_i++) 
//	{
//		while((USART2->SR&0X40)==0);  
//		USART2->DR = DataScope_OutPut_Buffer[speak_i]; 
//	}
//	
//for( speak_i = 0 ; speak_i < 1; speak_i++) 
//	{
//		delay_ms(1864);
//	delay_ms(1864);
//	delay_ms(1864);
//	delay_ms(1864);
//	}


}

////////////////////////////
//begin judge_photo_position
////////////////////////

void judge_photo_position(void)
{
	if(light_check_1==1&&light_check_2==1&&measure_start_flag==0&&touch_left_flag==0x00)//state 0 没开始测量
	{
		measure_start_flag=0x00;
		move_state=0x00;
						
	}
		
	if(light_check_3==1&&light_check_2==1&&light_check_1==0)//state 1//开始测量
	{
		measure_start_flag=0x01;
		touch_left_flag=0x00;
		move_state=0x01;
		if(clc_variable_flag==0x00)
		{
			clear_all_variable_before_begin();
			clc_variable_flag=0x01;
			
			speaker_context_clc_ram_begin();
		}
	}
	
	
	if(light_check_3==1&&light_check_2==0)//state 2//李小燕测量开始
		{
			measure_start_flag=0x01;
			touch_left_flag=0x01;
			move_state=0x02;
			if(send_photto_begin_flag==0x00)
			{
				send_photto_begin_flag=0x01;
				
				DataScope_OutPut_Buffer2[0] = 0xa5;  //帧头
				DataScope_OutPut_Buffer2[1] = 0x5a;  //帧头
				DataScope_OutPut_Buffer2[2] = 0x06;  //数据长度
				DataScope_OutPut_Buffer2[3] = 0x01;  //数据长度
			

				DataScope_OutPut_Buffer2[4] = 0x0d;  //帧尾
				DataScope_OutPut_Buffer2[5] = 0x0a;  //帧尾
			
		
				Send_Count = 0x06;//DataScope_Data_Generate2(2);
				for( s_i = 0 ; s_i < Send_Count; s_i++) 
				{
					while((USART1->SR&0X40)==0);  
					USART1->DR = DataScope_OutPut_Buffer2[s_i]; 
				}
				
				
				speaker_context_photo_start();
			}
		}
			

				
	if(light_check_3==0&&light_check_2==0)//state 3 
	{
		measure_start_flag=0x01;
		touch_left_flag=0x01;
		move_state=0x03;
		touch_left_flag=0x01;
		
	}
	
	if(light_check_3==0&&light_check_2==1&&light_check_1==1)//state 4
	{
		measure_start_flag=0x01;
		move_state=0x04;
		touch_left_flag=0x01;
		send_black_measure_is_over();
		
	}
	
				
	if(light_check_3==1&&light_check_2==1&&measure_start_flag==1&&touch_left_flag==0x01 )//state 5
	{
		measure_start_flag=0x00;
		touch_left_flag=0x00;
		measure_end_flag=1;
		move_state=0x05;
		clc_variable_flag=0x00;
		
	}
}
		





void variable_init(void)
{
	 s1_1=0;
	 s0_1=1;
	 multify_100_flag_1=0x00;
	 
	 //setup  2#light amplify
	 s1_2=0;
	 s0_2=1;
	 multify_100_flag_2=0x00;
	 
	 //setup  3#light amplify
	 s1_3=0;
	 s0_3=1;
	 multify_100_flag_3=0x00;
	 
	 s1_4=0;
	 s0_4=1;
	 multify_100_flag_4=0x00;
	 
	 s1_5=0;
	 s0_5=1;
	 multify_100_flag_5=0x00;
	 
	 
	 theta0_flag=0;
	 jump_down_flag_1=0x00;
	 
	 chnanel_1_num=0x00;
	 chnanel_2_num=0x00;
	 chnanel_3_num=0x00;
	 chnanel_4_num=0x00;
	 chnanel_5_num=0x00;
		 
	 channel_1_aperture_change=0x00;
	 channel_2_aperture_change=0x00;
	 channel_3_aperture_change=0x00;
	 channel_4_aperture_change=0x00;
	 channel_5_aperture_change=0x00;
	
	 send_measure_end_flag=0x00;
	 send_photto_begin_flag=0x00;

	
	  measure_end_flag=0;
	
}


/////////////////////////////
//////////
////////////////////////////
void end_measure_prepare_next_measure(void)
{
	if(chanenel_1_measure_end_flag==0x01 && chanenel_2_measure_end_flag==0x01 && chanenel_3_measure_end_flag==0x01&& chanenel_4_measure_end_flag==0x01&& chanenel_5_measure_end_flag==0x01)
	{	    
//				if(measure_end_flag==0x01)
//			  {		
		
//						LCD_Init();	
				//show_basic_information();
			
//						DataScope_Get_Channel_Data2((float)black_value_max, 1 );
//						DataScope_Get_Channel_Data2((float)black_value_min, 2 );
			
				DataScope_OutPut_Buffer2[0] = 0xa5;  //帧头
				DataScope_OutPut_Buffer2[1] = 0x5a;  //帧头
				DataScope_OutPut_Buffer2[2] = 0x06;  //数据长度
				DataScope_OutPut_Buffer2[3] = 0x02;  //数据长度
			

				DataScope_OutPut_Buffer2[4] = 0x0d;  //帧尾
				DataScope_OutPut_Buffer2[5] = 0x0a;  //帧尾
			
		
				Send_Count =0x06;// DataScope_Data_Generate2(2);
				for( s_i = 0 ; s_i < Send_Count; s_i++) 
				{
					while((USART1->SR&0X40)==0);  
					USART1->DR = DataScope_OutPut_Buffer2[s_i]; 
				}

				chnanel_1_num=0x00;
				chnanel_2_num=0x00;
				chnanel_3_num=0x00;
				chnanel_4_num=0x00;
				chnanel_5_num=0x00;
				
	
				
			
				theta0_flag=0x00;

//						jump_down_flag_1=0x00;
//						jump_down_flag_2=0x00;
//						jump_down_flag_3=0x00;
//						jump_down_flag_4=0x00;
//						jump_down_flag_5=0x00;
				
				chanenel_1_measure_end_flag=0x00;
				chanenel_2_measure_end_flag=0x00;
				chanenel_3_measure_end_flag=0x00;
				chanenel_4_measure_end_flag=0x00;
				chanenel_5_measure_end_flag=0x00;
				
				
				black_frequency0_1=0;
				black_frequency0_2=0;
				black_frequency0_3=0;
				black_frequency0_4=0;
				black_frequency0_5=0;
				

				measure_end_flag=0;
			
				
				measure_start_flag=0x00;
				touch_left_flag=0x00;
				send_measure_end_flag=0x00;
				send_photto_begin_flag=0x00;
				
				clc_variable_flag=0x00;
				
				speaker_context_photo_end_ok();
//					}		
//				else
//				{
//							measure_end_flag=0x00;
//				}

	}
	else
	{
		DataScope_OutPut_Buffer2[0] = 0xa5;  //帧头
		DataScope_OutPut_Buffer2[1] = 0x5a;  //帧头
		DataScope_OutPut_Buffer2[2] = 0x06;  //数据长度
		DataScope_OutPut_Buffer2[3] = 0x02;  //数据长度
	

		DataScope_OutPut_Buffer2[4] = 0x0d;  //帧尾
		DataScope_OutPut_Buffer2[5] = 0x0a;  //帧尾
	

		Send_Count =0x06;// DataScope_Data_Generate2(2);
		for( s_i = 0 ; s_i < Send_Count; s_i++) 
		{
			while((USART1->SR&0X40)==0);  
			USART1->DR = DataScope_OutPut_Buffer2[s_i]; 
		}
		
		speaker_context_photo_end_fail();
		
		analay_fail_reason();
		
		chnanel_1_num=0x00;
		chnanel_2_num=0x00;
		chnanel_3_num=0x00;
		chnanel_4_num=0x00;
		chnanel_5_num=0x00;
		

		
	
		theta0_flag=0x00;

//						jump_down_flag_1=0x00;
//						jump_down_flag_2=0x00;
//						jump_down_flag_3=0x00;
//						jump_down_flag_4=0x00;
//						jump_down_flag_5=0x00;
		
		chanenel_1_measure_end_flag=0x00;
		chanenel_2_measure_end_flag=0x00;
		chanenel_3_measure_end_flag=0x00;
		chanenel_4_measure_end_flag=0x00;
		chanenel_5_measure_end_flag=0x00;
		
		
		black_frequency0_1=0;
		black_frequency0_2=0;
		black_frequency0_3=0;
		black_frequency0_4=0;
		black_frequency0_5=0;
		

		measure_end_flag=0;
	
		
		measure_start_flag=0x00;
		touch_left_flag=0x00;
		send_measure_end_flag=0x00;
		send_photto_begin_flag=0x00;
		
		clc_variable_flag=0x00;
		
		
		
	}
	
	

	
}	
//////////////////////
//
//////
///////////////////////
void analay_fail_reason(void)
{
	//1#
	if(chnanel_1_num<5)
	{
		speaker_1_fail_no_start();
	}
	
	if((chnanel_1_num>5) && (chanenel_1_measure_end_flag==0x00))
	{
		speaker_1_fail_no_end();
	}
	
	//2#
	if(chnanel_2_num<5)
	{
		speaker_2_fail_no_start();
	}
	
	if((chnanel_2_num>5)&&(chanenel_2_measure_end_flag==0x00))
	{
		speaker_2_fail_no_end();
	}
	
	//3#
	if(chnanel_3_num<5)
	{
		speaker_3_fail_no_start();
	}
	
	if((chnanel_3_num>5)&&(chanenel_3_measure_end_flag==0x00))
	{
		speaker_3_fail_no_end();
	}
	
	//4#
	if(chnanel_4_num<5)
	{
		speaker_4_fail_no_start();
	}
	
	if((chnanel_4_num>5)&&(chanenel_4_measure_end_flag==0x00))
	{
		speaker_4_fail_no_end();
	}
	
	//5#
	if(chnanel_5_num<5)
	{
		speaker_5_fail_no_start();
	}
	
	if((chnanel_5_num>5)&&(chanenel_5_measure_end_flag==0x00))
	{
		speaker_5_fail_no_end();
	}
	
}










//ALIENTEK 探索者STM32F407开发板 实验13




////////////////////////////////




 int main(void)
 {	 

//		u8 x=0;
	 
	 

//		u8 lcd_id[12];			//存放LCD ID字符串

	 delay_init();	    	 //延时函数初始化	  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		uart_init(115200);	 	//串口初始化为115200
		uart2_init(9600);	 	//串口初始化为115200
	  uart3_init(115200);	 	//串口初始化为115200
	 
	 
		LED_Init();			     //LED端口初始化
	  //LCD_Init();
	 
		OLED_Init();         //OLED显示初始化
		OLED_P8x16Str(0, 0, "stall= 0", 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
		OLED_P8x16Str(0, 2, "width=00080 CM", 0); /*由于6050初始化较为复杂，所以显示其初始化状态*/
		check_io_ini();
		tlr230_s0_s1_ini();
	 
		variable_init();
	
		 //setup  1#light amplify
		
		 
		
		 TIM3_Int_Init(0xffff-1,1-1);//2# plus counter
		 TIM4_Int_Init(0xffff-1,1-1);//3# plus counter
		 TIM1_Int_Init(0xffff-1,1-1);//4# plus counter
		 TIM8_Int_Init(0xffff-1,1-1);//5# plus counter
		 
		 TIM5_Int_Init(500-1,7200-1);//0.1 second period
		 TIM6_Int_Init(107-1,72-1);//0.1 second period
		  
		 TIM2_Int_Init(0xffff-1,1-1);//1# plus counter//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     /*使能SWD 禁用JTAG*/ 
		 over_flow_t2=0x00;//1# channel 
		 over_flow_t3=0x00;//2# channel 
		 over_flow_t4=0x00;//3# channel 
		 over_flow_t1=0x00;//4# channel 
	   over_flow_t8=0x00;//5# channel 
		 
		 black_frequency_1=0x01;//1# channel 
		 black_frequency_2=0x01;//2# channel 
		 black_frequency_3=0x01;//3# channel 
		 black_frequency_4=0x01;//4# channel 
		 black_frequency_5=0x01;//5# channel 
		 
		 chanenel_1_measure_end_flag=0x00;
		 
		 count_delay=0x0a;
		 
		 clc_variable_flag=0x00;
	 
		 test_usart1_3_command();
		 
		 speaker_test();

		POINT_COLOR=RED;
		
	
		//////////
		///  main cycle 2018.03.29 15:00
		// dead cycle main purpose is wait 0.1s arrive,and then caculeter the blace value,display
		while(1) 
		{		 
			
				//judge_photo_position();	
					if(light_check_1==1&&light_check_2==1&&measure_start_flag==0&&touch_left_flag==0x00)//state 0 没开始测量
					{
						measure_start_flag=0x00;
						move_state=0x00;
										
					}
						
					if(light_check_3==1&&light_check_2==1&&light_check_1==0)//state 1//开始测量
					{
						measure_start_flag=0x01;
						touch_left_flag=0x00;
						move_state=0x01;
						if(clc_variable_flag==0x00)
						{
							clear_all_variable_before_begin();
							clc_variable_flag=0x01;
							
							speaker_context_clc_ram_begin();
						}
					}
					
					
					if(light_check_3==1&&light_check_2==0)//state 2//李小燕测量开始
						{
							measure_start_flag=0x01;
							touch_left_flag=0x01;
							move_state=0x02;
							if(send_photto_begin_flag==0x00)
							{
								send_photto_begin_flag=0x01;
								
								DataScope_OutPut_Buffer2[0] = 0xa5;  //帧头
								DataScope_OutPut_Buffer2[1] = 0x5a;  //帧头
								DataScope_OutPut_Buffer2[2] = 0x06;  //数据长度
								DataScope_OutPut_Buffer2[3] = 0x01;  //数据长度
							

								DataScope_OutPut_Buffer2[4] = 0x0d;  //帧尾
								DataScope_OutPut_Buffer2[5] = 0x0a;  //帧尾
							
						
								Send_Count = 0x06;//DataScope_Data_Generate2(2);
								for( s_i = 0 ; s_i < Send_Count; s_i++) 
								{
									while((USART1->SR&0X40)==0);  
									USART1->DR = DataScope_OutPut_Buffer2[s_i]; 
								}
								
								
								speaker_context_photo_start();
							}
						}
							

								
					if(light_check_3==0&&light_check_2==0)//state 3 
					{
						measure_start_flag=0x01;
						touch_left_flag=0x01;
						move_state=0x03;
						touch_left_flag=0x01;
						count_state_5=0x00;
						
					}
					
					if(light_check_3==0&&light_check_2==1&&light_check_1==1&&chanenel_4_measure_end_flag==1&&chanenel_2_measure_end_flag==1)//state 4
					{
						measure_start_flag=0x01;
						move_state=0x04;
						touch_left_flag=0x01;
						send_black_measure_is_over();
						count_state_5=0x00;
						
					}
					
								
					if(light_check_3==1&&light_check_2==1&&measure_start_flag==1&&touch_left_flag==0x01 )//state 5
					{
						count_state_5++;
						if(count_state_5>25)
						{
							measure_start_flag=0x00;
							touch_left_flag=0x00;
							measure_end_flag=1;
							move_state=0x05;
							clc_variable_flag=0x00;
						}
						
						
					}
				
				
				if(measure_start_flag==1)
				{
					//measure begin measuring 
					//begin_measure();
						if (display_flag==0x01)
						{
							
							if(theta0_flag==0x00)
							{
									black_frequency0_1=(float)black_frequency_1;
									black_frequency0_2=(float)black_frequency_2;
									black_frequency0_3=(float)black_frequency_3;
									black_frequency0_4=(float)black_frequency_4;
									black_frequency0_5=(float)black_frequency_5;

								
								if((black_frequency0_1>100)&&(black_frequency0_2>100)&&(black_frequency0_3>100)&&(black_frequency0_4>100)&&(black_frequency0_5>100))
								{
									theta0_flag=0x01;
									speaker_context_get_thita0();
								}
								
								
								if(black_frequency0_1<100)
								{
									theta0_flag=0x00;
								}
								
							}
					
							channel_1_calute_black_value_begin_end_save_data_adjust_aperture();
							
							channel_2_calute_black_value_begin_end_save_data_adjust_aperture();
							
							channel_3_calute_black_value_begin_end_save_data_adjust_aperture();
							
							channel_4_calute_black_value_begin_end_save_data_adjust_aperture();
							
							channel_5_calute_black_value_begin_end_save_data_adjust_aperture();
							

							 
									
							DataScope_Get_Channel_Data((float)black_value_original_1, 1 );
							DataScope_Get_Channel_Data((float)black_value_original_2, 2 );
							DataScope_Get_Channel_Data((float)black_value_original_3, 3 );
							DataScope_Get_Channel_Data((float)black_value_original_4, 4 );
							DataScope_Get_Channel_Data((float)black_value_original_5, 5 );

					
							Send_Count = DataScope_Data_Generate(5);
							for( s_i = 0 ; s_i < Send_Count; s_i++) 
							{
								while((USART3->SR&0X40)==0);  
								USART3->DR = DataScope_OutPut_Buffer[s_i]; 
							}
								
							display_flag=0x00;
					}
					
				}
				///////////////////////
				//
				/////////////////////////
				if(measure_end_flag==0x01)
				{	
					// measure is end ,prepare next measure
					//end_measure_prepare_next_measure();
					
					
						if(chanenel_1_measure_end_flag==0x01 && chanenel_2_measure_end_flag==0x01 && chanenel_3_measure_end_flag==0x01&& chanenel_4_measure_end_flag==0x01&& chanenel_5_measure_end_flag==0x01)
						{	    
		
							DataScope_OutPut_Buffer2[0] = 0xa5;  //帧头
							DataScope_OutPut_Buffer2[1] = 0x5a;  //帧头
							DataScope_OutPut_Buffer2[2] = 0x06;  //数据长度
							DataScope_OutPut_Buffer2[3] = 0x02;  //数据长度
						

							DataScope_OutPut_Buffer2[4] = 0x0d;  //帧尾
							DataScope_OutPut_Buffer2[5] = 0x0a;  //帧尾
						
					
							Send_Count =0x06;// DataScope_Data_Generate2(2);
							for( s_i = 0 ; s_i < Send_Count; s_i++) 
							{
								while((USART1->SR&0X40)==0);  
								USART1->DR = DataScope_OutPut_Buffer2[s_i]; 
							}

							chnanel_1_num=0x00;
							chnanel_2_num=0x00;
							chnanel_3_num=0x00;
							chnanel_4_num=0x00;
							chnanel_5_num=0x00;
							
				
							
						
							theta0_flag=0x00;

			//						jump_down_flag_1=0x00;
			//						jump_down_flag_2=0x00;
			//						jump_down_flag_3=0x00;
			//						jump_down_flag_4=0x00;
			//						jump_down_flag_5=0x00;
							
							chanenel_1_measure_end_flag=0x00;
							chanenel_2_measure_end_flag=0x00;
							chanenel_3_measure_end_flag=0x00;
							chanenel_4_measure_end_flag=0x00;
							chanenel_5_measure_end_flag=0x00;
							
							
							black_frequency0_1=0;
							black_frequency0_2=0;
							black_frequency0_3=0;
							black_frequency0_4=0;
							black_frequency0_5=0;
							

							measure_end_flag=0;
						
							
							measure_start_flag=0x00;
							touch_left_flag=0x00;
							send_measure_end_flag=0x00;
							send_photto_begin_flag=0x00;
							
							clc_variable_flag=0x00;
							
							speaker_context_photo_end_ok();
			//					}		
			//				else
			//				{
			//							measure_end_flag=0x00;
			//				}

				}
				else
				{
					DataScope_OutPut_Buffer2[0] = 0xa5;  //帧头
					DataScope_OutPut_Buffer2[1] = 0x5a;  //帧头
					DataScope_OutPut_Buffer2[2] = 0x06;  //数据长度
					DataScope_OutPut_Buffer2[3] = 0x02;  //数据长度
				

					DataScope_OutPut_Buffer2[4] = 0x0d;  //帧尾
					DataScope_OutPut_Buffer2[5] = 0x0a;  //帧尾
				

					Send_Count =0x06;// DataScope_Data_Generate2(2);
					for( s_i = 0 ; s_i < Send_Count; s_i++) 
					{
						while((USART1->SR&0X40)==0);  
						USART1->DR = DataScope_OutPut_Buffer2[s_i]; 
					}
					
					speaker_context_photo_end_fail();
					
					analay_fail_reason();
					
					chnanel_1_num=0x00;
					chnanel_2_num=0x00;
					chnanel_3_num=0x00;
					chnanel_4_num=0x00;
					chnanel_5_num=0x00;
					

					
				
					theta0_flag=0x00;

			//						jump_down_flag_1=0x00;
			//						jump_down_flag_2=0x00;
			//						jump_down_flag_3=0x00;
			//						jump_down_flag_4=0x00;
			//						jump_down_flag_5=0x00;
					
					chanenel_1_measure_end_flag=0x00;
					chanenel_2_measure_end_flag=0x00;
					chanenel_3_measure_end_flag=0x00;
					chanenel_4_measure_end_flag=0x00;
					chanenel_5_measure_end_flag=0x00;
					
					
					black_frequency0_1=0;
					black_frequency0_2=0;
					black_frequency0_3=0;
					black_frequency0_4=0;
					black_frequency0_5=0;
					

					measure_end_flag=0;
				
					
					measure_start_flag=0x00;
					touch_left_flag=0x00;
					send_measure_end_flag=0x00;
					send_photto_begin_flag=0x00;
					
					clc_variable_flag=0x00;
					
					
					
				}
					
					
					
					
					
					
					
					
					
					
					
					
					
				}
					delay_ms(2);	
					count_delay++;
			
		
		
			
		}  //end while(1)
			
						
			
 } 




