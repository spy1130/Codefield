#include "dac.h"
extern u16 nums;
void DAC_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
//	DAC_DeInit();//初始化
	DAC_Cmd(DAC_Channel_1, ENABLE);//DAC通道使能
	
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //输出速率 
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 ; //选择引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//配置为模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_SetBits(GPIOA,GPIO_Pin_4);   //拉高输出
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//开时钟
     
  /**************DAC结构初始化*******************/  
	DAC_InitTypeDef DAC_InitStructure;
	DAC_StructInit(&DAC_InitStructure);//对寄存器进行寻址初始化
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//不产生波形
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; //不使能输出缓存
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T7_TRGO;//DAC触发为定时器7触发,并且不使用软件触发
	DAC_DMACmd(DAC_Channel_1, ENABLE);//DAC的dma使能
	
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);//初始化
    DAC_Cmd(DAC_Channel_1, ENABLE);    //使能DAC的通道1
    DAC_DMACmd(DAC_Channel_1, ENABLE); //使能DAC通道1的DMA  
	DAC_SetChannel1Data(DAC_Align_12b_R,0);//设置dac输出模式
}
void dac_dma_wave_enable( uint16_t psc,uint16_t arr)
{
	TIM_Cmd(TIM7,DISABLE);//使能停止触发，使输出波形更稳定
	 
	DMA_Config();

	TIM_Config(psc,arr);//使能更新定时器
	
	TIM_Cmd(TIM7,ENABLE);//使能定时器
}

