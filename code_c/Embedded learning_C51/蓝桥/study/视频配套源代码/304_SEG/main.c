# include "timer.h"
# include "bsp_seg.h"
# include "bsp_init.h"
# include "STDIO.H"

//ȫ�ֱ���
unsigned char disp_data = 123;//����ʾ������
unsigned char seg_string[10];//��С����ͽ�����������2���ռ���ַ�������
unsigned char seg_buf[8];//��������������һһ��Ӧ
unsigned char pos=0;//�����Ҫ��ʾ��λ��



main()
{
	
	Cls_Peripheral();//�ر�����
	Timer1Init();//��ʱ��1�����õ��������ʾ1msһ�εĿ��ơ�
	EA = 1;//�����ж�
	
	sprintf(seg_string, "%8u", (unsigned int)disp_data);//seg_string��_ _ _ _ _ 1 2 3 \0
	Seg_Tran(seg_string, seg_buf);//���ַ�������ת����������seg_buf��0xbf 0xbf 0xbf 0xbf 0xbf 0xf9 0xa4 0xb0 
	
	while(1);

}




//-----------------------------------------------
/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{
	Seg_Disp(seg_buf, pos);
	pos++;
	if(pos == 8)
		pos = 0;
}
