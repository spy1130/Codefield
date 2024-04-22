#include "SEG.h"
#include "STC15F2K60S2.H"
#include "STRING.h"

//�����������ַ�������������ܶ��롱��ת������
//��ڲ�����seg_string��10���ֽڣ���ת�����ַ�������ת����Ķ���浽seg_buf��8���ֽڣ���
//����ֵ����
//�������ܣ���seg_string�ĸ���λת��Ϊ����ܶ��룬�洢��seg_buf�����С�
void NumTran(unsigned char* seg_string,unsigned char* seg_buf)
{	
	unsigned char i=0;
	unsigned char j=0;
	unsigned char temp;
	for(i=0;i<8;i++,j++)
	{	
		switch(seg_string[j]){
			case '0': temp=0xc0; break;
			case '1': temp=0xf9; break;
			case '2': temp=0xa4; break;
			case '3': temp=0xb0; break;
			case '4': temp=0x99; break;
			case '5': temp=0x92; break;
			case '6': temp=0x82; break;
			case '7': temp=0xf8; break;
			case '8': temp=0x80; break;
			case '9': temp=0x90; break;
			case 'A': temp=0x88; break;
			case 'b': temp=0x83; break;
			case 'C': temp=0xc6; break;
			case 'd': temp=0xa1; break;
			case 'E': temp=0x86; break;
			case 'F': temp=0x8e; break;
			case '-': temp = 0xbf; break;
			case ' ': temp = 0xff; break;
			
			default :temp=0xff; break;
		}
		if(seg_string[j+1]=='.'){//'.'�����ֻ�����ĸ��ͬһ���ط�,����Ҫ����һ���ǲ���'.'������temp���
			temp &= 0x7f;
			j++;//����'.'
		}
		seg_buf[i]=temp;
	}
}

//�������������������顱�ĵ�Nλ��ʾ��
//��ڲ�����seg_buf��8���ֽڣ���ת����Ķ���; pos����ʾ��seg_buf��posλ��
//����ֵ����
//�������ܣ���seg_buf�ĵ�posλ��ʾ������ܵĵ�posλ
void NumShow(unsigned char* seg_buf,unsigned char pos)
{
	P0=0x01<<pos;
	P2 =P2 & 0X1f | 0XC0;//y6
	P2 &=0X1f ;
	
	P0 =0XFF;//clear
	P2 =P2 & 0X1f | 0XE0;//Y7
	P2 &=0X1f ;
	
	P0=seg_buf[pos];//����
	P2 =P2 & 0X1f | 0XE0;//Y7
	P2 &=0X1f ;
}

