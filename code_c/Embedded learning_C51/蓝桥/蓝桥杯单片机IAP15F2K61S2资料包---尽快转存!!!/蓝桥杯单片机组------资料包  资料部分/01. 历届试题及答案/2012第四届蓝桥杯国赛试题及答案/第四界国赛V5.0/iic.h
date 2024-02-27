#ifndef _IIC_H
#define _IIC_H
#define uchar unsigned char

//º¯ÊýÉùÃ÷
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void);
void iicwrite(uchar add,uchar dat);
uchar iicread(uchar add); 

#endif