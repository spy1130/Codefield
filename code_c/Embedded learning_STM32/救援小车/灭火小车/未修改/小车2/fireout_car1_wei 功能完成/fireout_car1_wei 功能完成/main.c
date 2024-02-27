#include "ultrasonic.h"
#include "action.h"
#include "motor.h"
#include "key.h"



/**********************定义某些管脚********************/
bit candle_fire_1=0x01;//  1 is fire on ; 0 is extinguish
bit candle_fire_9=0x01;//  1 is fire on ; 0 is extinguish
bit candle_fire_3=0x00;////  1 is fire on ; 0 is extinguish
bit candle_fire_2=0x01;////  1 is fire on ; 0 is extinguish


sbit Fan = P0^0;
sbit LeftFire = P0^2;
sbit RightFire = P0^3;

#define Fan_On {Fan = 0;}
#define Fan_Off {Fan = 1;}



unsigned char prevent_time = 0;
unsigned int left_300 = 0;
unsigned int right_900 = 0;
unsigned int pwm_num = 0;
unsigned int room_2_pwm_num = 0;
unsigned int left_time=0;


/******************主函数***************
**************实现功能移动灭火**********/
void main()
{
	LED = 1;//亮灯指示
	Timer0Init();//左轮PWM
	Timer1Init();//右轮PWM
	Ultrasonic_Init();
	while(1)
	{ 
		keyscan();//按键检测
		if(start==1)
		{
			/**********************测试右转180°********************/
//				turn_right_180();
//				car_stop();	
//			
//				while(1);//for(;;);
			front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
			front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
			front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
			front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
			right_dis= Ultrasonic_Get_Right();//得到一次右超声波测距模块的距离
			left_dis= Ultrasonic_Get_Left();//得到一次左超声波测距模块的距离
			right_dis= Ultrasonic_Get_Right();
			left_dis= Ultrasonic_Get_Left();
			right_dis= Ultrasonic_Get_Right();
			left_dis= Ultrasonic_Get_Left();
			
			
			goto aaa;//跳转aaa
		//////////////////
aaa:
			if(candle_fire_1==0x01)
			{
				do								   
				{	
					Circulation_wall_right();								
				}
				while(front_dis > 182);
				//now position is fire 1,and next step find fire 1			
				car_stop();	
				
							
				////find_fire_turn_right();
				//now position is fire 1,and next step put off  fire 1		
				car_stop();
				
				do
				{
					Fan_On;     //-----------------灭1---------------------
					delay_ms(1500);
					break;
				}
				while(LeftFire==0||RightFire==0);
				
				//害怕死灰复燃，停2秒，再次检测，
				do
				{
					Fan_On;
					delay_ms(1500);
					break;
				}
				while(LeftFire==0||RightFire==0);
							
				//害怕死灰复燃，停0.5秒，再次检测，保证吹灭
				
				Fan_Off;
				//////加的
				delay_ms(150);
					//now position is fire 1,and next turn back				
							
//							find_fire_turn_back_left();
							
					//now position is fire 1,and next turn left 				
				car_stop();
				
				if(candle_fire_9==0x00&&candle_fire_3==0x00&&candle_fire_2==0x00)
				{
					turn_right_180();
					goto only_fire1_is_on_back_home;//灭1返航
				}
				turn_left_90();//
				car_stop();
				
			}
			else//----------------------1无火-----------------------
			{
				do								   //一号房间
				{
					Circulation_wall_right();	
					//walkalone_left_1();								
				}
				while(front_dis > 182);//灭一号火的距离
	//now position is fire 1,and next step find fire 1			
				car_stop();
				turn_left_90();//
			}
			/////////////////
//
//从1#着火点出发，准备寻找1#房间的大门中央，企图进入1号房间大门。
//为了防止误判，检测大门，检测两次，原因是又一次发现测不准还在1#着火点，却误认为自己在1#号房间门口
////////////////////
				//now position is fire 1,and next walk alone withe the right wall	
				 front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);//卡尔曼滤波
				 left_dis= Ultrasonic_Get_Left();
				 right_dis= Ultrasonic_Get_Right();
				 if(candle_fire_9==0x01)
				 {
					  i=0;
						do								   //一号房间
						{
							Circulation_wall_right();
						}
						while(i<=30);
						//为了防止误判，发现一号房间门口 执行两次。因为调试过程中在火焰1处应为前方超声测距错误，测成514毫米，实际应为1200毫米
						//，导致满足while(front_dis > 550||left_dis <600)条件，是的单片机本在火焰1误认为在一号房间门口，所以写两次
						//希望前方超声测距不要猜测错了。
								
						delay_ms(100);
							
						left_dis= Ultrasonic_Get_Left();//左侧检查
				    right_dis= Ultrasonic_Get_Right();//右侧检查
						
						do								   //进入一号房间前——寻找右墙
						{
							Circulation_wall_right();
							left_dis= Ultrasonic_Get_Left();
						}
						while(front_dis > 580||left_dis <600);  //front_dis=660
						
						car_stop();
					  //now position is middle gate of room 1,and next action turn left 
						delay_ms(50);

						//					
						/////////////////
						//
						//在1#房间大门中央。准备进入大门，并巡右墙 在离墙177毫米处停下来
						////////////////////						
								
						turn_left_90();				   //进入房间1
								
						//now position is middle gate of room 1,and next action pass through gate and walk alone with right wall         ///////////////	
						do
						{
							Circulation_wall_right();			
						}
						while(front_dis > 230);//改为20-5.19，23-5.20
						delay_ms(50);//-------------------
					//now position is corner 1 of room 1,and next action turn left
						car_stop();
						turn_left_90();							//左转
		//					
		/////////////////
		//
		//在1#房间1号拐角，准备前往9#着火点。并灭火
		////////////////////						
								
						//now position is corner 1 of room 1,and next action wanlk alone with right wall
						do
						{
									Circulation_wall_right();	//寻右墙		
						}
						while(front_dis > 210);//原数据185改160-5.18，19-5.20,21-5.25
						//now position is corner 2 of room 1,and next action find fire 2	
						car_stop();
						
						//////begin find fire/////////////////////////
						//
						//car_stop();
//								find_fire_turn_right();
						
				//now position is corner 2 of room 1,and next action put off fire 2			
								
						do
						{
							Fan_On;     //-----------------灭9火---------
							delay_ms(1500);
							break;
						}
						while(LeftFire==0||RightFire==0);
						
						//害怕死灰复燃，停2秒，再次检测，
						do
						{
							Fan_On;
							delay_ms(1500);
							break;
						}
						while(LeftFire==0||RightFire==0);
						
								
						//害怕死灰复燃，停0.5秒，再次检测，保证吹灭
						while(LeftFire==0||RightFire==0)
						{
							Fan_On;
							delay_ms(1000);
							break;
						}
								
						Fan_Off;
						//////加的
					  delay_ms(100);
						//now position is corner 2 of room 1,and next action turn back	
//								find_fire_turn_back_left();
								
								
						//now position is corner 2 of room 1,and next action turn 180 degree 	
						car_stop();
						turn_right_180();//右转有180°不足——已解决
						
						
						/******************************有误的地方*******************************/
						/*******************************已经解决********************************/
		/////////////////
		//
		//在9#着火点，准备离开着火点，巡左墙。在离墙160毫米处停下来
		////////////////////							
								///////////////////////////////
						//now position is corner 2 of room 1,and next action walk alone with left wall	
							//-----------------防误判-----------	
						i=0;
						do
						{
							Circulation_wall_left();	//寻左墙
						}
						while(left_dis < 390 && i < 24);//i原数据为12改为-24，5-18
						
						
					//----------------------------------	
						
						do
						{
							Circulation_wall_left();	//寻左墙		
						}
						while(front_dis > 230);//18cm，230-5.20
					// turn right at the cornner 1	
						car_stop();
						turn_right_90();
						
		//////////////////
			
//开机出发，巡右墙，在离墙180毫米处停下
//并在3#街道停下来，离墙180毫米停。
// now potion is cornre 3 of  room 2,next action  walk alone right wall
		///////////////////
						/////////////////
		//
		//在1#房间，拐角1，准备离开一号房间，巡左墙走，在离墙160毫米处停下来
		////////////////////	19年改190						

								
							/// now postion is room 1 corner 1 and walk alone left wall  and next action stop at the street 
						do
						{
							Circulation_wall_left();//寻左墙			
						}
						while(front_dis > 190);//驶出到1号房间出口1#街区处前墙19cm
						
					// now potion is street and nearby is the gate of room1 ,turn left 90 degree.	
						
						 car_stop();
								
						if(candle_fire_3==0x00&&candle_fire_2==0x00)//蜡烛2和蜡烛3灭
						{
							turn_right_90();//-----------------回家
							do
							{
								Circulation_wall_left();	//寻左墙		
							}
							while(front_dis > 200);//前方20cm处
							
							
							/********************************右转回家家******************************/
							turn_right_90();
							goto only_fire_9_fire_1_is_on_back_home;//灭蜡烛1和蜡烛9后回家
						}
						
						
						
						/******************出去1#后*********************/
						turn_left_90();
		/////////////////
		//
		//在1#房间门口，且处在2#街道上，并离墙160毫米。准备巡右墙走到2#大街尽头，
		////////////////////							
								///////////////////////////////						
								
							// now potion is street and nearby is the gate of room1 walk alone right.and next action stop at the end of street		
						do
						{
							Circulation_wall_right();			
						}
						while(front_dis > 180);

							// now potion is end of street 2 ,next action is turn left 90		
						car_stop();
						
						turn_left_90();//左转90°进去一号房间和二号房间的中间过道
					}
						
					else if(candle_fire_3==0x01||candle_fire_2==0x01)//蜡烛2蜡烛3燃烧
					{
						do
						{
							Circulation_wall_right();	//寻右墙		
						}
							while(front_dis > 200);//距离二号房间前方17.7cm处左转90°，200-5.20
						// now potion is end of street 2 ,next action is turn left 90		
						car_stop();
						turn_left_90();
					}
						/***********************没搞懂5-18**********************/
					else
					{
						turn_left_90();//这里回家
						goto only_fire1_is_on_back_home;
					}
			

//
//在2#街道的尽端，准备前往2#房间。先巡右墙走，发现左墙，立刻巡左墙，一直走到2#房间大门中央
////////////////////						
					// now potion is end of street2,next action iswalk alone right wall		
					//这一段程序较为麻烦。造成麻烦的原因是处于拐角时，测右墙数据处于混沌状态，应该为172毫米，可是处于直角边缘时
						//数据变大，甚至能变成220,250，乃至300多，而且这个混沌区域长度还不小，有大约50毫米。
						
/////////////////
//
//先巡右墙走，一直搜索左墙，直到稳定的发现左墙，
////////////////////
					
					
					
					
						left_300=0x00;
						do
						{
							Circulation_wall_right();//先寻右墙
							left_dis= Ultrasonic_Get_Left();
							
							if(left_dis<300)
							{
								left_300++;
							}
							else
							{
								left_300=0x00;
							}
						}
						while(left_dis>300||left_300<=3);//front_dis > 580||left_dis <600
						//car_stop();
						//car_stop();
						
/////////////						
//这时小车3#街道左墙首端，下面开始寻找左墙走，右边有稳定的大于800的数据，这就是到了2号房间的门边缘
///////////////
					
						right_900=0;
						do
						{
							Circulation_wall_right();
							
							if(right_dis>800)
							{
								right_900++;
								if(right_900>=0x03)
								{
									car_stop();
									break;
								}
							}
							else
							{
								right_900=0x00;
							}
						}while(right_dis<800||right_dis<=3);
						
						
						car_stop();
/***************************************此处有问题*****************************/
						/************************无法进入2#房间***************************/
						/**************************问题已解决*****************************/
						//while(front_dis > 1400||right_dis <600);//front_dis > 580||left_dis <600
					// now potion is begin of door of room 2,next action is walk forward 20 cm	
//						car_stop();
						
/////////////						
//这时小车2号房间大门口，但不是大门中央，准备前往大门中央
///////////////

						i=0x00;
						
						do
						{
							Circulation_wall_right();//寻右墙
							
							}while(i<=42);	//50改为44
						
					// now potion is middle of door of room 2,next action isturn right 90	
						car_stop();
						turn_right_90();//右转90°
		
					// now potion is middle of door of room 2,next action isturn walk alone left 2	
/////////////						
//这时小车2号房间大门口中央，准备进入2#房间，并在2号房间3#着火点处停下来，离墙180毫米停下来
///////////////
						
/////////////			///////////////////			
//修改路径2019-10-2///////////////
////////////////////////////
													
						do	
						{
							Circulation_wall_left();//寻左墙保持18cm
						}while(front_dis >210);//180,210-5.20

						car_stop();	
							
						if(candle_fire_2==0x01)//蜡烛2燃烧
						{
							turn_right_90();//右转90°
							car_stop();
							/*******************四次卡尔曼滤波是干什么？*********************/
							/*********************此处也有问题！******************/
							/******************问题已解决改变转弯延迟**********************/
							front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
							front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
							front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
							front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
							do
							{
								Circulation_wall_left();
								}while(front_dis >190);//前距离20cm，190-5.20
						
							car_stop();
							turn_right_90();
							car_stop();
/////////////						
//这时小车处于2#房间3#拐角，准备巡左墙，并在离墙180毫米处停下来
///////////////
							
							do
							{
								Circulation_wall_left();
							}while(front_dis >185);
				
							car_stop();
							delay_ms(100);
							
							turn_left_90_big();
							car_stop();//--------------2火-----------
							
							do
							{
								Fan_On;
								delay_ms(1500);
								break;
							}
							while(LeftFire==0||RightFire==0);
							
							//害怕死灰复燃，停2秒，再次检测，
							do
							{
								Fan_On;
								delay_ms(1500);
								break;
							}
							while(LeftFire==0||RightFire==0);
							
						
							Fan_Off;
							//turn back 
	//						find_fire_turn_back_right();
	//						car_stop();
							delay_ms(100);
						
							turn_left_90_big();
							car_stop();

						
                //-----------------防误判-----2回------	
							prevent_time=0;
							do
							{
								Circulation_wall_right();
								prevent_time++;
							}
							while(right_dis < 400&& prevent_time<5);
							prevent_time=0;
					//----------------------------------	
										
					
							do
							{
								Circulation_wall_right();
							}while(front_dis >190);//170,190-5.20
						// now potion is cornre 3 of room 2,next action  turn left
							car_stop();
							turn_left_90();
							car_stop();
						
							if(candle_fire_3==0x01)
							{
								do
								{
									Circulation_wall_right();
								}while(front_dis >170);//之前是210,170-5.20
									car_stop();
								 do
								{
									Fan_On;//------------------3火---
									delay_ms(2000);
									break;
								}
								while(LeftFire==0||RightFire==0);
								
								//害怕死灰复燃，停2秒，再次检测，
								do
								{
									Fan_On;
									delay_ms(1000);
									break;
								}
								while(LeftFire==0||RightFire==0);
								delay_ms(1);
								
								//害怕死灰复燃，停0.5秒，再次检测，保证吹灭
								while(LeftFire==0||RightFire==0)
								{
									Fan_On;
									delay_ms(1000);
									delay_ms(1000);
									break;
								}
							
								Fan_Off;
								//////加的
								delay_ms(150);
								//turn back 
	//							find_fire_turn_back_right();
								turn_left_90();
								car_stop();
								goto only_fire_1_fire_9_fire_3_is_on_back_home;
							}
							
							else //---------------无3--
							{
								do
								{
									Circulation_wall_right();
								}while(front_dis >180);
								car_stop();
								
								turn_left_90();
								
								goto only_fire_1_fire_9_fire_3_is_on_back_home;
							}
					}

				 else //------------------无2---
				 {
						do
						{
							Circulation_wall_left();
						}while(front_dis >170);

						car_stop();
						turn_left_90();
						do
						{
							Fan_On;//---------------灭3火
							delay_ms(1500);
							break;
						}
						while(LeftFire==0||RightFire==0);
						
						//害怕死灰复燃，停2秒，再次检测，
						do
						{
							Fan_On;
							delay_ms(1);
							break;
						}
						while(LeftFire==0||RightFire==0);
						delay_ms(1);
						
						//害怕死灰复燃，停0.5秒，再次检测，保证吹灭
						while(LeftFire==0||RightFire==0)
						{
							Fan_On;
							delay_ms(1000);
							delay_ms(1000);
							break;
						}
						
						Fan_Off;
						//////加的
						delay_ms(100);
						//turn back 
//							find_fire_turn_back_right();
//							car_stop();
					}
					turn_left_90();
					car_stop();
						
					goto only_fire_1_fire_9_fire_3_is_on_back_home;

						
						
						
//---------------------准备回头-------------------						
						
	



						

/////////////////
//
//在着火点3位置，准备驶出2#房间，并在3#街道停下来，离墙180毫米停。
////////////////////						
					// now potion is cornre 3 of  room 2,next action  walk alone righr wall
only_fire_1_fire_9_fire_3_is_on_back_home:						
						//-----------------防误判-----------
//								prevent_time=0;						
//								do
//								{
//									Circulation_wall_right();
//									prevent_time++;
//								}
//								while(right_dis < 200&&prevent_time<5);//驶出房间2走向3号街区右侧距离39cm——该为20cm-5.18
//								prevent_time=0;
//							//----------------------------------	
						
						
						do
						{
							Circulation_wall_right();
						}while(front_dis >410);
					// now potion is cornre 4 of room 2,next action  turn left
						car_stop();
						front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
						front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
						front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
						front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
						do
						{
													    
//						 forward();
							Circulation_wall_right();
						 
						}while(front_dis >180);
						car_stop();
						turn_left_90();
						car_stop();
					// now potion is  door of room 2,and meanwhile at the street 3,next action   walk alone righr wall
/////////////						
//这时小车处于3#大街，巡右墙走，并扫描左墙，稳定发现左墙
///////////////

						left_300=0x00;
						do
						{
							Circulation_wall_right();
							left_dis= Ultrasonic_Get_Left();
							
							if(left_dis<300)
							{
								left_300++;
							}
							else
							{
								left_300=0x00;
							}
						}
						while(left_dis>300||left_300<=3);//front_dis > 580||left_dis <600
//						car_stop();
//						car_stop();
						
						
						
/////////////						
//这时小车处于3#大街，发现左墙，准备巡左墙走，并在前墙170毫米处停下
///////////////						
	
						do
							
						{
							Circulation_wall_left();
						}while(front_dis >180);
						car_stop();
						turn_right_90();
/////////////						
//这时小车处于2#大街尽头，准备巡左墙走，并在前墙170毫米处停下
///////////////							
						do
							
						{
							Circulation_wall_left();
						}while(front_dis >210);//原数据180
						car_stop();
						turn_right_back_90(); 
						
/////////////						
//这时小车处于1#着火点，准备巡左墙走，并在前墙580毫米处停下，并掉头，为下次启动做准备
///////////////	
only_fire_9_fire_1_is_on_back_home:						
						car_stop();
						
only_fire1_is_on_back_home:	
						
//						i=0;
//						do
//						{
//							Circulation_wall_left();
//						}while(i<=30);
						
						
						front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
						front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
						front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
						front_dis= kalmanfilter(Ultrasonic_Get_Front(),1);
						
						do
						{
							Circulation_wall_left();
						}while(front_dis >580);
						//car_stop();
						//car_stop();	

						do
							
						{
							Circulation_wall_left();
						}while(front_dis >500);
//						car_stop();								

						do
							
						{
							Circulation_wall_left();
						}while(front_dis >430);//-------------10.15
						do
							
						{
							Circulation_wall_left();
						}while(front_dis >430);//-------------10.15
						do
							
						{
							Circulation_wall_left();
						}while(front_dis >430);//-------------10.15
						do
							
						{
							Circulation_wall_left();
						}while(front_dis >430);//-------------10.15
						car_stop();
						car_stop();	
						delay_ms(100);
						car_stop();
						while(1);
						
		}
  }
}
			