/* 

	���װ������̵ײ�v2.0
	
	�������ݣ�
		1.�޸������෴��bug
		2.�����ĺ궨��д��key.h
		
	ע�⣺
		��ʹ��ʱһ��ҲҪ��key.h�滻��
		
	���þ�����
		void Key_Proc()
		{
			if(Key_Slow_Down) return;
			Key_Slow_Down = 1;

			Key_Val = Key_Read();
			Key_Down = Key_Val & (Key_Old ^ Key_Val);
			Key_Up = ~Key_Val & (Key_Old ^ Key_Val);
			Key_Old = Key_Val;
			
			if(KeyDown_Num(4))
			{
				if(!KeyDown_Num(9)) //�ų���ϰ���
				{
					//S4������������
				}
			}

			if(KeyDown_Num(9)) //����S9�����½���
			{
				if(!KeyDown_Num(4)) //�ų���ϰ���
				{
					//S9������������
				}
			}

			if((KeyDown_Num(4)) && (KeyDown_Num(9))) 
			{
				//S4+S9��ϰ���������
			}
		}

*/

#include <Key.h>

unsigned int Key_Read()
{
	unsigned int temp = 0;//16bit��Ӧ16������
	int row, col;
	
	for(col=0;col<4;col++) 
	{
		P44 = (col != 0);
		P42 = (col != 1);
		P35 = (col != 2);
		P34 = (col != 3);
		
		for(row=0;row<4;row++) 
		{
			if (!(P3&(0x08>>row))) //��0��-P33 ��1��-P32 ��2��-P31 ��3��-P30 ˳���෴ 
				temp |= (1 << ( col* 4 + row ));
		}
	}
	return temp;
}





/*
unsigned char Key_Read()
{
	unsigned char temp = 0;
	if(P33 == 0) temp = 4;
	if(P32 == 0) temp = 5;
	if(P31 == 0) temp = 6;
	if(P30 == 0) temp = 7;
	return temp;
}
*/

