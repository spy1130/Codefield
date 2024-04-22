#include "SEG.h"
#include "STC15F2K60S2.H"
#include "STRING.h"
code unsigned char  SMG_Table[]=
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
     0xbf,0x7f};
//�����������ַ�������������ܶ��롱��ת������
//��ڲ�����seg_string��10���ֽڣ���ת�����ַ�������ת����Ķ���浽seg_buf��8���ֽڣ���
//����ֵ����
//�������ܣ���seg_string�ĸ���λת��Ϊ����ܶ��룬�洢��seg_buf�����С�
void NumTran(unsigned char* seg_string,unsigned char* seg_buf)
{	
	unsigned char i=0;
	unsigned char j=0;
	unsigned char temp;
	for(i=0;i<len;i++,j++)
	{	
		switch(seg_string[j]){
			case '0': temp=SMG_Table[0]; break;
			case '1': temp=SMG_Table[1]; break;
			case '2': temp=SMG_Table[2]; break;
			case '3': temp=SMG_Table[3]; break;
			case '4': temp=SMG_Table[4]; break;
			case '5': temp=SMG_Table[5]; break;
			case '6': temp=SMG_Table[6]; break;
			case '7': temp=SMG_Table[7]; break;
			case '8': temp=SMG_Table[8]; break;
			case '9': temp=SMG_Table[9]; break;
			case 'A': temp=SMG_Table[10]; break;
			case 'b': temp=SMG_Table[11]; break;
			case 'C': temp=SMG_Table[12]; break;
			case 'd': temp=SMG_Table[13]; break;
			case 'E': temp=SMG_Table[14]; break;
			case 'F': temp=SMG_Table[15]; break;
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

