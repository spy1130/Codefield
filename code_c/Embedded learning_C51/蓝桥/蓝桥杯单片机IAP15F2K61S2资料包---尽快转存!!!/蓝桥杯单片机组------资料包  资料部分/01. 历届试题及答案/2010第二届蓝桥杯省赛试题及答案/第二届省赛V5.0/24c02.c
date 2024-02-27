#include <stc15f2k60s2.h>
#include <intrins.h>
sbit sda=P2^1;
sbit scl=P2^0;
void Snop()
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}
void delay(unsigned char i)
{
	unsigned char j;
	do{
		for(j=0;j<112;j++);
	}while(i--);
}
void at24c02_start()
{
	sda=1;
	scl=1;
	Snop();
    sda=0;
	Snop();
	scl=0;	
}
void at24c02_stop()
{	
	sda=0;
	scl=1;
	Snop();
	sda=1;
	Snop();
	scl=0;
}
void at24c02_ack()
{
	scl=0;
	sda=1;
	scl=1;
	Snop();
	if(sda)
	at24c02_stop();
	scl=0;			
}
void at24c02_nack()
{
	scl=0;
	sda=1;
	scl=1;
	Snop();
	sda=1;
	Snop();
	scl=0;					
}
void writebyte(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		scl=0;
		if(dat&0x80)
		sda=1;
		else
		sda=0;
		scl=1;
		dat<<=1;
		Snop();	
	}	
}
unsigned char readbyte()
{
	unsigned char i,dat;
	for(i=0;i<8;i++)
	{
		scl=0;
		Snop();
		dat<<=1;
		scl=1;
		if(sda)
		dat|=0x01;
		else
		 dat&=0xfe;	
	}
	return dat;	
}
void at24c02_wirte(unsigned char add,unsigned char dat)
{
	at24c02_start();
	writebyte(0xa0);
	at24c02_ack();
	writebyte(add);
	at24c02_ack();
	writebyte(dat);
	at24c02_ack();
	at24c02_stop();
	delay(30);
				
}
unsigned char at24c02_read(unsigned char add)
{
	unsigned char dat;
	at24c02_start();
	writebyte(0xa0);
	at24c02_ack();
	writebyte(add);
	at24c02_ack();
	at24c02_start();
	writebyte(0xa1);
	at24c02_ack();
	dat=readbyte();
	at24c02_nack();
	at24c02_stop();
	return dat;			
}