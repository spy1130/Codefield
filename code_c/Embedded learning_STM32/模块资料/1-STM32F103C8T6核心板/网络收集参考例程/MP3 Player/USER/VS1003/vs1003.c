/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               VS1003.c
** Descriptions:            The VS1003 application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2011-2-27
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "vs1003.h"

/* Private variables ---------------------------------------------------------*/

/* VS1003���ò��� */ 
/* 0 , henh.1 , hfreq.2 , lenh.3 , lfreq 5 ,������ */
uint8_t vs1003ram[5] = { 0 , 0 , 0 , 0 , 250 };
		
/*******************************************************************************
* Function Name  : VS1003_SPI_Init
* Description    : VS1003 SPI Init
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void VS1003_SPI_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE);
  /* SPI1 configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);
}

/*******************************************************************************
* Function Name  : VS1003_SPI_SetSpeed
* Description    : None
* Input          : - SpeedSet: SPI_SPEED_HIGH 1  SPI_SPEED_LOW 0
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void VS1003_SPI_SetSpeed( uint8_t SpeedSet)
{
  SPI_InitTypeDef SPI_InitStructure ;
    
  SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex ;
  SPI_InitStructure.SPI_Mode=SPI_Mode_Master ;
  SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b ;
  SPI_InitStructure.SPI_CPOL=SPI_CPOL_High ;
  SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge ;
  SPI_InitStructure.SPI_NSS=SPI_NSS_Soft ;

  if( SpeedSet == SPI_SPEED_LOW )
  {
      SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;
  }
  else 
  {
      SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;
  }
  SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB ;
  SPI_InitStructure.SPI_CRCPolynomial=7 ;
  SPI_Init(SPI1,&SPI_InitStructure);
}

/*******************************************************************************
* Function Name  : VS1003_SPI_ReadWriteByte
* Description    : None
* Input          : - TxData: Write Byte
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static uint8_t VS1003_SPI_ReadWriteByte( uint8_t TxData )
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, TxData);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}

/*******************************************************************************
* Function Name  : VS1003_Init
* Description    : VS1003�˿ڳ�ʼ��
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void VS1003_Init(void)
{		  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC , ENABLE);
  /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5  | GPIO_Pin_6 | GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  /* XCS */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* DREQ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

  MP3_DCS(1); 
  MP3_CCS(1); 

  VS1003_SPI_Init();
}

/*******************************************************************************
* Function Name  : delay_ms
* Description    : delay time
* Input          : - ms: time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void delay_ms(uint32_t ms)
{
    uint32_t len;
    for (;ms > 0; ms --)
        for (len = 0; len < 100; len++ );
}

/*******************************************************************************
* Function Name  : VS1003_WriteReg
* Description    : VS1003д�Ĵ���
* Input          : - reg: �����ַ
*				   - value: ��������
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void VS1003_WriteReg( uint8_t reg,uint16_t value )
{  
   while(  MP3_DREQ == 0 );           /* �ȴ����� */
   
   VS1003_SPI_SetSpeed( SPI_SPEED_LOW );	 
   MP3_DCS(1); 
   MP3_CCS(0); 
   VS1003_SPI_ReadWriteByte(VS_WRITE_COMMAND); /* ����VS1003��д���� */
   VS1003_SPI_ReadWriteByte(reg);             
   VS1003_SPI_ReadWriteByte(value>>8);        
   VS1003_SPI_ReadWriteByte(value);	          
   MP3_CCS(1);         
   VS1003_SPI_SetSpeed( SPI_SPEED_HIGH );
} 

/*******************************************************************************
* Function Name  : VS1003_ReadReg
* Description    : VS1003���Ĵ���
* Input          : - reg: �����ַ
* Output         : None
* Return         : - value: ����
* Attention		 : None
*******************************************************************************/
uint16_t VS1003_ReadReg( uint8_t reg)
{ 
   uint16_t value;
    
   while(  MP3_DREQ == 0 );           /* �ȴ����� */
   VS1003_SPI_SetSpeed( SPI_SPEED_LOW );
   MP3_DCS(1);     
   MP3_CCS(0);     
   VS1003_SPI_ReadWriteByte(VS_READ_COMMAND);/* ����VS1003�Ķ����� */
   VS1003_SPI_ReadWriteByte( reg );   
   value = VS1003_SPI_ReadWriteByte(0xff);		
   value = value << 8;
   value |= VS1003_SPI_ReadWriteByte(0xff); 
   MP3_CCS(1);   
   VS1003_SPI_SetSpeed( SPI_SPEED_HIGH );
   return value; 
} 

/*******************************************************************************
* Function Name  : VS1003_GetBitrate
* Description    : �õ�mp3&wma�Ĳ�����
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
uint16_t VS1003_GetBitrate(void)
{
	uint16_t head0;
	uint16_t head1;            
    const uint16_t bitrate[2][16]={ {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,0}, 
                                    {0,32,40,48,56,64,80,96,112,128,160,192,224,256,320,0}}; 

    head0 = VS1003_ReadReg(SPI_HDAT0); 
    head1 = VS1003_ReadReg(SPI_HDAT1);

    switch( head1 )
    {        
        case 0x7665:	  /* WAV��ʽ */
		     return 0;    
        case 0X4D54:	  /* MIDI��ʽ */
		     return 1;    
        case 0X574D:      /* WMA��ʽ */
             head1 = head0 * 2 / 25;
             if( ( head1 % 10 ) > 5 )
			 {
			     return head1 / 10 + 1;
			 }
             else 
			 {
			     return head1 / 10;
			 }
        default:          /* MP3��ʽ */
            head1 >>= 3;
            head1 = head1 & 0x03; 
            if( head1 == 3 )
			{
			    head1 = 1;
			}
            else 
			{
			    head1 = 0;
			}
            return bitrate[head1][head0>>12];
    } 
}  

/*******************************************************************************
* Function Name  : VS1003_ResetDecodeTime
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/                       
void VS1003_ResetDecodeTime(void)
{
   VS1003_WriteReg(SPI_DECODE_TIME,0x0000);
   VS1003_WriteReg(SPI_DECODE_TIME,0x0000); /* �������� */
}

/*******************************************************************************
* Function Name  : VS1003_GetDecodeTime
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/    
uint16_t VS1003_GetDecodeTime(void)
{ 
   return VS1003_ReadReg(SPI_DECODE_TIME);   
} 

/*******************************************************************************
* Function Name  : VS1003_SoftReset
* Description    : VS1003��λ
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_SoftReset(void)
{
   uint8_t retry; 	
   			   
   while(  MP3_DREQ == 0 );               /* �ȴ������λ���� */
   VS1003_SPI_ReadWriteByte(0xff);        /* �������� */
   retry = 0;
   while( VS1003_ReadReg(0) != 0x0804 )   /* �����λ,��ģʽ */  
   {
	   VS1003_WriteReg(SPI_MODE,0x0804);  /* �����λ,��ģʽ */
	   delay_ms(2);                       /* �ȴ�����1.35ms */
	   if( retry++ > 100 )
	   { 
	      break; 
	   }
   }	 	

   while(  MP3_DREQ ==0 );   /* �ȴ������λ���� */

   retry = 0;

   while( VS1003_ReadReg(SPI_CLOCKF) != 0X9800 ) /* ����vs1003��ʱ��,3��Ƶ ,1.5xADD */ 
   {
	   VS1003_WriteReg(SPI_CLOCKF,0X9800);       /* ����vs1003��ʱ��,3��Ƶ ,1.5xADD */
	   if( retry++ > 100 )
	   { 
	      break; 
	   }
   }		   
   retry = 0;

   while( VS1003_ReadReg(SPI_AUDATA) != 0XBB81 ) /* ����vs1003��ʱ��,3��Ƶ ,1.5xADD */
   {
	   VS1003_WriteReg(SPI_AUDATA,0XBB81);
	   if( retry++ > 100 )
	   { 
	      break; 
	   }   
   }

   VS1003_WriteReg(11,0x2020);	 /* ���� */			 
   VS1003_ResetDecodeTime();     /* ��λ����ʱ�� */	     
   /* ��vs1003����4���ֽ���Ч���ݣ���������SPI���� */	
   MP3_DCS(0);  /* ѡ�����ݴ��� */
   VS1003_SPI_ReadWriteByte(0XFF);
   VS1003_SPI_ReadWriteByte(0XFF);
   VS1003_SPI_ReadWriteByte(0XFF);
   VS1003_SPI_ReadWriteByte(0XFF);
   MP3_DCS(1);  /* ȡ�����ݴ��� */
   delay_ms(20);
} 

/*******************************************************************************
* Function Name  : VS1003_Reset
* Description    : VS1003��λ
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_Reset(void)
{
   delay_ms(20);
   VS1003_SPI_ReadWriteByte(0XFF);
   MP3_DCS(1);              /* ȡ�����ݴ��� */
   MP3_CCS(1);              /* ȡ�����ݴ��� */

   while(  MP3_DREQ ==0 );  /* �ȴ�DREQΪ�� */
   delay_ms(20);			 
}

/*******************************************************************************
* Function Name  : VS1003_SineTest
* Description    : VS1003���Ҳ���
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void VS1003_SineTest(void)
{	
   VS1003_WriteReg(0x0b,0X2020);	  /* �������� */	 
   VS1003_WriteReg(SPI_MODE,0x0820); /* ����vs1003�Ĳ���ģʽ */	    
   while(  MP3_DREQ == 0 );  /* �ȴ�DREQΪ�� */
 	
   /* ��vs1003�������Ҳ������0x53 0xef 0x6e n 0x00 0x00 0x00 0x00 */
   MP3_DCS(0);/* ѡ�����ݴ��� */
   VS1003_SPI_ReadWriteByte(0x53);
   VS1003_SPI_ReadWriteByte(0xef);
   VS1003_SPI_ReadWriteByte(0x6e);
   VS1003_SPI_ReadWriteByte(0x24);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(100);
   MP3_DCS(1); 
   /* �˳����Ҳ��� */
   MP3_DCS(0);/* ѡ�����ݴ��� */
   VS1003_SPI_ReadWriteByte(0x45);
   VS1003_SPI_ReadWriteByte(0x78);
   VS1003_SPI_ReadWriteByte(0x69);
   VS1003_SPI_ReadWriteByte(0x74);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(100);
   MP3_DCS(1);		 

   /* �ٴν������Ҳ��Բ�����nֵΪ0x44���������Ҳ���Ƶ������Ϊ�����ֵ */
   MP3_DCS(0);/* ѡ�����ݴ��� */      
   VS1003_SPI_ReadWriteByte(0x53);
   VS1003_SPI_ReadWriteByte(0xef);
   VS1003_SPI_ReadWriteByte(0x6e);
   VS1003_SPI_ReadWriteByte(0x44);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(100);
   MP3_DCS(1);
   /* �˳����Ҳ��� */
   MP3_DCS(0);/* ѡ�����ݴ��� */      
   VS1003_SPI_ReadWriteByte(0x45);
   VS1003_SPI_ReadWriteByte(0x78);
   VS1003_SPI_ReadWriteByte(0x69);
   VS1003_SPI_ReadWriteByte(0x74);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(100);
   MP3_DCS(1);	 
}	 

/*******************************************************************************
* Function Name  : VS1003_RamTest
* Description    : VS1003 RAM����
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/																			 
void VS1003_RamTest(void)
{
   volatile uint16_t value;
     
   VS1003_Reset();     
   VS1003_WriteReg(SPI_MODE,0x0820); /* ����vs1003�Ĳ���ģʽ */
   while(  MP3_DREQ == 0 );          /* �ȴ�DREQΪ�� */
   MP3_DCS(0);	       			     /* xDCS = 1��ѡ��vs1003�����ݽӿ� */
   VS1003_SPI_ReadWriteByte(0x4d);
   VS1003_SPI_ReadWriteByte(0xea);
   VS1003_SPI_ReadWriteByte(0x6d);
   VS1003_SPI_ReadWriteByte(0x54);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   VS1003_SPI_ReadWriteByte(0x00);
   delay_ms(50);  
   MP3_DCS(1);
   value = VS1003_ReadReg(SPI_HDAT0); /* ����õ���ֵΪ0x807F���������� */
}     
		 				
/*******************************************************************************
* Function Name  : VS1003_SetVol
* Description    : �趨vs1003���ŵ������͸ߵ���
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/   
void VS1003_SetVol(void)
{
   uint8_t i;
   uint16_t bass=0;  /* �ݴ������Ĵ���ֵ */
   uint16_t volt=0;  /* �ݴ�����ֵ */
   uint8_t  vset=0;  /* �ݴ�����ֵ */ 
		 
   vset = 255 - vs1003ram[4]; /* ȡ���õ����ֵ */ 
   volt =vset;
   volt <<= 8;
   volt += vset;/* �õ��������ú��С */
   /* 0,henh.1,hfreq.2,lenh.3,lfreq */      
   for( i = 0; i < 4; i++ )
   {
       bass <<= 4;
       bass += vs1003ram[i]; 
   }     
   VS1003_WriteReg(SPI_BASS,bass);/* BASS */   
   VS1003_WriteReg(SPI_VOL,volt); /* ������ */ 
}    

/*******************************************************************************
* Function Name  : VS1003_WriteData
* Description    : VS1003д����
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/  
void VS1003_WriteData( uint8_t * buf)
{		
   uint8_t  count = 32;

   MP3_DCS(0);   
   while( count-- )
   {
	  VS1003_SPI_ReadWriteByte( *buf++ );
   }
   MP3_DCS(1);  
   MP3_CCS(1);  
} 

/*******************************************************************************
* Function Name  : VS1003_Record_Init
* Description    : VS1003¼����ʼ��
* Input          : None
* Output         : None
* Return         : None
* Attention		 : VS1003¼����ʼ���ɹ�֮��������������лط�
*                  ��ʱ�������������ײ����Լ�
*******************************************************************************/ 
void VS1003_Record_Init(void)
{
  uint8_t retry; 	

  /* ����ʱ�� */
  while( VS1003_ReadReg(SPI_CLOCKF) != 0x9800 )   
  {
	  VS1003_WriteReg(SPI_CLOCKF,0x9800);   
	  delay_ms(2);                        /* �ȴ�����1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  while( VS1003_ReadReg(SPI_BASS) != 0x0000 )   
  {
	  VS1003_WriteReg(SPI_CLOCKF,0x0000);   
	  delay_ms(2);                        /* �ȴ�����1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  /* Set sample rate divider=12 */ 
  while( VS1003_ReadReg(SPI_AICTRL0) != 0x0012 )   
  {
	  VS1003_WriteReg(SPI_AICTRL0,0x0012);   
	  delay_ms(2);                        /* �ȴ�����1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  /* AutoGain OFF, reclevel 0x1000 */ 
  while( VS1003_ReadReg(SPI_AICTRL1) != 0x1000 )   
  {
	  VS1003_WriteReg(SPI_AICTRL1,0x1000);   
	  delay_ms(2);                        /* �ȴ�����1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  /* RECORD,NEWMODE,RESET */ 
  while( VS1003_ReadReg(SPI_MODE) != 0x1804 )   
  {
	  VS1003_WriteReg(SPI_MODE,0x1804);   
	  delay_ms(2);                        /* �ȴ�����1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  while( VS1003_ReadReg(SPI_CLOCKF) != 0x9800 )   
  {
	  VS1003_WriteReg(SPI_CLOCKF,0x9800);   
	  delay_ms(2);                        /* �ȴ�����1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

