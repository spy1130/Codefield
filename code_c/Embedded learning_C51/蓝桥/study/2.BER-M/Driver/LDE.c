#include "LED.h"
//����16���ƣ�8λ,1��0��
void LED(unsigned int num)
{
	P0=~num;
	P2= P2 & 0X1F | 0X80;//����Y4
	P2 &=0X1F;//�ر�Y4~Y6
}