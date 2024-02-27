# include "bsp_seg.h"

//�����������ַ�������������ܶ��롱��ת������
//��ڲ�����seg_string��10���ֽڣ���ת�����ַ�������ת����Ķ���浽seg_buf��8���ֽڣ���
//����ֵ����
//�������ܣ���seg_string�ĸ���λת��Ϊ����ܶ��룬�洢��seg_buf�����С�
void Seg_Tran(unsigned char *seg_string, unsigned char *seg_buf)
{
	unsigned char i=0;//buf[i],i=0~7
	unsigned char j=0;//seg_string[j],j=0~10	
	unsigned char temp;//�ַ���ת��Ϊ������м�洢����
	
	for(i=0;i<=7;i++,j++)
	{
		switch(seg_string[j])
		{
			case '0':	temp = 0xc0; break;			
			case '1':	temp = 0xf9; break;			
			case '2':	temp = 0xa4; break;			
			case '3':	temp = 0xb0; break;		
			case '4':	temp = 0x99; break;			
			case '5':	temp = 0x92; break;		
			case '6':	temp = 0x82; break;			
			case '7':	temp = 0xf8; break;		
			case '8':	temp = 0x80; break;			
			case '9':	temp = 0x90; break;		
					
			case 'A': temp = 0x88; break;
			case 'B': temp = 0x83; break;
			case 'C': temp = 0xc6; break;
			case 'D': temp = 0xA1; break;
			case 'E': temp = 0x86; break;
			case 'F': temp = 0x8E; break;
			
			case 'H': temp = 0x89; break;
			case 'L': temp = 0xC7; break;
			case 'N': temp = 0xC8; break;
			case 'P': temp = 0x8c; break;
			case 'U': temp = 0xC1; break;
			
			case '-': temp = 0xbf; break;
			case ' ': temp = 0xff; break;		
			
			default : temp = 0xff; break;//��"."ʱ��ѡ�����
		
		}
	
		if(seg_string[j+1] == '.')//����ַ�����߳����ˡ�.����Ҫ�Ѹոճ�¯��tempֵ�ı䡣
		{
			temp &= 0x7f;//��dpλ���������ı�֮ǰ��tempֵ��ֻ�������dpλ
			j++;//������'.'��λ��
		}
		
		seg_buf[i] = temp;//��ת����Ķ���ֵ���ݸ�Buf�洢��

	}
}



//�������������������顱�ĵ�Nλ��ʾ��
//��ڲ�����seg_buf��8���ֽڣ���ת����Ķ���; pos����ʾ��seg_buf��posλ��
//����ֵ����
//�������ܣ���seg_buf�ĵ�posλ��ʾ������ܵĵ�posλ
void Seg_Disp(unsigned char *seg_buf, unsigned char pos)
{
	

	
	P0 = 1<<pos;//λ������
	P2 = P2 & 0x1F | 0xC0; // 0xC0��ѡͨY6��Ҳ����λ�����������������͸����ȥ
	P2 &= 0x1F; //���򿪵��������رգ�ʹ�κ�һ��������������	
	
	P0 = 0xff;//�����Ļ
	P2 = P2 & 0x1F | 0xE0; // 0xE0��ѡͨY7��Ҳ���Ƕ������������������͸����ȥ
	P2 &= 0x1F; //���򿪵��������رգ�ʹ�κ�һ��������������		
	
	P0 = seg_buf[pos];//��������
	P2 = P2 & 0x1F | 0xE0; // 0xE0��ѡͨY7��Ҳ���Ƕ������������������͸����ȥ
	P2 &= 0x1F; //���򿪵��������رգ�ʹ�κ�һ��������������
}






