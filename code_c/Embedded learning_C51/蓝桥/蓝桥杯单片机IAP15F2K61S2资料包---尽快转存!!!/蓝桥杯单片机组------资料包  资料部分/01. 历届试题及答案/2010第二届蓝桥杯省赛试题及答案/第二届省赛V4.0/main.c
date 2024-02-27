#include <stc15f2k60s2.h>
#include <24c02.h>
#include <ds18b20.h>

#define uint unsigned int
#define uchar unsigned char

uchar keysta[4]={1,1,1,1},DQTH,DQTL,temp;
bit flag;

uchar code smgchar[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar smgbuff[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

void smg_ds18b20(unsigned char temp,unsigned char THC,unsigned char TLC)
{
	smgbuff[0]=smgchar[temp%10];
	smgbuff[1]=smgchar[temp/10%10];
	smgbuff[2]=smgchar[11];
	smgbuff[3]=smgchar[11];
	smgbuff[4]=smgchar[TLC%10];
	smgbuff[5]=smgchar[TLC/10%10];
	smgbuff[6]=smgchar[THC%10];
	smgbuff[7]=smgchar[THC/10%10];
}

void Time_T0(unsigned char ms)
{
	unsigned int temp;
	TMOD&=0xf0;
	TMOD|=0x00;
	temp=11059200/12/1000;
	temp=65536-temp*ms;
	TH0=temp/256;
	TL0=temp%256;
	EA=1;
	ET0=1;
	TR0=1;
}

void smg_buff()
{
	static unsigned char i,cnt=0x80;
	P0=0x00;
	P2&=0x1f;
	P2|=0xc0;
	P2&=0x1f;
	P0=smgbuff[i];
	P2|=0xe0;
	P2&=0x1f;
	P0=cnt>>i;
	P2|=0xc0;
	P2&=0x1f;
	i++;
	if(i==8)
	i=0;	
}

void warningDS1302()
{
	static bit relay,pwmkey;
	if(DQTH<temp&&pwmkey==0)
	{
		pwmkey=1;
		TR1=1;	
	}
	if(DQTH>=temp&&pwmkey==1)
	{
		pwmkey=0;
		TR1=0;
		P34=1;
	}
	if(DQTL<=temp)
	{
		if(relay)
		{
			relay=0;
			P0=0x00;
			P2&=0x1f;
			P2|=0xa0;
			P2&=0x1f;	
		}
	}
	if(DQTL>temp)
	{
		if(relay==0)
		{
			relay=1;
			P0=0x00;
			P2&=0x1f;
			P2|=0xa0;
			P04=1;
			P2&=0x1f;
		}		
	}
}
void key_4()
{
	static unsigned char keyback[4]={1,1,1,1},i;
	for(i=0;i<4;i++)
	{
		if(keyback[i]!=keysta[i])
		{
			if(keyback[i]==0)
			{
				switch(i)
				{
					case 0:DQTL--;if(DQTL>100)DQTL=0;at24c02_wirte(0x01,DQTL);break;
					case 1:DQTH--;if(DQTH>100)DQTH=0;if(DQTL>DQTH)DQTH=DQTL;at24c02_wirte(0x00,DQTH);break;
					case 2:DQTL++;if(DQTL>=100)DQTL=99;if(DQTL>DQTH)DQTL=DQTH;at24c02_wirte(0x01,DQTL);break;
					case 3:DQTH++;if(DQTH>=100)DQTH=99;at24c02_wirte(0x00,DQTH);break;
					default:break;
				}
			}
			keyback[i]=keysta[i];
		}
	}	
}


void scanf_key()
{
	static unsigned char i,keybuf[4]={0xff,0xff,0xff,0xff};
	keybuf[0]=(keybuf[0]<<1)|P30;
	keybuf[1]=(keybuf[1]<<1)|P31;
	keybuf[2]=(keybuf[2]<<1)|P32;
	keybuf[3]=(keybuf[3]<<1)|P33;
	for(i=0;i<4;i++)
	{
	 	if(keybuf[i]==0x00)
			keysta[i]=0;
	 	if(keybuf[i]==0xff)
			keysta[i]=1;
	}
}
void Timer1Init(void)		//100΢��@11.0592MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xAE;		//���ö�ʱ��ֵ
	TH1 = 0xFB;		//���ö�ʱ��ֵ
	ET1=1;
}

void allinit()
{
	P0=0xff;
	P2&=0x1f;
	P2|=0x80;
	P2&=0x1f;
	P0=0x00;
	P2&=0x1f;
	P2|=0xa0;
	P2&=0x1f;
}
void main()
{
	allinit();
	DQTH=at24c02_read(0x00);
	DQTL=at24c02_read(0x01);
	Time_T0(1);
	Timer1Init();
	while(1)
	{
		if(flag)
		{
			flag=0;
			temp=read_ds18b20();
		}
		key_4();
		smg_ds18b20(temp,DQTH,DQTL);
		warningDS1302();
	}
}


void TimeT0interrupt() interrupt 1
{
	static unsigned int i;
	i++;
	if(i==1000)
	{
		flag=1;
		i=0;
	}
	scanf_key();
	if(i%2)
	smg_buff();
}
void TimeT1interrupt() interrupt 3
{
	static unsigned char i;
	if(i<3&&P34==0)
	P34=1;
	else if(i>3&&i<10&&P34==1)
	P34=0;
	i++;
	if(i==10)
	i=0;
}
