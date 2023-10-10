#include "stm32f10x.h"                  // Device header
#include "Delay.h"
//对操作端口的库函数进行封装
void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);//BitAction是一个枚举类型，表示这个位是高电平还是低电平

	Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return BitValue;
}
//软件I2C只需配置GPIO的读写函数即可
//把scl和sda都初始化为开漏输出模式,此模式仍可以输入,输入时.先输入l，再直接输入数据寄存器即可。
//把scl和SDA置为高电平，此时I2C总线处于空闲状态

void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}
//起始条件
void MyI2C_Start(void)
{	//确保I2C总线释放
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	//先拉低SDA，再拉低SCL
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}
//终止条件
void MyI2C_Stop(void)
{	//先拉低SDA,再释放SCL.SDA
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)//发送一个字节8位
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));//高位先行依次向下赋值，Byte&0x80相当于把Byte的最高位取出来
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);//每放完一位，执行释放SCL，拉低SCL操作
	}
}
//接收一个字节8位
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;//初始为0
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)//进来后SCL为低电平，主机释放SDA，从机把数据放到SDA，此时主机释放SCL，用R_SDA读取
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}//判断条件为这个数据不为0
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)//发一位
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)//读一位
	//从机应答为0，不应答为1
{	//函数进来时，SCL低电平，主机释放SDA，防止从机干扰，
	//同时从机把应答位放在SDA上，SCL高电平，主机读取应答位，SCL低电平，进入下一个时序单元
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
