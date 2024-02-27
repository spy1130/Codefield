#ifndef _IIC_H
#define _IIC_H

//º¯ÊıÉùÃ÷
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 
extern unsigned char read_adc(unsigned char addr);

unsigned char write_date(unsigned char addr,unsigned char dat);
unsigned char read_date(unsigned char addr);

#endif