#include "SystemClock.h"



//系统时钟设置步骤(通俗语言)———————————————————————————————————

void RCC_HSE_Config(u32 div,u32 pllm) //自定义系统时间（可以修改时钟）

{
	
RCC_DeInit(); //RCC设置为复位状态
RCC_HSEConfig(RCC_HSE_ON);//开启外部高速晶振（HSE）
	
if(RCC_WaitForHSEStartUp()==SUCCESS) //等待HSE起振稳定，由该函数[ RCC_WaitForHSEStartUp() ]的两个返回值判定：1.返回"SUCCESS"代表成功稳定起振。2.返回"ERROR"则是错误，失败。

	{
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHB时钟(HCLK)配置为[1分频](即72MHz)	
	RCC_PCLK1Config(RCC_HCLK_Div2);//低速APB(PCLK1)配置为[2分频](即36MHz)	
	RCC_PCLK2Config(RCC_HCLK_Div1);//高速AHB(PCLK2)配置为[1分频](即72MHz)

	
	RCC_PLLConfig(div,pllm);//PLL锁相环配置——(HSE分频系数，PLL倍频系数)

	
	RCC_PLLCmd(ENABLE); //PLL使能
	
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);//PLL是否就绪
//，若已设置就绪，则返回"1"，那么就不等于RESET，则退出while并进行下一步。反之，死循环至就绪。	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//选择系统时钟(SYSCLK)的时钟来源[PLLCLK]
	
	while(RCC_GetSYSCLKSource()!=0x08);//判断 返回时钟源的值：当PLL为系统时钟来源时，函数[ RCC_GetSYSCLKSource() ]会返回"0x08"，则"0x08 = 0x08"，

	}			             //所以函数不成立，则会退出循环并继续下一步，否则若返回值不是(0x08)那就代表不正确，则死循环在这里。

}



