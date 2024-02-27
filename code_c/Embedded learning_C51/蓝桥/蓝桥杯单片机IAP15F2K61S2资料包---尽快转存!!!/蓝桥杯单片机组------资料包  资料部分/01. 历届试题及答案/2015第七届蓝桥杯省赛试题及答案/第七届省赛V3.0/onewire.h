#ifndef __DEPUTY_H_
#define __DEPUTY_H_


#define uchar unsigned char 
#define uint unsigned int	
	



void allinit();


void Delay_OneWire(unsigned int t);
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);
bit init_ds18b20(void);
uchar temget(void);

	

#endif