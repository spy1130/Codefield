/*	# 	单总线代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/
# include "onewire.h"


void Delay_OneWire(unsigned int t)  
{
	unsigned char i;/*晶振是12Mhz所以一个时钟周期为0.083333us*/
	while(t--){
		for(i=0;i<12;i++); //3us
	}
}

//
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;//DQ从1到0写时序开始
		DQ = dat&0x01;//进行时序的传输，
		Delay_OneWire(5);//写一位的间隔15us
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);//表示一个字节写完了
}

//
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;//释放
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);//表示一个字节写完了
	}
	return dat;
}

//参考初始化程序
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;//单片机操作DQ先拉高
  	Delay_OneWire(12);//等待一会
  	DQ = 0;//单片机拉低总线
  	Delay_OneWire(80);//等待480us发送一个复位脉冲
  	DQ = 1;//单片机释放总线
  	Delay_OneWire(10); //总线由上拉电阻拉高到高电平，等待15到60us
    initflag = DQ;    //18b20操作总线发送存在脉冲，initflag表示初始化成功的标志位，成功为1
  	Delay_OneWire(5);//18b20将总线拉高有上拉电阻产生一个存在脉冲
  	return initflag;//初始化成功为0
}
unsigned int Read_Tm()
{
	unsigned char high;
	unsigned char low;
	init_ds18b20();
	Write_DS18B20(0xcc);//忽略rom直接读取
	Write_DS18B20(0x44);//温度转换完之后返回到1（初始化），所以18b20必须再次进行初始化
	
	init_ds18b20();
	Write_DS18B20(0xcc);//跳过rom
	Write_DS18B20(0xbe);//给一个读取rom的指令，共返回9个字节其中只有LSB，LSB能够用到是温度的低位，高位。
	low=Read_DS18B20();//从字节0开始,先读低位，在读高位。
	high=Read_DS18B20();
	
	return (high<<8)+low;
}