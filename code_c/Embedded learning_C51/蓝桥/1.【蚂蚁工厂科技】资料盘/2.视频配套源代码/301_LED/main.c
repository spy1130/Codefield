# include "bsp_init.h"
# include "bsp_led.h"
# include "delay.h"

void main()
{
	unsigned char i,j;//Ϊѭ��������׼��
	
	Cls_Peripheral();//�ر�����
	
	while(1)
	{
		for(i=0;i<9;i++)//�������ȵȼ�
		{
			for(j=0;j<50;j++)//���Ƶ�ǰ���ȵȼ���ѭ������
			{
   			 Led_Disp(0xff);//ȫ��
			   Delay(i+1);//����ʱ��
				 Led_Disp(0x00);//ȫ��
			   Delay(9-i);//���ʱ��						
			}
		}
	}
}	











