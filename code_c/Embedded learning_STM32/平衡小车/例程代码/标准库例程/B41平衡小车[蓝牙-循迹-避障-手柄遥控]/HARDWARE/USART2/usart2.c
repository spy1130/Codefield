#include "usart2.h"
/**************************************************************************
 ��  �� ���������
 �Ա���ַ��https://shop119207236.taobao.com
**************************************************************************/
/**************************************************************************
�������ܣ�����2��ʼ��
��ڲ����� bound:������
����  ֵ����
**************************************************************************/
void uart2_init(u32 bound)
{  	 
	  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��UGPIOBʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART2ʱ��
	//USART2_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  //USART2_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);

   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure);     //��ʼ������2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2 
}

/**************************************************************************
�������ܣ�����2�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
u8 Fore,Back,Left,Right;
void USART2_IRQHandler(void)
{
	int Uart_Receive;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//�����жϱ�־λ����
	{
		Uart_Receive=USART_ReceiveData(USART2);//������յ�����
		BluetoothCMD(Uart_Receive);								
	}
}

void BluetoothCMD(int Uart_Receive)
{
	switch(Uart_Receive)
		{
			case 90://ֹͣ Z
				Fore=0,Back=0,Left=0,Right=0;
				break;
			case 65://ǰ�� A
				Fore=1,Back=0,Left=0,Right=0;
				break;
			case 72://��ǰ H
				Fore=1,Back=0,Left=1,Right=0;
				break;
			case 66://��ǰ B
				Fore=1,Back=0,Left=0,Right=1;
				break;
			case 71://��ת G
				Fore=0,Back=0,Left=1,Right=0;
				break;
			case 67://��ת C
				Fore=0,Back=0,Left=0,Right=1;
				break;
			case 69://���� E
				Fore=0,Back=1,Left=0,Right=0;
				break;
			case 70://���,������ F
				Fore=0,Back=1,Left=0,Right=1;
				break;
			case 68://�Һ������� D
				Fore=0,Back=1,Left=1,Right=0;
				break;
			default://ֹͣ Z
				Fore=0,Back=0,Left=0,Right=0;
				break;
		}
}

void Uart2SendByte(char byte)   //���ڷ���һ���ֽ�
{
		USART_SendData(USART2, byte);        //ͨ���⺯��  ��������
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  
		//�ȴ�������ɡ�   ��� USART_FLAG_TC �Ƿ���1��    //���⺯�� P359 ����
}

void Uart2SendBuf(char *buf, u16 len)
{
	u16 i;
	for(i=0; i<len; i++)Uart2SendByte(*buf++);
}
void Uart2SendStr(char *str)
{
	u16 i,len;
	len = strlen(str);
	for(i=0; i<len; i++)Uart2SendByte(*str++);
}

