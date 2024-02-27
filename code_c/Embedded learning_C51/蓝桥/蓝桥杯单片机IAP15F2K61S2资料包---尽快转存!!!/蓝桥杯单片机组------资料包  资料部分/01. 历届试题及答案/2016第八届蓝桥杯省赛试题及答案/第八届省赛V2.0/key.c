sbit key1 = P3^0;
sbit key2 = P3^1;
sbit key3 = P3^2;
sbit key4 = P3^3;
#define uchar unsigned char
#define uint unsigned int 
unsigned char mode;
unsigned char hour_n=0,min_n=0,sec_n=0;
uchar hour=23,min=59,sec=50,flash;
uchar hour_i,min_i,sec_i;
void delay(unsigned int x)
{
	while(x--);
}
void key_func()
{
	if(key1 == 0)
	{
		delay(400);
		if(key1 == 0)
		{
			while(key1 == 0);
			hour_i = hour;
			min_i = min;
			sec_i = sec;
			mode++;
			if(mode>=4) mode=0;
		}
	}
	if(key3 == 0)
	{
		delay(400);
		if(key3 == 0)
		{
			while(key3 == 0);
			switch(mode)
			{
				case 1: hour_i++;if(hour_i>=24) hour_i=0;hour = hour_i;break;
				case 2: min_i++;if(min_i>=60) min_i=0;min = min_i;break;
				case 3: sec_i++;if(sec_i>=60) sec_i=0;sec = sec_i;break;
				case 4: hour_n++;if(hour_n>=24) hour_n=0;break;
				case 5: min_n++;if(min_n>=60) min_n=0;break;
				case 6: sec_n++;if(sec_n>=60) sec_n=0;break;
			}
		}
	}
	if(key4 == 0)
	{
		delay(400);
		if(key4 == 0)
		{	
			if( mode == 0) mode	= 7;
			while(key4 == 0);
			if(mode == 7) mode =0;
			switch(mode)
			{
				case 1: hour_i--;if(hour_i<=0) hour_i=23;hour = hour_i;break;
				case 2: min_i--;if(min_i<=0) min_i=59;min = min_i;break;
				case 3: sec_i--;if(sec_i<=0) sec_i=59;sec = sec_i;break;
				case 4: hour_n--;if(hour_n<=0) hour_n=23;break;
				case 5: min_n--;if(min_n<=0) min_n=59;break;
				case 6: sec_n--;if(sec_n<=0) sec_n=59;break;
			}
		}
	}
	if(key2 == 0)
	{
		delay(400);
		if(key2 == 0)
		{
			while(key2 == 0);
			if(mode>=4) mode++;
			if(mode<4) mode = 4;
			
			if(mode>=7) mode=0;

		
		}
	}

}