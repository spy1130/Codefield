#include "key.h"
#include "STC15F2K60S2.H"
//��������������ֵ��ȡ����-��������
//��ڲ�������
//����ֵ����������ֵ
//�������ܣ����ذ����롣
unsigned char Key_BIN(void)
{
	unsigned char Key_Value;
	
	if(P30 ==0) Key_Value=7 ;
	else if(P31 ==0) Key_Value=6;
	else if(P32 ==0) Key_Value=5;
	else if(P33 ==0) Key_Value=4;
	else Key_Value = 0;
	
	return Key_Value;
}