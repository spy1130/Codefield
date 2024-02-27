#ifndef _IIC_H
#define _IIC_H
//º¯ÊýÉùÃ÷
void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 

unsigned char PCF8591_Adc(void);
void PCF8591_Dac(unsigned char dat);

void EEPROM_Read(unsigned char* pucBuf, unsigned char addr, unsigned char num);
void EEPROM_Write(unsigned char* pucBuf, unsigned char addr, unsigned char num);
#endif
