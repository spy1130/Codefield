# include "delay.h"


//����������ʱ���뺯��
//��ڲ�������ʱ����ֵ��2���ֽڣ�16λ��0---65535
//����ֵ����
//�������ܣ���ʱһ���ĺ�������
void Delay(unsigned int num)
{
	unsigned int i;
	while(num--)
	for(i=0; i<628; i++);
}
