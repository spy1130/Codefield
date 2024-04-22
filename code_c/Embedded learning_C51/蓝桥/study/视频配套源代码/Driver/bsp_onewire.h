# include "STC15F2K60S2.H"

sbit DQ = P1^4;  //单总线接口


//函数名：读取温度函数
//入口参数：无
//返回值：温度高低两个字节
//函数功能：完成温度转换，并返回转换之后的温度数据;
unsigned int rd_temperature(void);  



