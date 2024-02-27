#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"
//下载时，按下复位键不放，在唤醒上一个睡眠模式代码的一秒期间，马上点击下载，并放开复位键，低功耗期间不能下载
//停止模式，时钟关闭，中断不需要时钟
int main(void)
{
	/*模块初始化*/
	OLED_Init();			//OLED初始化
	CountSensor_Init();		//计数传感器初始化
	
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);		//开启PWR的时钟
															//停止模式和待机模式一定要记得开启
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "Count:");
	
	while (1)
	{
		OLED_ShowNum(1, 7, CountSensor_Get(), 5);			//OLED不断刷新显示CountSensor_Get的返回值
		
		OLED_ShowString(2, 1, "Running");					//OLED闪烁Running，指示当前主循环正在运行
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);	//STM32进入停止模式，并等待中断唤醒
		SystemInit();	//唤醒后，要重新配置时钟，重新启用72m主频，不然复位后会回到HSI（8M）主频
	}
}
//有时候遮挡数值没加是因为模块信号有些抖动，在中断函数里有一个if判断，过滤掉了这个计数信号，部分批次的模块和芯片确实会有信号抖动这个问题