#include "STC15F2K60S2.H"
#include "INT.h"

void INT0_init()
{
	IT0=1;//�½��ش���
	EX0=1;//�������ж�0ʹ��
	
	EA=1;//�����ж�
}

void INT1_init()
{
	IT1=1;//�½��ش���
	EX1=1;//�������ж�1ʹ��
	
	EA=1;//�����ж�
}