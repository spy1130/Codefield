#include "STC15F2K60S2.H"
#include "intrins.h"


#define DELAY_TIME	5
#define Photo_Res_Channel 0x41
#define Adj_Res_Channel 0x43

sbit sda = P2^1;
sbit scl = P2^0;


void I2CStart(void);
void I2CStop(void);
void I2CSendByte(unsigned char byt);
unsigned char I2CReceiveByte(void);
unsigned char I2CWaitAck(void);
void I2CSendAck(unsigned char ackbit);



unsigned char Pcf8591_Adc(unsigned char channel_num_contrl);
void Pcf8591_Dac(unsigned char trans_dat)	;
void EEPROM_Write(unsigned char* EEPROM_String, unsigned char addr, unsigned char num);
void EEPROM_Read(unsigned char* EEPROM_String, unsigned char addr, unsigned char num);

