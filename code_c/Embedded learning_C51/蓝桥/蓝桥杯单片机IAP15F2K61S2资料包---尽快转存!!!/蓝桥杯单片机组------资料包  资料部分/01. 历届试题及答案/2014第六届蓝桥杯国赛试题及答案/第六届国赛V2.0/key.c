#include <key.h>

uchar s4,s5,s6=0,s7,ss=0;
uchar normal,time,kongzai,guozai;
uchar noise_flag=1,he_flag=1;

void keyscan()
{
	if(P33==0)
	{
		Delay5ms();
		if(P33==0)
		{
			if(normal==1)
			 {s4=1;s6=0;ss=0;}
		}
		while(!P33);	
	}
	else if(P32==0)
	{
		Delay5ms();
		if(P32==0)
		{
			if((time>0)&&(s4==1))
			{
				if(s5==0) {s5=1;fengming(0x00);}
				else s5=0;
			}
		}
		while(!P32);	
	}
	else if(P31==0)
	{
		Delay5ms();
		if(P31==0)
		{
			if(kongzai==1)
			{
				s6=1;s4=0;
				if(ss<=3)
				ss++;
//				if(ss<2)	//循环
//				ss++;
//				else ss=1;
			}
		}
		while(!P31);	
	}
	else if(P30==0)
	{
		Delay5ms();
		if(P30==0)
		{ if((kongzai==1)&&(s6==1)) 
			s7=1;
		}
		while(!P30);	
	}  
}
void fengming(uchar xiang)	 //适用于蜂鸣器和继电器不要求同时出现的情况 若要求同时出现 P0口的赋值需要重新考虑
{
	if((xiang==1)&&(noise_flag==1))
	{
		P2=P2&0x1f;
		P2=0xa0;
		P0=0x40;
		P2=P2&0x1f;
		noise_flag=0;
	}
	else if((xiang==0)&&(noise_flag==0))
	{
		P2=P2&0x1f;
		P2=0xa0;
		P0=0x00;
		P2=P2&0x1f;
		noise_flag=1;		
	}	
}
void jidian(uchar he)
{
	if((he==1)&&(he_flag==1))
	{
		P2=P2&0x1f;
		P2=0xa0;
		P0=0x10;
		P2=P2&0x1f;
		he_flag=0;
	}
	else if((he==0)&&(he_flag==0))
	{
		P2=P2&0x1f;
		P2=0xa0;
		P0=0x00;
		P2=P2&0x1f;
		he_flag=1;		
	}	
}