#include "dac.h"
extern u16 nums;
void DAC_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
//	DAC_DeInit();//��ʼ��
	DAC_Cmd(DAC_Channel_1, ENABLE);//DACͨ��ʹ��
	
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //������� 
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 ; //ѡ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//����Ϊģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_SetBits(GPIOA,GPIO_Pin_4);   //�������
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//��ʱ��
     
  /**************DAC�ṹ��ʼ��*******************/  
	DAC_InitTypeDef DAC_InitStructure;
	DAC_StructInit(&DAC_InitStructure);//�ԼĴ�������Ѱַ��ʼ��
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//����������
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; //��ʹ���������
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T7_TRGO;//DAC����Ϊ��ʱ��7����,���Ҳ�ʹ���������
	DAC_DMACmd(DAC_Channel_1, ENABLE);//DAC��dmaʹ��
	
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);//��ʼ��
    DAC_Cmd(DAC_Channel_1, ENABLE);    //ʹ��DAC��ͨ��1
    DAC_DMACmd(DAC_Channel_1, ENABLE); //ʹ��DACͨ��1��DMA  
	DAC_SetChannel1Data(DAC_Align_12b_R,0);//����dac���ģʽ
}
void dac_dma_wave_enable( uint16_t psc,uint16_t arr)
{
	TIM_Cmd(TIM7,DISABLE);//ʹ��ֹͣ������ʹ������θ��ȶ�
	 
	DMA_Config();

	TIM_Config(psc,arr);//ʹ�ܸ��¶�ʱ��
	
	TIM_Cmd(TIM7,ENABLE);//ʹ�ܶ�ʱ��
}

