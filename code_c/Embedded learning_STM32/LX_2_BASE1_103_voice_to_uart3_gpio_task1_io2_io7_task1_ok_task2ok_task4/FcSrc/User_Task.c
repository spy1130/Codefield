#include "User_Task.h"
#include "Drv_RcIn.h"


#include "Drv_Uart.h"


#include "LX_FC_Fun.h"
#include "ANO_DT_LX.h"
#include "main.h"

#include "stdio.h"

#include <string.h>
#include <math.h>//调用三角函数
#include "main.h"

#include "User_Task.h"
#include <stdlib.h>//调用三角函数
#include "Drv_Sys.h"

#include "Drv_Uart.h"
#include "I2C_Init.h"

extern float		sum_x_aa;
extern u8 m1_step,m2_step,m3_step,m4_step;
extern u8 rx_buff_com1[20];
struct point
{
	float x;
	float y;
};

struct point sow_piont[12]={{4.77,3.65},{3.27,5.20},{3.99,5.98},{1.76,6.83},{4.83,6.73},{2.54,5.94},{2.56,4.55},{3.99,4.44},{3.28,3.66},\
														{1.79,5.25},{4.83,5.23},{3.3,6.71}};

int sow_add[27]={28,27,20,19,26,25,18,14,10,8,4,3,7,9,3,7,24,23,16,6,2,1,5,11,15,22};
#define state_num 18
	
float home_x=1.9;
float home_y=3.3;

//////////////////////////////////////////////////////////////////////////////////////////////
//follow_line()
//////////////////////////////////////////////////////////////////////////////////////////
int16_t pole_x=0,pole_y=0,pole_w=0,pole_w_middle=0;
int16_t line_x=0,line_y=0,line_sita=0;
int time_dly_cnt_ms_xm=0,time_dly_cnt_ms=0;
int16_t x_position=0,y_position=0,y_position_last=0;
char c, type;
double plant_x_init,plant_y_init,plant_z,act_x=4.19,act_y=4.46;
double plant_x,plant_y;
float error_x=0,error_y=0;
int add_n=0;
	
	
float altitude_set=130,p_altitude=1.0,speed_z_axis=0,speed_x_axis=0,speed_y_axis=0,p_longitude=10.0;
	
float altitude_set_1=80	;

int16_t speed_x_axis_16=0,speed_y_axis_16=0,speed_z_axis_16=0,yaw_16=0,p_speed_y_axis_16=0,d_speed_y_axis_16=0,speed_yaw_axis_16=0;										

u8 mission_step=0,near_alarm_flag=0,far_alarm_flag=0,near_alarm_count=0;

u8 far_alarm_count=0,first_a0_voice=1,cnt_20ms_no_signal=0,flag_45=0;

float cons_kp=0.08,cons_kd=0.2;

int i,cnt_20=0,trun_on_time=0;

u8 period_dis=0,basic_step=0,light_flag=0,s_i_xm=0,Send_Count_xm=0;

u8 DataScope_OutPut_Buffer_xm11[10]={0};

u8 index_sow_i=0;
float tar_temp_x,tar_temp_x,tar_temp_x;
float dis_sonic=0;


u8 position_1_order=0x00;
u8 position_2_order=0x00;
u8 tx1_buf_xm[6]={0};

int box_down_flag=0x00;
int box_count=0,result_target=0;

int record_cnt=0x00,record_arry[10]={0},start_iden_flag=0x00,m2_time_cnt=0x00;//;












void task1_UserTask_OneKeyCmd(void);
void stop_basket(void)
{
	steering_pd12=0;
	steering_pd13=0;
	
}

void up_basket(void)
{
	steering_pd12=1;
	steering_pd13=0;
}


void down_basket(void)
{
	steering_pd12=0;
	steering_pd13=1;
}



///////////////////////////////////
void sow_seed(void)
{
	
	GPIO_SetBits(GPIOD,GPIO_Pin_15);//GPIOF9,F10设置高，灯灭
	light_flag=1;
	
//	MyDelayMs(500);
//	
//	GPIO_ResetBits(GPIOD,GPIO_Pin_15);//GPIOF9,F10设置高，灯灭
//	light_flag=0;
	
	
	
}

int find_target(float target_x,float target_y,float target_z)
{

	if (((plant_x>=(target_x-0.15f))&&(plant_x<=(target_x+0.15f)))&&((plant_y>(target_y-0.15f))&&(plant_y<=(target_y+0.15f))))
		{
			sow_seed();
			index_sow_i=0;
			
			sprintf(buf1,"[s9]到达第一个区域");//\[b1] 欢迎光临，请进
			speak_len1=strlen(buf1);
			speak_context((u8*)buf1,speak_len1);
			
			
			LxStringSend(1,"到达第二个区域，准备下降高度");
			
			//time_dly_cnt_ms = 0;
			//mission_step += 1;
		
//			first_a0_voice=1;
//			time_dly_cnt_ms_xm=0;
			return 1;
		 }
		 else
		 {
			dis_sonic=sum_x_aa;		
			speed_x_axis=p_longitude*(target_x-plant_x);//PIDX轴控制
			speed_x_axis_16=(int16_t) speed_x_axis;
			if(speed_x_axis_16>20)
			{
				speed_x_axis_16=10;
			}
			if(speed_x_axis_16<-20)
			{
				speed_x_axis_16=-10;
			}
			
			if((speed_x_axis_16<5)&&(speed_x_axis_16>0))
			{
				speed_x_axis_16=4;
			}
			
			if((speed_x_axis_16<0)&&(speed_x_axis_16>-5))
			{
				speed_x_axis_16=-4;
			}
			
			speed_y_axis=p_longitude*(plant_y-target_y);//PIDY轴控制  act_y-plant_y
			speed_y_axis_16=(int16_t) speed_y_axis;
			if(speed_y_axis_16>20)
			{
				speed_y_axis_16=10;
			}
			if(speed_y_axis_16<-20)
			{
				speed_y_axis_16=-10;
			}
			
			
			if((speed_y_axis_16<5)&&(speed_y_axis_16>0))
			{
				speed_y_axis_16=4;
			}
			
			if((speed_y_axis_16<0)&&(speed_y_axis_16>-5))
			{
				speed_y_axis_16=-4;
			}
	
			speed_z_axis=p_altitude*(target_z-alt_add_xm);//PID高度控制
			speed_z_axis_16=(int16_t) speed_z_axis;
		
			rt_tar.st_data.rol = 0;
			rt_tar.st_data.pit = 0;
			rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
			//这里会把实时XYZ-YAW期望速度置零		
			rt_tar.st_data.yaw_dps=0;// yaw_16;
			rt_tar.st_data.vel_x =speed_x_axis_16;//speed_x_axis_16;
			rt_tar.st_data.vel_y =speed_y_axis_16;//
			rt_tar.st_data.vel_z =speed_z_axis_16;

			dt.fun[0x41].WTS = 1; //将要发送rt_tar数据。
			
			if(time_dly_cnt_ms_xm%300==0)
			{
				if((speed_x_axis_16>0)&&(speed_y_axis_16>0))//右手定理
				{
					LxStringSend(1,"前进+1,向左+1");
					sprintf(buf1,"[s9]前左");//\[b1] 欢迎光临，请进
					speak_len1=strlen(buf1);
					speak_context((u8*)buf1,speak_len1);
				}
				else if((speed_x_axis_16>0)&&(speed_y_axis_16<0))
				{
					LxStringSend(1,"前进+1,向右+1");
					sprintf(buf1,"[s9]前右");//\[b1] 欢迎光临，请进
					speak_len1=strlen(buf1);
					speak_context((u8*)buf1,speak_len1);
				}
				else if((speed_x_axis_16<0)&&(speed_y_axis_16>0))
				{
					LxStringSend(1,"后退+1,向左+1");
					sprintf(buf1,"[s9]后左");//\[b1] 欢迎光临，请进
					speak_len1=strlen(buf1);
					speak_context((u8*)buf1,speak_len1);
				}
				else if((speed_x_axis_16<0)&&(speed_y_axis_16<0))
				{
					LxStringSend(1,"后退+1,向右+1");
					sprintf(buf1,"[s9]后右");//\[b1] 欢迎光临，请进
					speak_len1=strlen(buf1);
					speak_context((u8*)buf1,speak_len1);
				}
				else
				{
					
				}

			}
			
			return 0;
								
    }



}













/////////////////////////////////////////////////////////////////////////////////////////////
//解析数据
/////////////////////////////////////////////////////////////////////////////////////////

void uart3_camera_command_analyse(void)
{
	
	if( rx_end_flag==0x01)
	{
		
		int n2 = sscanf((char*)rx_buff,"$KT%c, %lf,%lf, %lf ", &type,  &plant_x_init, &plant_y_init, &plant_z);
		
		plant_x = (-0.0057)*pow(plant_x_init,4) + 0.1462*pow(plant_x_init,3) + (-1.087)*pow(plant_x_init,2) + 4.101*plant_x_init - 3.178;
		
		plant_y = plant_y_init;
				
		rx_end_flag=0;
		first_frame_end=0;
	}
		
}	


void uart2_openmv(void)
{
	
	if( rx_end_flag==0x01)
	{
//		first_frame_end=0;
//		second_frame_end=0;
//		rx_end_flag=0;
		
		if(rx_buff[1]==0xf0)//ploe
		{
			pole_x=(rx_buff[3]<<8|rx_buff[4]);//YAW，PITCH,ROLL为真实值的100倍
			pole_y=(rx_buff[5]<<8|rx_buff[6]);
			pole_w=(rx_buff[7]<<8|rx_buff[8]);
//			ROLL=(rx_buff[5]<<8|rx_buff[6]);
		 
		
		}
		
		if(rx_buff[1]==0xf1)//line
		{
			line_x=(rx_buff[3]<<8|rx_buff[4]);//YAW，PITCH,ROLL为真实值的100倍
			line_y=(rx_buff[5]<<8|rx_buff[6]);//YAW，PITCH,ROLL为真实值的100倍
			line_sita=(rx_buff[7]<<8|rx_buff[8]);
			
			
			
		}
		
		rx_end_flag=0;
		first_frame_end=0;
		
		
	}			
	
	 
	
}











//////////////////////////////////////////////////////////////////////////////////////////////
//开始作业1
//////////////////////////////////////////////////////////////////////////////////////////

void UserTask_task_one(void)
{
	switch(m1_step)
	{
		case 0:
		{
			//reset
			if(1==rx_end_flag_com1)
			{
				rx_end_flag_com1=0;
				position_1_order= rx_buff_com1[3];
				position_2_order= rx_buff_com1[4];
				
				tx1_buf_xm[0]=0xa5;
				tx1_buf_xm[1]=0x5a;
				tx1_buf_xm[2]=0x03;
				tx1_buf_xm[3]=0x00;
				tx1_buf_xm[4]=0xfd;
				
				DrvUart1SendBuf(tx1_buf_xm,5);
				
				m1_step=0x01;
				
				sprintf(buf1,"[s8]收到数据，准备前往%d和%d送货点",position_1_order,position_2_order);//飞控解锁
				speak_len1=strlen(buf1);
				speak_context((u8*)buf1,speak_len1);
				
				
			}
			else
			{
				
			}
				
			
		}
		break;
		case 1:
		{
			//reset
			task1_UserTask_OneKeyCmd();
		}
		break;
		
		default:
			break;
     }
}



//////////////////////////////////////////////////////////////////////////////////////////////
//开始作业2
//////////////////////////////////////////////////////////////////////////////////////////
void UserTask_task_two(void)
{
	switch(m2_step)
	{
		case 0:
		{  
			if(1==task_pc6)//开始识别物体
			{
				if (start_iden_flag==0)
				{
					start_iden_flag=1;
					
					record_cnt=0x00;
					
					sprintf(buf1,"[s8]开始识别物体");//飞控解锁
					speak_len1=strlen(buf1);
					speak_context((u8*)buf1,speak_len1);

				}
				else//开识别物体
				{
						if( rx_end_flag==0x01)
						{
					//		first_frame_end=0;
					//		second_frame_end=0;
					//		rx_end_flag=0;
							
							if(rx_buff[1]==0xf0)//ploe
							{
								pole_x=(rx_buff[3]<<8|rx_buff[4]);//YAW，PITCH,ROLL为真实值的100倍
								pole_y=(rx_buff[5]<<8|rx_buff[6]);
								pole_w=(rx_buff[7]<<8|rx_buff[8]);
					//			ROLL=(rx_buff[5]<<8|rx_buff[6]);
							 
							
							}
							
							if(rx_buff[1]==0xf1)//line
							{
								line_x=(rx_buff[3]<<8|rx_buff[4]);//YAW，PITCH,ROLL为真实值的100倍
								line_y=(rx_buff[5]<<8|rx_buff[6]);//YAW，PITCH,ROLL为真实值的100倍
								line_sita=(rx_buff[7]<<8|rx_buff[8]);
								
								
								
							}
							
							record_arry[record_cnt]=line_sita;
							
							record_cnt++;
							
							if(record_cnt>=0x04)
							{
								if((record_arry[0]==record_arry[1])&&(record_arry[1]==record_arry[2])&&(record_arry[2]==record_arry[3]))
								{
									//识别三次相同，播报结果，请人确认
									switch (record_arry[0])
                  {
                  	case 0:
											sprintf(buf1,"[s8]失败");//飞控解锁
											speak_len1=strlen(buf1);
											speak_context((u8*)buf1,speak_len1);
											
											start_iden_flag=0x00;
											
                  		break;
                  	case 1:
											sprintf(buf1,"[s8]蓝色原形");//飞控解锁
											speak_len1=strlen(buf1);
											speak_context((u8*)buf1,speak_len1);
											
											position_1_order=6;
											position_2_order=7;
										
											m2_step=0x01;
											m2_time_cnt=0x00;
										
                  		break;
										case 2:
											sprintf(buf1,"[s8]蓝色正方形");//飞控解锁
											speak_len1=strlen(buf1);
											speak_context((u8*)buf1,speak_len1);
										
											position_1_order=10;
											position_2_order=11;
											m2_time_cnt=0x00;
											m2_step=0x01;
                  		break;
                  	case 3:
											sprintf(buf1,"[s8]蓝色三角形");//飞控解锁
											speak_len1=strlen(buf1);
											speak_context((u8*)buf1,speak_len1);
											m2_time_cnt=0x00;
											position_1_order=2;
											position_2_order=3;
											m2_step=0x01;
                  		break;
										
										case 4:
											sprintf(buf1,"[s8]失败");//飞控解锁
											speak_len1=strlen(buf1);
											speak_context((u8*)buf1,speak_len1);
											
											start_iden_flag=0x00;
                  		break;
                  	case 5:
											sprintf(buf1,"[s8]红色圆形");//飞控解锁
											speak_len1=strlen(buf1);
											speak_context((u8*)buf1,speak_len1);
											m2_time_cnt=0x00;
											position_1_order=4;
											position_2_order=5;
											m2_step=0x01;
                  		break;
										
										case 6:
											sprintf(buf1,"[s8]红色正方形");//飞控解锁
											speak_len1=strlen(buf1);
											speak_context((u8*)buf1,speak_len1);
										
											position_1_order=8;
											position_2_order=9;
											m2_time_cnt=0x00;
											m2_step=0x01;
                  		break;
                  	case 7:
											sprintf(buf1,"[s8]红色三角形");//飞控解锁
											speak_len1=strlen(buf1);
											speak_context((u8*)buf1,speak_len1);
											m2_time_cnt=0x00;
											position_1_order=0;
											position_2_order=1;
											m2_step=0x01;
                  		break;
                  	default:
                  		break;
                  }
									
								}
								else//识别失败，重新学习
								{
									sprintf(buf1,"[s8]失败");//飞控解锁
									speak_len1=strlen(buf1);
									speak_context((u8*)buf1,speak_len1);
									
									start_iden_flag=0x00;

								}
							}
							
							
							
							rx_end_flag=0;
							first_frame_end=0;
							
							
						}	
				}					
				
			}
			else//等待pc6跳线帽拔开
			{
				
				//等待pc6拨出后，开始学习
				
			}
			//reset

		}



		
		break;
		case 1:
		{
			//reset
			m2_time_cnt++;
			if(task_pc6==0)
			{
				//表示确认识别结果
				
				sprintf(buf1,"[s8]马上前往%d 号 和  %d 号快递点",position_1_order+1,position_2_order+1);//飞控解锁
					speak_len1=strlen(buf1);
					speak_context((u8*)buf1,speak_len1);
				
				m2_step=0x02;
				
			}
			else
			{
				//结果有误，4秒内不要插回pc6，即表示结果有误，单片机重学
				if(m2_time_cnt>=250)
				{
					sprintf(buf1,"[s8]识别错误");//飞控解锁
					speak_len1=strlen(buf1);
					speak_context((u8*)buf1,speak_len1);
					
					start_iden_flag=0x00;
					m2_step=0x00;
				}
			}
		}
		case 2:
		{
			//reset
			task1_UserTask_OneKeyCmd();
		}
		break;
		
		default:
			break;
     }
	
}


//////////////////////////////////////////////////////////////////////////////////////////////
//开始作业3
//////////////////////////////////////////////////////////////////////////////////////////
void UserTask_task_three(void)
{
	
}

//////////////////////////////////////////////////////////////////////////////////////////////
//开始作业4
//////////////////////////////////////////////////////////////////////////////////////////
void UserTask_task_four(void)
{
	switch(m1_step)
	{
		case 0:
		{
			//reset
			if(1==rx_end_flag_com1)
			{
				rx_end_flag_com1=0;
				position_1_order= rx_buff_com1[3];
				position_2_order= rx_buff_com1[4];
				
				tx1_buf_xm[0]=0xa5;
				tx1_buf_xm[1]=0x5a;
				tx1_buf_xm[2]=0x03;
				tx1_buf_xm[3]=0x00;
				tx1_buf_xm[4]=0xfd;
				
				DrvUart1SendBuf(tx1_buf_xm,5);
				
				m1_step=0x01;
				
				sprintf(buf1,"[s8]收到数据，准备前往%d和%d送货点",position_1_order,position_2_order);//飞控解锁
				speak_len1=strlen(buf1);
				speak_context((u8*)buf1,speak_len1);
				
				
			}
			else
			{
				
			}
				
			
		}
		break;
		case 1:
		{
			//reset
			task1_UserTask_OneKeyCmd();
		}
		break;
		
		default:
			break;
     }
}


//////////////////////////////////////////////////////////////////////////////////////////////
//开始作业
//////////////////////////////////////////////////////////////////////////////////////////
void task1_UserTask_OneKeyCmd(void)
{
    //////////////////////////////////////////////////////////////////////
    //一键起飞/降落例程
    //////////////////////////////////////////////////////////////////////
    //用静态变量记录一键起飞/降落指令已经执行。
    static u8 one_key_takeoff_f = 1, one_key_land_f = 1, one_key_mission_f = 0;
    static u8 last_switch=0,now_switch=0;
	
    //判断有遥控信号才执行
    if (rc_in.no_signal == 0)//收到信号标志位为0
    {
			  //判断第6通道拨杆位置 800<CH_6<1200 1000  c号开关拨到了1挡
        if (rc_in.rc_ch.st_data.ch_[ch_6_aux2] > 800 && rc_in.rc_ch.st_data.ch_[ch_6_aux2] < 1200)
        {
            //还没有执行
            if (one_key_land_f == 0)
            {
                //标记已经执行
                one_key_land_f =  OneKey_Land(); //执行一键降落
                 
            }
			now_switch=1;
		
			if(last_switch!=now_switch)
			{
				last_switch=now_switch;
				sprintf(buf1,"[x1] sound201 ");//\[b1] 欢迎光临，请进
				speak_len1=strlen(buf1);
				speak_context((u8*)buf1,speak_len1);
				
				LxStringSend(1,"1档");
				
			}
				
				
        }
        else
        {
            //复位标记，以便再次执行
            one_key_land_f = 0;
        }
			
			//判断第6通道拨杆位置 1300<CH_6<1700  1500   c号开关拨到了2挡
        if (rc_in.rc_ch.st_data.ch_[ch_6_aux2] > 1300 && rc_in.rc_ch.st_data.ch_[ch_6_aux2] < 1700)
        {
					
					if (one_key_takeoff_f == 0)
					{	
						
						one_key_mission_f = 0;
						mission_step = 1;
						
						now_switch=2;
					
						if(last_switch!=now_switch)
						{
							last_switch=now_switch;
							sprintf(buf1,"[x1] sound203 ");//\[b1] 欢迎光临，请进
							speak_len1=strlen(buf1);
							speak_context((u8*)buf1,speak_len1);
							
							LxStringSend(1,"2档");
							first_a0_voice=1;
							
							find_pole_flag=0;
							first_frame_end=0;
							second_frame_end=0;
					
							rt_tar.st_data.rol = 0;
							rt_tar.st_data.pit = 0;
							rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
							//这里会把实时XYZ-YAW期望速度置零		
							rt_tar.st_data.yaw_dps =0;// yaw_16;
							rt_tar.st_data.vel_x =0;//speed_x_axis_16;
							rt_tar.st_data.vel_y =0;//;
							rt_tar.st_data.vel_z = 0;

							dt.fun[0x41].WTS = 1; //将要发送rt_tar数据。

						}	
				  }						
        }
        else
        {
            //复位标记，以便再次执行
            one_key_takeoff_f = 0;
        }
    
	//判断第6通道拨杆位置 1700<CH_6<2200  c号开关拨到了三挡
		if(rc_in.rc_ch.st_data.ch_[ch_6_aux2]>1700 && rc_in.rc_ch.st_data.ch_[ch_6_aux2]<2200)
		{
			//还没有执行
			if(one_key_mission_f ==0)
			{
				//标记已经执行
				one_key_mission_f = 1;
				//开始流程
				mission_step = 1;
				//可以播放语音			
			}
			
			now_switch=3;
			last_switch=3;
		}
		else
		{
			//复位标记，以便再次执行
			one_key_mission_f = 0;		
		}
		//
		if(one_key_mission_f==1)
		{
			static int time_dly_cnt_ms;
			//
			switch(mission_step)
			{
				case 0:
				{
					//reset
					time_dly_cnt_ms = 0;
					first_a0_voice=1;
				}
				break;
				case 1:
				{
					mission_step=mission_step+1;
					first_a0_voice=1;
					
					rt_tar.st_data.rol = 0;
					rt_tar.st_data.pit = 0;
					rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
					//这里会把实时XYZ-YAW期望速度置零		
					rt_tar.st_data.yaw_dps = 0;// yaw_16;
					rt_tar.st_data.vel_x = 0;//speed_x_axis_16;
					rt_tar.st_data.vel_y = 0;//;
					rt_tar.st_data.vel_z = 0;

					dt.fun[0x41].WTS = 1; //将要发送rt_tar数据。
				}
				break;
				case 2:
				{
					//解锁
					mission_step += FC_Unlock();
					
					if(first_a0_voice==1)
					{
						sprintf(buf1,"[s8]解锁");//飞控解锁
						speak_len1=strlen(buf1);
						speak_context((u8*)buf1,speak_len1);
						
						LxStringSend(1,"解锁");
						
						first_a0_voice=0;
					}
					first_a0_voice=1;
					 
				}
				break;
				case 3:
				{
					//等2秒
					if(time_dly_cnt_ms<2000)
					{
						time_dly_cnt_ms+=20;//ms
						
						
						if (time_dly_cnt_ms<1000&&time_dly_cnt_ms>100)
						{
							rt_tar.st_data.rol = 0;
							rt_tar.st_data.pit = 0;
							rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
							//这里会把实时XYZ-YAW期望速度置零		
							rt_tar.st_data.yaw_dps = 0;// yaw_16;
							rt_tar.st_data.vel_x = 0;//speed_x_axis_16;
							rt_tar.st_data.vel_y = 0;//;
							rt_tar.st_data.vel_z = 0;

							dt.fun[0x41].WTS = 1; //将要发送rt_tar数据。
						}
						
					}
					else
					{
						time_dly_cnt_ms = 0;
						mission_step += 1;
						first_a0_voice=1;
					}
				}
				break;
				case 4:
				{
					//模式切换
					mission_step += LX_Change_Mode(2);
				    time_dly_cnt_ms = 0;
						
					if(first_a0_voice==1)
					{
						sprintf(buf1,"[s9]模式2，z轴零速");//\[b1] 欢迎光临，请进
						speak_len1=strlen(buf1);
						speak_context((u8*)buf1,speak_len1);
						
						LxStringSend(1,"模式2，z轴零速");
						
						first_a0_voice=0;
					}
					
					first_a0_voice=1;
									
				}
				break;
				case 5:
				{
					//等2秒
				if(time_dly_cnt_ms<2000)
				{
					time_dly_cnt_ms+=20;//ms
					
					if (time_dly_cnt_ms<1000&&time_dly_cnt_ms>100)
					{
						rt_tar.st_data.rol = 0;
						rt_tar.st_data.pit = 0;
						rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
						//这里会把实时XYZ-YAW期望速度置零		
						rt_tar.st_data.yaw_dps =0;// yaw_16;
						rt_tar.st_data.vel_x = 0;//speed_x_axis_16;
						rt_tar.st_data.vel_y = 0;//;
						rt_tar.st_data.vel_z = 0;

						dt.fun[0x41].WTS = 1; //将要发送rt_tar数据。
					}
				}
				else
				{
					time_dly_cnt_ms = 0;
					mission_step += 1;
					first_a0_voice=1;
				}					
				}
				break;
				case 6:
				{
				//一键起飞		
					  mission_step += OneKey_Takeoff(100);
					  time_dly_cnt_ms = 0;
			
					if(first_a0_voice==1)
					{
						sprintf(buf1,"[s9]一键起飞");//\[b1] 欢迎光临，请进
						speak_len1=strlen(buf1);
						speak_context((u8*)buf1,speak_len1);
						
						LxStringSend(1,"一键起飞");
				}
			 }
				break;
				case 7:
				{
					//等2秒
					if(time_dly_cnt_ms<2000)
					{
						time_dly_cnt_ms+=20;//ms
						
						
						if(time_dly_cnt_ms%100==0)
						{

							speed_x_axis_16=10;
							speed_y_axis_16=0;
							

							
							speed_z_axis=p_altitude*(altitude_set-alt_add_xm);//PID高度控制
							speed_z_axis_16=(int16_t) speed_z_axis;
							
							rt_tar.st_data.rol = 0;
							rt_tar.st_data.pit = 0;
							rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
							//这里会把实时XYZ-YAW期望速度置零		
							rt_tar.st_data.yaw_dps =0;
							rt_tar.st_data.vel_x =speed_x_axis_16;
							rt_tar.st_data.vel_y =speed_y_axis_16;
							rt_tar.st_data.vel_z = speed_z_axis_16;

							dt.fun[0x41].WTS = 1; //将要发送rt_tar数据。
						}
					}
					else
					{
							sprintf(buf1,"[s9]离开起飞点");//\[b1] 欢迎光临，请进
							speak_len1=strlen(buf1);
							speak_context((u8*)buf1,speak_len1);
							LxStringSend(1,"离开起飞点");
						
							time_dly_cnt_ms = 0;
							mission_step += 1;
							first_a0_voice=1;
											
					}					
				}
				break;
		    case 8:
			{
				//等2秒
				
					time_dly_cnt_ms_xm+=20;//ms
					if(time_dly_cnt_ms_xm>=10000)
					{
						time_dly_cnt_ms_xm=0;
					}
				
					
					uart3_camera_command_analyse();
					
					
					result_target=find_target(sow_piont[position_1_order].x,sow_piont[position_1_order].y,altitude_set);
					if(1==result_target)
					{
						time_dly_cnt_ms = 0;
						mission_step += 1;
						first_a0_voice=1;
						time_dly_cnt_ms_xm=0;
					}
					
//					if (((plant_x>=(sow_piont[position_1_order].x-0.15f))&&(plant_x<=(sow_piont[position_1_order].x+0.15f)))&&((plant_y>(sow_piont[position_1_order].y-0.15f))&&(plant_y<=(sow_piont[position_1_order].y+0.15f))))
//					{
//						sow_seed();
//						index_sow_i=0;
//						
//						sprintf(buf1,"[s9]到达第一个区域");//\[b1] 欢迎光临，请进
//						speak_len1=strlen(buf1);
//						speak_context((u8*)buf1,speak_len1);
//						
//						
//						LxStringSend(1,"到达第二个区域，准备下降高度");
//						
//						time_dly_cnt_ms = 0;
//						mission_step += 1;
//						first_a0_voice=1;
//						time_dly_cnt_ms_xm=0;
//					 }
//					 else
//					 {
//						dis_sonic=sum_x_aa;		
//						speed_x_axis=p_longitude*(sow_piont[position_1_order].x-plant_x);//PIDX轴控制
//						speed_x_axis_16=(int16_t) speed_x_axis;
//						if(speed_x_axis_16>20)
//						{
//							speed_x_axis_16=15;
//						}
//						if(speed_x_axis_16<-20)
//						{
//							speed_x_axis_16=-15;
//						}
//						
//						if((speed_x_axis_16<5)&&(speed_x_axis_16>0))
//						{
//							speed_x_axis_16=5;
//						}
//						
//						if((speed_x_axis_16<0)&&(speed_x_axis_16>-5))
//						{
//							speed_x_axis_16=-5;
//						}
//						
//						speed_y_axis=p_longitude*(plant_y-sow_piont[position_1_order].y);//PIDY轴控制  act_y-plant_y
//						speed_y_axis_16=(int16_t) speed_y_axis;
//						if(speed_y_axis_16>20)
//						{
//							speed_y_axis_16=15;
//						}
//						if(speed_y_axis_16<-20)
//						{
//							speed_y_axis_16=-15;
//						}
//						
//						
//						if((speed_y_axis_16<5)&&(speed_y_axis_16>0))
//						{
//							speed_y_axis_16=5;
//						}
//						
//						if((speed_y_axis_16<0)&&(speed_y_axis_16>-5))
//						{
//							speed_y_axis_16=-5;
//						}
//				
//						speed_z_axis=p_altitude*(altitude_set-alt_add_xm);//PID高度控制
//						speed_z_axis_16=(int16_t) speed_z_axis;
//					
//						rt_tar.st_data.rol = 0;
//						rt_tar.st_data.pit = 0;
//						rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
//						//这里会把实时XYZ-YAW期望速度置零		
//						rt_tar.st_data.yaw_dps=0;// yaw_16;
//						rt_tar.st_data.vel_x =speed_x_axis_16;//speed_x_axis_16;
//						rt_tar.st_data.vel_y =speed_y_axis_16;//
//						rt_tar.st_data.vel_z =speed_z_axis_16;

//						dt.fun[0x41].WTS = 1; //将要发送rt_tar数据。
//						
//						if(time_dly_cnt_ms_xm%300==0)
//						{
//							if((speed_x_axis_16>0)&&(speed_y_axis_16>0))//右手定理
//							{
//								LxStringSend(1,"前进+1,向左+1");
//								sprintf(buf1,"[s9]前左");//\[b1] 欢迎光临，请进
//								speak_len1=strlen(buf1);
//								speak_context((u8*)buf1,speak_len1);
//							}
//							else if((speed_x_axis_16>0)&&(speed_y_axis_16<0))
//							{
//								LxStringSend(1,"前进+1,向右+1");
//								sprintf(buf1,"[s9]前右");//\[b1] 欢迎光临，请进
//								speak_len1=strlen(buf1);
//								speak_context((u8*)buf1,speak_len1);
//							}
//							else if((speed_x_axis_16<0)&&(speed_y_axis_16>0))
//							{
//								LxStringSend(1,"后退+1,向左+1");
//								sprintf(buf1,"[s9]后左");//\[b1] 欢迎光临，请进
//								speak_len1=strlen(buf1);
//								speak_context((u8*)buf1,speak_len1);
//							}
//							else if((speed_x_axis_16<0)&&(speed_y_axis_16<0))
//							{
//								LxStringSend(1,"后退+1,向右+1");
//								sprintf(buf1,"[s9]后右");//\[b1] 欢迎光临，请进
//								speak_len1=strlen(buf1);
//								speak_context((u8*)buf1,speak_len1);
//							}
//							else
//							{
//								
//							}

//						}
//								
//          }

				}
			
		   break;	
			
			case 9://直接降低高度
				{
						time_dly_cnt_ms_xm+=20;//ms		
						
						
						if(time_dly_cnt_ms_xm%100==0)
						{
							//uart2_openmv();
							//停止区
							
							if(fabs(altitude_set_1-alt_add_xm)>=10)
							{
									speed_x_axis_16=0;
									speed_y_axis_16=0;
									
									speed_z_axis=p_altitude*(altitude_set_1-alt_add_xm);//int16_t
								
									speed_z_axis_16=(int16_t) speed_z_axis;
									
									
									rt_tar.st_data.rol = 0;
									rt_tar.st_data.pit = 0;
									rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
									//这里会把实时XYZ-YAW期望速度置零		
									rt_tar.st_data.yaw_dps = 0;
									rt_tar.st_data.vel_x =speed_x_axis_16;
									rt_tar.st_data.vel_y =speed_y_axis_16;
									rt_tar.st_data.vel_z = speed_z_axis_16;

									dt.fun[0x41].WTS = 1; //将要发送rt_tar数据?
									if(altitude_set_1>alt_add_xm)
									{
										LxStringSend(1,"上升高度");
									}
									else
									{
										LxStringSend(1,"下降高度");
									}
							}
							else
							{
								LxStringSend(1,"到达指定高度，准备下放吊篮");
								
								time_dly_cnt_ms = 0;
								mission_step= mission_step+1;
								basic_step=0;
								time_dly_cnt_ms_xm=0;
								
								box_down_flag=0x00;
								box_count=0;
								
								
								
								speed_x_axis_16=0;
								speed_y_axis_16=0;
								
								speed_z_axis=p_altitude*(altitude_set_1-alt_add_xm);//int16_t
							
								speed_z_axis_16=(int16_t) speed_z_axis;
								
								
								rt_tar.st_data.rol = 0;
								rt_tar.st_data.pit = 0;
								rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
								//这里会把实时XYZ-YAW期望速度置零		
								rt_tar.st_data.yaw_dps = 0;
								rt_tar.st_data.vel_x =speed_x_axis_16;
								rt_tar.st_data.vel_y =speed_y_axis_16;
								rt_tar.st_data.vel_z = speed_z_axis_16;

								dt.fun[0x41].WTS = 1; //将要发送rt_tar数据?
								
								
								
								
							}
		
						
						}
						
					}
				
				
				break;	
				
				
				
				

		 
			case 10:   //收放吊篮
			{
				
				
				box_count++;
				time_dly_cnt_ms_xm++;
				if(box_count%1==0)//注意这里不是加20毫秒，而是加一
				{
					speed_x_axis_16=0;
					speed_y_axis_16=0;
					
					speed_z_axis=p_altitude*(altitude_set_1-alt_add_xm);//int16_t
				
					speed_z_axis_16=(int16_t) speed_z_axis;
					
					
					rt_tar.st_data.rol = 0;
					rt_tar.st_data.pit = 0;
					rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
					//这里会把实时XYZ-YAW期望速度置零		
					rt_tar.st_data.yaw_dps = 0;
					rt_tar.st_data.vel_x =speed_x_axis_16;
					rt_tar.st_data.vel_y =speed_y_axis_16;
					rt_tar.st_data.vel_z = speed_z_axis_16;
				
					
					dt.fun[0x41].WTS = 1; //将要发送rt_tar数据?	
					
					uart3_camera_command_analyse();
					
	
					result_target=find_target(sow_piont[position_1_order].x,sow_piont[position_1_order].y,altitude_set_1);
					if(1==result_target)
					{
//						time_dly_cnt_ms = 0;
//						//mission_step += 1;
//						first_a0_voice=1;
//						time_dly_cnt_ms_xm=0;
					}
					
				}
				
				if(box_down_flag==0x00)
				{
					box_down_flag=1;
					down_basket();
					LxStringSend(1,"下放吊篮，计时开始");
					
				}
				else
				{
						if(box_count>=290&&box_count<290+250)//5.秒下放吊篮时间，然后再悬停5秒
						{
							if(box_count==290)
							{
								stop_basket();
								
								LxStringSend(1,"停止下放吊篮，准备悬停5秒");
								
								sprintf(buf1,"[s9]停止下放吊篮，准备悬停5秒");//\[b1] 欢迎光临，请进
								speak_len1=strlen(buf1);
								speak_context((u8*)buf1,speak_len1);
								
							}
							else
							{
								stop_basket();
							}
							
						}
						else if(box_count>=290+250&&box_count<290+250+290)//5.8秒下放吊篮时间，然后再悬停5秒
						{
						
							up_basket();
							
							if(box_count==290+250)
							{
						
								LxStringSend(1,"悬停5秒时间到，准备上升吊篮");
								
								sprintf(buf1,"[s9]悬停5秒时间到，准备上升吊篮");//\[b1] 欢迎光临，请进
								speak_len1=strlen(buf1);
								speak_context((u8*)buf1,speak_len1);
							}
						
						
						
						}	
						else if(box_count>=290+250+290)//5.秒下放吊篮时间，然后再悬停5秒
						{
						
								stop_basket();
								LxStringSend(1,"上升5.8秒到，准备回复高度");
									
								sprintf(buf1,"[s9]上升5.8秒到，准备回复高度");//\[b1] 欢迎光临，请进
								speak_len1=strlen(buf1);
								speak_context((u8*)buf1,speak_len1);
								
								mission_step= mission_step+1;
						//灭灯
								GPIO_ResetBits(GPIOD,GPIO_Pin_15);//GPIOF9,F10设置高，灯灭
								light_flag=0;
						//
								box_count=0;
								
								time_dly_cnt_ms = 0;
								mission_step= mission_step+1;
								basic_step=0;
								time_dly_cnt_ms_xm=0;
								
								box_down_flag=0x00;
								box_count=0;
							}

					
				}
				
			}
			
		   break;
			
			
				case 11:
				{
						time_dly_cnt_ms_xm+=20;//ms		
						//回复1.5高度
						
						if(time_dly_cnt_ms_xm%100==0)
						{
							//uart2_openmv();
							//停止区
							
							if(fabs(altitude_set-alt_add_xm)>=10)
							{
									speed_x_axis_16=0;
									speed_y_axis_16=0;
									
									speed_z_axis=p_altitude*(altitude_set-alt_add_xm);//int16_t
								
									speed_z_axis_16=(int16_t) speed_z_axis;
									
									
									rt_tar.st_data.rol = 0;
									rt_tar.st_data.pit = 0;
									rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
									//这里会把实时XYZ-YAW期望速度置零		
									rt_tar.st_data.yaw_dps = 0;
									rt_tar.st_data.vel_x =speed_x_axis_16;
									rt_tar.st_data.vel_y =speed_y_axis_16;
									rt_tar.st_data.vel_z = speed_z_axis_16;

									dt.fun[0x41].WTS = 1; //将要发送rt_tar数据?
									LxStringSend(1,"上升高度");
							}
							else
							{
								LxStringSend(1,"到达1.5米高度，准备去下一个快递点");
								
								time_dly_cnt_ms = 0;
								mission_step= mission_step+1;
								basic_step=0;
								time_dly_cnt_ms_xm=0;
								
								box_down_flag=0x00;
								box_count=0;
								
								
								
								speed_x_axis_16=0;
								speed_y_axis_16=0;
								
								speed_z_axis=p_altitude*(altitude_set-alt_add_xm);//int16_t
							
								speed_z_axis_16=(int16_t) speed_z_axis;
								
								
								rt_tar.st_data.rol = 0;
								rt_tar.st_data.pit = 0;
								rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
								//这里会把实时XYZ-YAW期望速度置零		
								rt_tar.st_data.yaw_dps = 0;
								rt_tar.st_data.vel_x =speed_x_axis_16;
								rt_tar.st_data.vel_y =speed_y_axis_16;
								rt_tar.st_data.vel_z = speed_z_axis_16;

								dt.fun[0x41].WTS = 1; //将要发送rt_tar数据?
								
								
								
								
							}
		
						
						}
						
					}
				
				
				break;			
/////////////////////////////////////////////////////////////////////////////////////			
/////////////////////////////////////////////////////////////////////////////////////			
/////////////////////////////////////////////////////////////////////////////////////			
/////////////////////////////////////////////////////////////////////////////////////			
///////////              目前高度1.5米，执行下一个快递点                                  //////////////			
//////////////////////////case 从12开始////////////////////////////////////////			
/////////////////////////////////////////////////////////////////////////////////////			
/////////////////////////////////////////////////////////////////////////////////////			
/////////////////////////////////////////////////////////////////////////////////////			
/////////////////////////////////////////////////////////////////////////////////////			
/////////////////////////////////////////////////////////////////////////////////////			
		    case 12:
			{
				//等2秒
				
					time_dly_cnt_ms_xm+=20;//ms
					if(time_dly_cnt_ms_xm>=10000)
					{
						time_dly_cnt_ms_xm=0;
					}
				
					
					uart3_camera_command_analyse();
					
					
					//find_target(sow_piont[position_2_order].x,sow_piont[position_2_order].y,altitude_set);
					
					result_target=find_target(sow_piont[position_2_order].x,sow_piont[position_2_order].y,altitude_set);
					if(1==result_target)
					{
						time_dly_cnt_ms = 0;
						mission_step += 1;
						first_a0_voice=1;
						time_dly_cnt_ms_xm=0;
					}
					
					
					
					
//					if (((plant_x>=(sow_piont[position_2_order].x-0.15f))&&(plant_x<=(sow_piont[position_2_order].x+0.15f)))&&((plant_y>(sow_piont[position_2_order].y-0.15f))&&(plant_y<=(sow_piont[position_2_order].y+0.15f))))
//					{
//						sow_seed();
//						index_sow_i=0;
//						
//						sprintf(buf1,"[s9]到达第二个区域");//\[b1] 欢迎光临，请进
//						speak_len1=strlen(buf1);
//						speak_context((u8*)buf1,speak_len1);
//						
//						
//						LxStringSend(1,"到达第二个区域，准备下降高度");
//						
//						time_dly_cnt_ms = 0;
//						mission_step += 1;
//						first_a0_voice=1;
//						time_dly_cnt_ms_xm=0;
//					 }
//					 else
//					 {
//						dis_sonic=sum_x_aa;		
//						speed_x_axis=p_longitude*(sow_piont[position_2_order].x-plant_x);//PIDX轴控制
//						speed_x_axis_16=(int16_t) speed_x_axis;
//						if(speed_x_axis_16>20)
//						{
//							speed_x_axis_16=15;
//						}
//						if(speed_x_axis_16<-20)
//						{
//							speed_x_axis_16=-15;
//						}
//						
//						if((speed_x_axis_16<5)&&(speed_x_axis_16>0))
//						{
//							speed_x_axis_16=5;
//						}
//						
//						if((speed_x_axis_16<0)&&(speed_x_axis_16>-5))
//						{
//							speed_x_axis_16=-5;
//						}
//						
//						speed_y_axis=p_longitude*(plant_y-sow_piont[position_2_order].y);//PIDY轴控制  act_y-plant_y
//						speed_y_axis_16=(int16_t) speed_y_axis;
//						if(speed_y_axis_16>20)
//						{
//							speed_y_axis_16=15;
//						}
//						if(speed_y_axis_16<-20)
//						{
//							speed_y_axis_16=-15;
//						}
//						
//						
//						if((speed_y_axis_16<5)&&(speed_y_axis_16>0))
//						{
//							speed_y_axis_16=5;
//						}
//						
//						if((speed_y_axis_16<0)&&(speed_y_axis_16>-5))
//						{
//							speed_y_axis_16=-5;
//						}
//				
//						speed_z_axis=p_altitude*(altitude_set-alt_add_xm);//PID高度控制
//						speed_z_axis_16=(int16_t) speed_z_axis;
//					
//						rt_tar.st_data.rol = 0;
//						rt_tar.st_data.pit = 0;
//						rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
//						//这里会把实时XYZ-YAW期望速度置零		
//						rt_tar.st_data.yaw_dps=0;// yaw_16;
//						rt_tar.st_data.vel_x =speed_x_axis_16;//speed_x_axis_16;
//						rt_tar.st_data.vel_y =speed_y_axis_16;//
//						rt_tar.st_data.vel_z =speed_z_axis_16;

//						dt.fun[0x41].WTS = 1; //将要发送rt_tar数据。
//						
//						if(time_dly_cnt_ms_xm%300==0)
//						{
//							if((speed_x_axis_16>0)&&(speed_y_axis_16>0))//右手定理
//							{
//								LxStringSend(1,"前进+1,向左+1");
//								sprintf(buf1,"[s9]前左");//\[b1] 欢迎光临，请进
//								speak_len1=strlen(buf1);
//								speak_context((u8*)buf1,speak_len1);
//							}
//							else if((speed_x_axis_16>0)&&(speed_y_axis_16<0))
//							{
//								LxStringSend(1,"前进+1,向右+1");
//								sprintf(buf1,"[s9]前右");//\[b1] 欢迎光临，请进
//								speak_len1=strlen(buf1);
//								speak_context((u8*)buf1,speak_len1);
//							}
//							else if((speed_x_axis_16<0)&&(speed_y_axis_16>0))
//							{
//								LxStringSend(1,"后退+1,向左+1");
//								sprintf(buf1,"[s9]后左");//\[b1] 欢迎光临，请进
//								speak_len1=strlen(buf1);
//								speak_context((u8*)buf1,speak_len1);
//							}
//							else if((speed_x_axis_16<0)&&(speed_y_axis_16<0))
//							{
//								LxStringSend(1,"后退+1,向右+1");
//								sprintf(buf1,"[s9]后右");//\[b1] 欢迎光临，请进
//								speak_len1=strlen(buf1);
//								speak_context((u8*)buf1,speak_len1);
//							}
//							else
//							{
//								
//							}

//						}
//								
//          }

				}
			
		   break;	
			
			case 13:
				{
						time_dly_cnt_ms_xm+=20;//ms		
						//直接降低高度
						
						if(time_dly_cnt_ms_xm%100==0)
						{
							//uart2_openmv();
							//停止区
							
							if(fabs(altitude_set_1-alt_add_xm)>=10)
							{
									speed_x_axis_16=0;
									speed_y_axis_16=0;
									
									speed_z_axis=p_altitude*(altitude_set_1-alt_add_xm);//int16_t
								
									speed_z_axis_16=(int16_t) speed_z_axis;
									
									
									rt_tar.st_data.rol = 0;
									rt_tar.st_data.pit = 0;
									rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
									//这里会把实时XYZ-YAW期望速度置零		
									rt_tar.st_data.yaw_dps = 0;
									rt_tar.st_data.vel_x =speed_x_axis_16;
									rt_tar.st_data.vel_y =speed_y_axis_16;
									rt_tar.st_data.vel_z = speed_z_axis_16;

									dt.fun[0x41].WTS = 1; //将要发送rt_tar数据?
									if(altitude_set_1>alt_add_xm)
									{
										LxStringSend(1,"上升高度");
									}
									else
									{
										LxStringSend(1,"下降高度");
									}
									
							}
							else
							{
								LxStringSend(1,"到达指定高度，准备下放吊篮");
								
								time_dly_cnt_ms = 0;
								mission_step= mission_step+1;
								basic_step=0;
								time_dly_cnt_ms_xm=0;
								
								box_down_flag=0x00;
								box_count=0;
								
								
								
								speed_x_axis_16=0;
								speed_y_axis_16=0;
								
								speed_z_axis=p_altitude*(altitude_set_1-alt_add_xm);//int16_t
							
								speed_z_axis_16=(int16_t) speed_z_axis;
								
								
								rt_tar.st_data.rol = 0;
								rt_tar.st_data.pit = 0;
								rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
								//这里会把实时XYZ-YAW期望速度置零		
								rt_tar.st_data.yaw_dps = 0;
								rt_tar.st_data.vel_x =speed_x_axis_16;
								rt_tar.st_data.vel_y =speed_y_axis_16;
								rt_tar.st_data.vel_z = speed_z_axis_16;

								dt.fun[0x41].WTS = 1; //将要发送rt_tar数据?
								
								
								
								
							}
		
						
						}
						
					}
				
				
				break;	
				
				
				
				

		 
			case 14:   //收放吊篮
			{
//				
//				
//				box_count++;
//				if(box_count%5==0)//注意这里不是加20毫秒，而是加一
//				{
//					speed_x_axis_16=0;
//					speed_y_axis_16=0;
//					
//					speed_z_axis=p_altitude*(altitude_set_1-alt_add_xm);//int16_t
//				
//					speed_z_axis_16=(int16_t) speed_z_axis;
//					
//					
//					rt_tar.st_data.rol = 0;
//					rt_tar.st_data.pit = 0;
//					rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
//					//这里会把实时XYZ-YAW期望速度置零		
//					rt_tar.st_data.yaw_dps = 0;
//					rt_tar.st_data.vel_x =speed_x_axis_16;
//					rt_tar.st_data.vel_y =speed_y_axis_16;
//					rt_tar.st_data.vel_z = speed_z_axis_16;
//				
//					
//					dt.fun[0x41].WTS = 1; //将要发送rt_tar数据?	
//					
//					
//					uart3_camera_command_analyse();
//					
//					
//					//find_target(sow_piont[position_2_order].x,sow_piont[position_2_order].y,altitude_set_1);
//					
//					result_target=find_target(sow_piont[position_2_order].x,sow_piont[position_2_order].y,altitude_set_1);
//					if(1==result_target)
//					{
////						time_dly_cnt_ms = 0;
////						mission_step += 1;
////						first_a0_voice=1;
////						time_dly_cnt_ms_xm=0;
//					}
//					
//				}
//				
//				if(box_down_flag==0x00)
//				{
//					box_down_flag=1;
//					steering_pd12=1;
//					steering_pd13=1;
//					LxStringSend(1,"下放吊篮，计时开始");
//					
//				}
//				else
//				{
//						if(box_count>=290&&box_count<290+250)//5.秒下放吊篮时间，然后再悬停5秒
//						{
//							if(box_count==290)
//							{
//								steering_pd12=0;
//								steering_pd13=1;
//								
//								LxStringSend(1,"停止下放吊篮，准备悬停5秒");
//								
//								sprintf(buf1,"[s9]停止下放吊篮，准备悬停5秒");//\[b1] 欢迎光临，请进
//								speak_len1=strlen(buf1);
//								speak_context((u8*)buf1,speak_len1);
//								
//							}
//							else
//							{
//								steering_pd12=0;
//								steering_pd13=1;
//							}
//							
//						}
//						else if(box_count>=290+250&&box_count<290+250+290)//5.8秒下放吊篮时间，然后再悬停5秒
//						{
//						
//							steering_pd12=0;
//							steering_pd13=0;
//							
//							if(box_count==290+250)
//							{
//						
//								LxStringSend(1,"悬停5秒时间到，准备上升吊篮");
//								
//								sprintf(buf1,"[s9]悬停5秒时间到，准备上升吊篮");//\[b1] 欢迎光临，请进
//								speak_len1=strlen(buf1);
//								speak_context((u8*)buf1,speak_len1);
//							}
//						
//						
//						
//					}
//					else if(box_count>=290+250+290)//5.秒下放吊篮时间，然后再悬停5秒
//					{
//						
//								steering_pd12=1;
//								steering_pd13=0;
//						LxStringSend(1,"上升5.8秒到，准备回复高度");
//							
//							sprintf(buf1,"[s9]上升5.8秒到，准备回复高度");//\[b1] 欢迎光临，请进
//							speak_len1=strlen(buf1);
//							speak_context((u8*)buf1,speak_len1);
//						
//						mission_step= mission_step+1;
//						
//						//灭灯
//						GPIO_ResetBits(GPIOD,GPIO_Pin_15);//GPIOF9,F10设置高，灯灭
//						light_flag=0;
//						//
//						box_count=0;
//						
//						time_dly_cnt_ms = 0;
//						mission_step= mission_step+1;
//						basic_step=0;
//						time_dly_cnt_ms_xm=0;
//						
//						box_down_flag=0x00;
//						box_count=0;
//					}

//					
//				}
//				
//			}


		
				
				
				box_count++;
				time_dly_cnt_ms_xm++;
				if(box_count%1==0)//注意这里不是加20毫秒，而是加一
				{
					speed_x_axis_16=0;
					speed_y_axis_16=0;
					
					speed_z_axis=p_altitude*(altitude_set_1-alt_add_xm);//int16_t
				
					speed_z_axis_16=(int16_t) speed_z_axis;
					
					
					rt_tar.st_data.rol = 0;
					rt_tar.st_data.pit = 0;
					rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
					//这里会把实时XYZ-YAW期望速度置零		
					rt_tar.st_data.yaw_dps = 0;
					rt_tar.st_data.vel_x =speed_x_axis_16;
					rt_tar.st_data.vel_y =speed_y_axis_16;
					rt_tar.st_data.vel_z = speed_z_axis_16;
				
					
					dt.fun[0x41].WTS = 1; //将要发送rt_tar数据?	
					
					uart3_camera_command_analyse();
					
	
					result_target=find_target(sow_piont[position_2_order].x,sow_piont[position_2_order].y,altitude_set_1);
					if(1==result_target)
					{
//						time_dly_cnt_ms = 0;
//						//mission_step += 1;
//						first_a0_voice=1;
//						time_dly_cnt_ms_xm=0;
					}
					
				}
				
				if(box_down_flag==0x00)
				{
					box_down_flag=1;
					down_basket();
					LxStringSend(1,"下放吊篮，计时开始");
					
				}
				else
				{
						if(box_count>=290&&box_count<290+250)//5.秒下放吊篮时间，然后再悬停5秒
						{
							if(box_count==290)
							{
								stop_basket();
								
								LxStringSend(1,"停止下放吊篮，准备悬停5秒");
								
								sprintf(buf1,"[s9]停止下放吊篮，准备悬停5秒");//\[b1] 欢迎光临，请进
								speak_len1=strlen(buf1);
								speak_context((u8*)buf1,speak_len1);
								
							}
							else
							{
								stop_basket();
							}
							
						}
						else if(box_count>=290+250&&box_count<290+250+290)//5.8秒下放吊篮时间，然后再悬停5秒
						{
						
							up_basket();
							
							if(box_count==290+250)
							{
						
								LxStringSend(1,"悬停5秒时间到，准备上升吊篮");
								
								sprintf(buf1,"[s9]悬停5秒时间到，准备上升吊篮");//\[b1] 欢迎光临，请进
								speak_len1=strlen(buf1);
								speak_context((u8*)buf1,speak_len1);
							}
						
						
						
						}	
						else if(box_count>=290+250+290)//5.秒下放吊篮时间，然后再悬停5秒
						{
						
								stop_basket();
								LxStringSend(1,"上升5.8秒到，准备回复高度");
									
								sprintf(buf1,"[s9]上升5.8秒到，准备回复高度");//\[b1] 欢迎光临，请进
								speak_len1=strlen(buf1);
								speak_context((u8*)buf1,speak_len1);
								
								mission_step= mission_step+1;
						//灭灯
								GPIO_ResetBits(GPIOD,GPIO_Pin_15);//GPIOF9,F10设置高，灯灭
								light_flag=0;
						//
								box_count=0;
								
								time_dly_cnt_ms = 0;
								mission_step= mission_step+1;
								basic_step=0;
								time_dly_cnt_ms_xm=0;
								
								box_down_flag=0x00;
								box_count=0;
							}

					
				}
			}
			
			
		   break;
			
			
				case 15://回复1.5高度
				{
						time_dly_cnt_ms_xm+=20;//ms		
						
						
						if(time_dly_cnt_ms_xm%100==0)
						{
							//uart2_openmv();
							//停止区
							
							if(fabs(altitude_set-alt_add_xm)>=10)
							{
									speed_x_axis_16=0;
									speed_y_axis_16=0;
									
									speed_z_axis=p_altitude*(altitude_set-alt_add_xm);//int16_t
								
									speed_z_axis_16=(int16_t) speed_z_axis;
									
									
									rt_tar.st_data.rol = 0;
									rt_tar.st_data.pit = 0;
									rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
									//这里会把实时XYZ-YAW期望速度置零		
									rt_tar.st_data.yaw_dps = 0;
									rt_tar.st_data.vel_x =speed_x_axis_16;
									rt_tar.st_data.vel_y =speed_y_axis_16;
									rt_tar.st_data.vel_z = speed_z_axis_16;

									dt.fun[0x41].WTS = 1; //将要发送rt_tar数据?
									if(altitude_set_1>alt_add_xm)
									{
										LxStringSend(1,"上升高度");
									}
									else
									{
										LxStringSend(1,"下降高度");
									}
							}
							else
							{
								LxStringSend(1,"到达1.5米高度，准备回家降落");
								
								time_dly_cnt_ms = 0;
								mission_step= mission_step+1;

								//	灭灯
								GPIO_ResetBits(GPIOD,GPIO_Pin_15);//GPIOF9,F10设置高，灯灭
								light_flag=0;
								basic_step=0;
								time_dly_cnt_ms_xm=0;
								
								box_down_flag=0x00;
								box_count=0;
								
								
								
								speed_x_axis_16=0;
								speed_y_axis_16=0;
								
								speed_z_axis=p_altitude*(altitude_set-alt_add_xm);//int16_t
							
								speed_z_axis_16=(int16_t) speed_z_axis;
								
								
								rt_tar.st_data.rol = 0;
								rt_tar.st_data.pit = 0;
								rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
								//这里会把实时XYZ-YAW期望速度置零		
								rt_tar.st_data.yaw_dps = 0;
								rt_tar.st_data.vel_x =speed_x_axis_16;
								rt_tar.st_data.vel_y =speed_y_axis_16;
								rt_tar.st_data.vel_z = speed_z_axis_16;

								dt.fun[0x41].WTS = 1; //将要发送rt_tar数据?
								
								
								
								
							}
		
						
						}
						
					}
				
				
				break;						
			
			
			
			
			
			
			
			
	/////////////////////////////////////////////////////////////////////////////////////			
/////////////////////////////////////////////////////////////////////////////////////			
/////////////////////////////////////////////////////////////////////////////////////			
/////////////////////////////////////////////////////////////////////////////////////			
///////////              目前高度1.5米，准备回家降落                                  //////////////			
//////////////////////////case 从17开始////////////////////////////////////////			
		    case 16:
			{
				//等2秒
				
					time_dly_cnt_ms_xm+=20;//ms
					if(time_dly_cnt_ms_xm>=10000)
					{
						time_dly_cnt_ms_xm=0;
					}
				
					
					uart3_camera_command_analyse();
					
					if ((plant_x>=home_x-0.15f)&&(plant_x<=home_x+0.15f)&&(plant_y>home_y-0.15f)&&(plant_y<=home_y+0.15f))
					{
						sow_seed();
						index_sow_i=0;
						
						sprintf(buf1,"[s9]到达起降点，准备z轴0速");//\[b1] 欢迎光临，请进
						speak_len1=strlen(buf1);
						speak_context((u8*)buf1,speak_len1);
						
						
						LxStringSend(1,"到达起降点，准备z轴0速");
						
						time_dly_cnt_ms = 0;
						mission_step += 1;
						first_a0_voice=1;
						time_dly_cnt_ms_xm=0;
					 }
					 else
					 {
						dis_sonic=sum_x_aa;		
						speed_x_axis=p_longitude*(home_x-plant_x);//PIDX轴控制
						speed_x_axis_16=(int16_t) speed_x_axis;
						if(speed_x_axis_16>20)
						{
							speed_x_axis_16=15;
						}
						if(speed_x_axis_16<-20)
						{
							speed_x_axis_16=-15;
						}
						
						if((speed_x_axis_16<5)&&(speed_x_axis_16>0))
						{
							speed_x_axis_16=5;
						}
						
						if((speed_x_axis_16<0)&&(speed_x_axis_16>-5))
						{
							speed_x_axis_16=-5;
						}
						
						speed_y_axis=p_longitude*(plant_y-home_y);//PIDY轴控制  act_y-plant_y
						speed_y_axis_16=(int16_t) speed_y_axis;
						if(speed_y_axis_16>20)
						{
							speed_y_axis_16=15;
						}
						if(speed_y_axis_16<-20)
						{
							speed_y_axis_16=-15;
						}
						
						
						if((speed_y_axis_16<5)&&(speed_y_axis_16>0))
						{
							speed_y_axis_16=5;
						}
						
						if((speed_y_axis_16<0)&&(speed_y_axis_16>-5))
						{
							speed_y_axis_16=-5;
						}
				
						speed_z_axis=p_altitude*(altitude_set-alt_add_xm);//PID高度控制
						speed_z_axis_16=(int16_t) speed_z_axis;
					
						rt_tar.st_data.rol = 0;
						rt_tar.st_data.pit = 0;
						rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
						//这里会把实时XYZ-YAW期望速度置零		
						rt_tar.st_data.yaw_dps=0;// yaw_16;
						rt_tar.st_data.vel_x =speed_x_axis_16;//speed_x_axis_16;
						rt_tar.st_data.vel_y =speed_y_axis_16;//
						rt_tar.st_data.vel_z =speed_z_axis_16;

						dt.fun[0x41].WTS = 1; //将要发送rt_tar数据。
						
						if(time_dly_cnt_ms_xm%300==0)
						{
							if((speed_x_axis_16>0)&&(speed_y_axis_16>0))//右手定理
							{
								LxStringSend(1,"前进+1,向左+1");
							}
							else if((speed_x_axis_16>0)&&(speed_y_axis_16<0))
							{
								LxStringSend(1,"前进+1,向右+1");
							}
							else if((speed_x_axis_16<0)&&(speed_y_axis_16>0))
							{
								LxStringSend(1,"后退+1,向左+1");
							}
							else if((speed_x_axis_16<0)&&(speed_y_axis_16<0))
							{
								LxStringSend(1,"后退+1,向右+1");
							}
							else
							{
								
							}

						}
								
					}			

				}
			
		   break;	
			
			case 17://z轴0速
				{
						time_dly_cnt_ms_xm+=20;//ms		
						
						
						if(time_dly_cnt_ms_xm%100==0)
						{
							//uart2_openmv();
							//停止区

							speed_x_axis_16=0;
							speed_y_axis_16=0;
							
							speed_z_axis=0;//int16_t
						
							speed_z_axis_16=(int16_t) speed_z_axis;
							
							
							rt_tar.st_data.rol = 0;
							rt_tar.st_data.pit = 0;
							rt_tar.st_data.thr = 0; //用于模式0，避免模式0时失控，油门过大飞跑，给一个稍低于中位的油门
							//这里会把实时XYZ-YAW期望速度置零		
							rt_tar.st_data.yaw_dps = 0;
							rt_tar.st_data.vel_x =speed_x_axis_16;
							rt_tar.st_data.vel_y =speed_y_axis_16;
							rt_tar.st_data.vel_z = speed_z_axis_16;

							dt.fun[0x41].WTS = 1; //将要发送rt_tar数据?

						
						}


						if(time_dly_cnt_ms_xm>=1000)
						{
							LxStringSend(1,"z轴0速,准备一键降落");
						
							time_dly_cnt_ms = 0;
							mission_step += 1;
							first_a0_voice=1;
							time_dly_cnt_ms_xm=0;


						}


						
				}
				
				
				break;	
				
				
				
				


					
	case 18://恢复1.5米高度
		{
			//执行一键降落
			OneKey_Land();
			
			time_dly_cnt_ms = 0;
			mission_step += 1;
			
			if(first_a0_voice==1)
			{
				sprintf(buf1,"[s9]降落");//\[b1] 欢迎光临，请进
				speak_len1=strlen(buf1);
				speak_context((u8*)buf1,speak_len1);
				
				LxStringSend(1,"降落");
				
				first_a0_voice=0;
			}
			
		}
		break;			
	case 19:
		{
			//执行一键降落
			
				time_dly_cnt_ms+=20;//ms
				first_a0_voice=0;
			
			
		}
		break;
		case 20:
		{
			time_dly_cnt_ms+=20;//ms
		}
		break;				
		default:
			break;
	}
	}
	}
	else
	{
		mission_step = 0;
	}
		
}
    ////////////////////////////////////////////////////////////////////////



