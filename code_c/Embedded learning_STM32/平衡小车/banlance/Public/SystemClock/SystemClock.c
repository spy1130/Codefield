#include "SystemClock.h"



//ϵͳʱ�����ò���(ͨ������)����������������������������������������������������������������������

void RCC_HSE_Config(u32 div,u32 pllm) //�Զ���ϵͳʱ�䣨�����޸�ʱ�ӣ�

{
	
RCC_DeInit(); //RCC����Ϊ��λ״̬
RCC_HSEConfig(RCC_HSE_ON);//�����ⲿ���پ���HSE��
	
if(RCC_WaitForHSEStartUp()==SUCCESS) //�ȴ�HSE�����ȶ����ɸú���[ RCC_WaitForHSEStartUp() ]����������ֵ�ж���1.����"SUCCESS"����ɹ��ȶ�����2.����"ERROR"���Ǵ���ʧ�ܡ�

	{
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHBʱ��(HCLK)����Ϊ[1��Ƶ](��72MHz)	
	RCC_PCLK1Config(RCC_HCLK_Div2);//����APB(PCLK1)����Ϊ[2��Ƶ](��36MHz)	
	RCC_PCLK2Config(RCC_HCLK_Div1);//����AHB(PCLK2)����Ϊ[1��Ƶ](��72MHz)

	
	RCC_PLLConfig(div,pllm);//PLL���໷���á���(HSE��Ƶϵ����PLL��Ƶϵ��)

	
	RCC_PLLCmd(ENABLE); //PLLʹ��
	
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);//PLL�Ƿ����
//���������þ������򷵻�"1"����ô�Ͳ�����RESET�����˳�while��������һ������֮����ѭ����������	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//ѡ��ϵͳʱ��(SYSCLK)��ʱ����Դ[PLLCLK]
	
	while(RCC_GetSYSCLKSource()!=0x08);//�ж� ����ʱ��Դ��ֵ����PLLΪϵͳʱ����Դʱ������[ RCC_GetSYSCLKSource() ]�᷵��"0x08"����"0x08 = 0x08"��

	}			             //���Ժ���������������˳�ѭ����������һ��������������ֵ����(0x08)�Ǿʹ�����ȷ������ѭ�������

}



