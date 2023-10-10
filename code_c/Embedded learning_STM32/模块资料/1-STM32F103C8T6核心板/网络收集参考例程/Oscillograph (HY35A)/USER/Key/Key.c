/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               key.c
** Descriptions:            The key application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2011-3-16
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
#include "key.h"


/* Private variables ---------------------------------------------------------*/
static BUTTON_T s_BtnUser;		/* USER �� */
static BUTTON_T s_BtnTamper;	/* TAMPER �� */
static BUTTON_T s_BtnWakeUp;	/* WAKEUP �� */
static BUTTON_T s_BtnUp;		/* ҡ��UP�� */
static BUTTON_T s_BtnDown;		/* ҡ��DOWN�� */
static BUTTON_T s_BtnLeft;		/* ҡ��LEFT�� */
static BUTTON_T s_BtnRight;		/* ҡ��RIGHT�� */
static BUTTON_T s_BtnOk;		/* ҡ��OK�� */

static KEY_FIFO_T s_Key;		/* ����FIFO����,�ṹ�� */

/* Private function prototypes -----------------------------------------------*/
static uint8_t IsKeyDownUser(void) 		{if ( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)  ) return 0; return 1;}
static uint8_t IsKeyDownTamper(void) 	{if ( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) ) return 0; return 1;}
static uint8_t IsKeyDownWakeUp(void) 	{if ( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)  ) return 1; return 0;}
static uint8_t IsKeyDownUp(void) 		{if ( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11) ) return 0; return 1;}
static uint8_t IsKeyDownDown(void) 		{if ( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_8)  ) return 0; return 1;}
static uint8_t IsKeyDownLeft(void) 		{if ( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13) ) return 0; return 1;}
static uint8_t IsKeyDownRight(void) 	{if ( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14) ) return 0; return 1;}
static uint8_t IsKeyDownOk(void) 		{if ( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7)  ) return 0; return 1;}

/*******************************************************************************
* Function Name  : KEY_GPIO_Configuration
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void KEY_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOG, ENABLE); 						 
  /**
  *  JOY_UP -> PG11 , JOY_DOWN -> PG8 , JOY_LEFT -> PG13 , JOY_RIGHT -> PG14 , JOY_SEL -> PG7
  *  Tamper Button -> PC13 , Wakeup Button -> PA0 , User Button -> PA8
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : Key_Initializtion
* Description    : ��ʼ�����еİ�������
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Key_Initializtion(void)
{
    KEY_GPIO_Configuration();

	/* �԰���FIFO��дָ������ */
	s_Key.Read = 0;
	s_Key.Write = 0;

	/* ��ʼ��USER�������� */
	s_BtnUser.IsKeyDownFunc = IsKeyDownUser;	    /* �жϰ������µĺ��� */
	s_BtnUser.FilterTime = BUTTON_FILTER_TIME;	    /* �����˲�ʱ�� */
	s_BtnUser.LongTime = BUTTON_LONG_TIME;		    /* ����ʱ�� */
	s_BtnUser.Count = s_BtnUser.FilterTime / 2;	    /* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnUser.State = 0;						    /* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnUser.KeyCodeDown = KEY_DOWN_USER;		    /* �������µļ�ֵ���� */
	s_BtnUser.KeyCodeUp = KEY_UP_USER;			    /* ��������ļ�ֵ���� */
	s_BtnUser.KeyCodeLong = KEY_HOLD_USER;		    /* �������������µļ�ֵ���� */

	/* ��ʼ��TAMPER�������� */
	s_BtnTamper.IsKeyDownFunc = IsKeyDownTamper;	/* �жϰ������µĺ��� */
	s_BtnTamper.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_BtnTamper.LongTime = 0;						/* ����ʱ��, 0��ʾ����� */
	s_BtnTamper.Count = s_BtnTamper.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnTamper.State = 0;							/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnTamper.KeyCodeDown = KEY_DOWN_TAMPER;		/* �������µļ�ֵ���� */
	s_BtnTamper.KeyCodeUp = 0;						/* ��������ļ�ֵ���� */
	s_BtnTamper.KeyCodeLong = 0;					/* �������������µļ�ֵ���� */

	/* ��ʼ��WAKEUP�������� */
	s_BtnWakeUp.IsKeyDownFunc = IsKeyDownWakeUp;	/* �жϰ������µĺ��� */
	s_BtnWakeUp.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_BtnWakeUp.LongTime = 0;						/* ����ʱ�� */
	s_BtnWakeUp.Count = s_BtnWakeUp.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnWakeUp.State = 0;							/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnWakeUp.KeyCodeUp = 0;						/* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnWakeUp.KeyCodeDown = KEY_DOWN_WAKEUP;		/* �������µļ�ֵ���� */
	s_BtnWakeUp.KeyCodeLong = 0;					/* �������������µļ�ֵ���룬0��ʾ����� */

	/* ��ʼ��UP�������� */
	s_BtnUp.IsKeyDownFunc = IsKeyDownUp;		    /* �жϰ������µĺ��� */
	s_BtnUp.FilterTime = BUTTON_FILTER_TIME;	    /* �����˲�ʱ�� */
	s_BtnUp.LongTime = 0;						    /* ����ʱ�� */
	s_BtnUp.Count = s_BtnUp.FilterTime / 2;		    /* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnUp.State = 0;							    /* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnUp.KeyCodeDown = KEY_DOWN_JOY_UP;		    /* �������µļ�ֵ���� */
	s_BtnUp.KeyCodeUp = 0;						    /* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnUp.KeyCodeLong = 0;					    /* �������������µļ�ֵ���룬0��ʾ����� */

	/* ��ʼ��DOWN�������� */
	s_BtnDown.IsKeyDownFunc = IsKeyDownDown;	    /* �жϰ������µĺ��� */
	s_BtnDown.FilterTime = BUTTON_FILTER_TIME;	    /* �����˲�ʱ�� */
	s_BtnDown.LongTime = 0;						    /* ����ʱ�� */
	s_BtnDown.Count = s_BtnDown.FilterTime / 2;	    /* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnDown.State = 0;						    /* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnDown.KeyCodeDown = KEY_DOWN_JOY_DOWN;	    /* �������µļ�ֵ���� */
	s_BtnDown.KeyCodeUp = 0;					    /* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnDown.KeyCodeLong = 0;					    /* �������������µļ�ֵ���룬0��ʾ����� */

	/* ��ʼ��LEFT�������� */
	s_BtnLeft.IsKeyDownFunc = IsKeyDownLeft;	    /* �жϰ������µĺ��� */
	s_BtnLeft.FilterTime = BUTTON_FILTER_TIME;	    /* �����˲�ʱ�� */
	s_BtnLeft.LongTime = 0;						    /* ����ʱ�� */
	s_BtnLeft.Count = s_BtnLeft.FilterTime / 2;	    /* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnLeft.State = 0;						    /* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnLeft.KeyCodeDown = KEY_DOWN_JOY_LEFT;	    /* �������µļ�ֵ���� */
	s_BtnLeft.KeyCodeUp = 0;					    /* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnLeft.KeyCodeLong = 0;					    /* �������������µļ�ֵ���룬0��ʾ����� */

	/* ��ʼ��RIGHT�������� */
	s_BtnRight.IsKeyDownFunc = IsKeyDownRight;	    /* �жϰ������µĺ��� */
	s_BtnRight.FilterTime = BUTTON_FILTER_TIME;	    /* �����˲�ʱ�� */
	s_BtnRight.LongTime = 0;					    /* ����ʱ�� */
	s_BtnRight.Count = s_BtnRight.FilterTime / 2;   /* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnRight.State = 0;						    /* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnRight.KeyCodeDown = KEY_DOWN_JOY_RIGHT;    /* �������µļ�ֵ���� */
	s_BtnRight.KeyCodeUp = 0;					    /* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnRight.KeyCodeLong = 0;					    /* �������������µļ�ֵ���룬0��ʾ����� */

	/* ��ʼ��OK�������� */
	s_BtnOk.IsKeyDownFunc = IsKeyDownOk;		    /* �жϰ������µĺ��� */
	s_BtnOk.FilterTime = BUTTON_FILTER_TIME;	    /* �����˲�ʱ�� */
	s_BtnOk.LongTime = 0;						    /* ����ʱ�� */
	s_BtnOk.Count = s_BtnOk.FilterTime / 2;		    /* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnOk.State = 0;							    /* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnOk.KeyCodeDown = KEY_DOWN_JOY_OK;		    /* �������µļ�ֵ���� */
	s_BtnOk.KeyCodeUp = 0;						    /* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnOk.KeyCodeLong = 0;					    /* �������������µļ�ֵ���룬0��ʾ����� */
}

/*******************************************************************************
* Function Name  : PutKey
* Description    : ����ֵѹ�밴��FIFO������
* Input          : - _KeyCode: key
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutKey(uint8_t _KeyCode)
{
	s_Key.Buf[s_Key.Write] = _KeyCode;

	if (++s_Key.Write  >= KEY_FIFO_SIZE)
	{
		s_Key.Write = 0;
	}
}
  
/*******************************************************************************
* Function Name  : GetKey
* Description    : �Ӱ���FIFOȡ1����ֵ
* Input          : None
* Output         : None
* Return         : ���ؼ�ֵ
* Attention		 : None
*******************************************************************************/
uint8_t GetKey(void)
{
	uint8_t ret;

	if (s_Key.Read == s_Key.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_Key.Buf[s_Key.Read];

		if (++s_Key.Read >= KEY_FIFO_SIZE)
		{
			s_Key.Read = 0;
		}
		return ret;
	}
}

/*******************************************************************************
* Function Name  : DetectButton
* Description    : ���ָ���İ���
* Input          : - _pBtn: �����ṹ����ָ�� 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void DetectButton(BUTTON_T *_pBtn)
{
	if (_pBtn->IsKeyDownFunc())
	{
		if (_pBtn->Count < _pBtn->FilterTime)
		{
			_pBtn->Count = _pBtn->FilterTime;
		}
		else if(_pBtn->Count < 2 * _pBtn->FilterTime)
		{
			_pBtn->Count++;
		}
		else
		{
			if (_pBtn->State == 0)
			{
				_pBtn->State = 1;

				/* ���Ͱ�ť���µ���Ϣ */
				if (_pBtn->KeyCodeDown > 0)
				{
					/* ��ֵ���밴��FIFO */
					PutKey(_pBtn->KeyCodeDown);
				}
			}

			if (_pBtn->LongTime > 0)
			{
				if (_pBtn->LongCount < _pBtn->LongTime)
				{
					/* ���Ͱ�ť�������µ���Ϣ */
					if (++_pBtn->LongCount == _pBtn->LongTime)
					{
						/* ��ֵ���밴��FIFO */
						PutKey(_pBtn->KeyCodeLong);
					}
				}
			}
		}
	}
	else
	{
		if(_pBtn->Count > _pBtn->FilterTime)
		{
			_pBtn->Count = _pBtn->FilterTime;
		}
		else if(_pBtn->Count != 0)
		{
			_pBtn->Count--;
		}
		else
		{
			if (_pBtn->State == 1)
			{
				_pBtn->State = 0;

				/* ���Ͱ�ť�������Ϣ */
				if (_pBtn->KeyCodeUp > 0)
				{
					/* ��ֵ���밴��FIFO */
					PutKey(_pBtn->KeyCodeUp);
				}
			}
		}

		_pBtn->LongCount = 0;
	}
}

/*******************************************************************************
* Function Name  : KeyPro
* Description    : ������еİ���
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void KeyPro(void)
{
	DetectButton(&s_BtnUser);	/* USER �� */
	DetectButton(&s_BtnTamper);	/* TAMPER �� */
	DetectButton(&s_BtnWakeUp);	/* WAKEUP �� */
	DetectButton(&s_BtnUp);		/* ҡ��UP�� */
	DetectButton(&s_BtnDown);	/* ҡ��DOWN�� */
	DetectButton(&s_BtnLeft);	/* ҡ��LEFT�� */
	DetectButton(&s_BtnRight);	/* ҡ��RIGHT�� */
	DetectButton(&s_BtnOk);		/* ҡ��OK�� */
}
								
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
