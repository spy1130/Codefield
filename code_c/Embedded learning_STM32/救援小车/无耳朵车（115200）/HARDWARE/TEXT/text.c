#include "sys.h" 
#include "fontupd.h"

#include "lcd.h"
#include "text.h"	
#include "string.h"												    
#include "usart.h"				
char fre_font[32*50]={0x00,0x11,0x00,0x16,0x7C,0x90,0x44,0x90,0x54,0x94,0x4C,0x93,0x44,0x90,0x7F,0xF0,
0x44,0x94,0x4C,0x93,0x54,0x90,0x44,0x90,0x7C,0x94,0x00,0x13,0x00,0x10,0x00,0x00,/*"��",0*/

0x00,0x02,0x00,0x0C,0x3F,0xF1,0x24,0x01,0x24,0x21,0x24,0x32,0x3F,0xAA,0xA4,0xA4,
0x64,0xA4,0x24,0xA4,0x3F,0xAA,0x24,0x32,0x24,0x01,0x24,0x01,0x20,0x01,0x00,0x00,/*"��",1*/

0x00,0x80,0x01,0x00,0x06,0x00,0x1F,0xFF,0xE0,0x02,0x20,0x02,0x27,0xFE,0x25,0x52,
0x25,0x52,0xFD,0x52,0x25,0x52,0x25,0x52,0x27,0xFE,0x20,0x02,0x00,0x02,0x00,0x00,/*"ֵ",2*/

0x02,0x00,0x04,0x00,0x1F,0xFF,0xE0,0x00,0x0F,0xF8,0x05,0x01,0x09,0x49,0x32,0x4A,
0xEA,0x92,0x24,0x94,0x25,0x24,0x2A,0x48,0x32,0x10,0x01,0x00,0x01,0x00,0x00,0x00,/*"��",3*/

0x00,0x02,0x40,0x02,0x40,0x02,0x43,0xFE,0x40,0x02,0x40,0x02,0x40,0x02,0x7F,0xFE,
0x41,0x02,0x41,0x02,0x41,0x02,0x41,0x02,0x41,0x02,0x40,0x02,0x00,0x02,0x00,0x00,/*"��",4*/


};
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������V3
//������ʾ ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/20
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
 
//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С	
//size:�����С
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset; 
	u8 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
	{   		    
	    for(i=0;i<csize;i++)*mat++=0x00;//�������
	    return; //��������
	}          
	if(ql<0x7f)ql-=0x40;//ע��!
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	//�õ��ֿ��е��ֽ�ƫ����  		  
	switch(size)
	{
		case 12:
			//W25QXX_Read(mat,foffset+ftinfo.f12addr,csize);
			foffset++;
			break;
		case 16:
			//W25QXX_Read(mat,foffset+ftinfo.f16addr,csize);
			break;
		case 24:
			//W25QXX_Read(mat,foffset+ftinfo.f24addr,csize);
			break;
			
	}     												    
}  
//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//size:�����С
//mode:0,������ʾ,1,������ʾ	   
void Show_Font(u16 x,u16 y,int num,u8 size,u8 mode)
{
	u8 temp,t,t1,font_i;
	u16 y0=y;
	u8 dzk[72];   
	u8 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	if(size!=12&&size!=16&&size!=24)return;	//��֧�ֵ�size
	//Get_HzMat(font,dzk,size);	//�õ���Ӧ��С�ĵ������� 
	//��������������dzk[72]����������ʹ�ã�����int numֻѡ��Ҫ�����Ī��Ϣ
	
//	char fre_font[32*3]={0x40,0x90,0x7C,0x8E,0x40,0x40,0x7F,0x4F,0x48,0x20,0x48,0x1E,0x40,0x80,0xF2,0x4F,
//								0x12,0x20,0x1A,0x18,0xD6,0x07,0x12,0x10,0x12,0x20,0xF2,0x4F,0x02,0x80,0x00,0x00,/*"Ƶ",0*/

//0x00,0x08,0x14,0x09,0xA4,0x08,0x44,0x08,0x24,0x09,0x34,0x09,0xAD,0x09,0x66,0xFD,
//0x24,0x09,0x94,0x09,0x04,0x0B,0x44,0x08,0xA4,0x08,0x14,0x09,0x00,0x08,0x00,0x00,/*"��",1*/

//0x00,0x01,0x80,0x00,0x60,0x00,0xF8,0xFF,0x07,0x40,0x04,0x40,0xE4,0x7F,0xA4,0x4A,
//0xA4,0x4A,0xBF,0x4A,0xA4,0x4A,0xA4,0x4A,0xE4,0x7F,0x04,0x40,0x00,0x40,0x00,0x00,/*"ֵ",2*/


//}; ----��  dzk[72];
	for( font_i=0;font_i<64;font_i++)
		{
			dzk[font_i]=fre_font[32*num+font_i];
		}

	
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//�õ���������                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR); 
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���	    
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ    	   		   
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode)
{					
	u16 x0=x;
	u16 y0=y;							  	  
    u8 bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//���� 
	        else              //�ַ�
	        {      
                if(x>(x0+width-size/2))//����
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))break;//Խ�緵��      
		        if(*str==13)//���з���
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else LCD_ShowChar(x,y,*str,size,mode);//��Ч����д�� 
				str++; 
		        x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
	        }
        }else//���� 
        {     
            bHz=0;//�к��ֿ�    
            if(x>(x0+width-size))//����
			{	    
				y+=size;
				x=x0;		  
			}
	        if(y>(y0+height-size))break;//Խ�緵��  						     
	        //Show_Font(x,y,str,size,mode); //��ʾ�������,������ʾ 
	        str+=2; 
	        x+=size;//��һ������ƫ��	    
        }						 
    }   
}  			 		 
//��ָ����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ��			  
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len)
{
	u16 strlenth=0;
   	strlenth=strlen((const char*)str);
	strlenth*=size/2;
	if(strlenth>len)Show_Str(x,y,lcddev.width,lcddev.height,str,size,1);
	else
	{
		strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+x,y,lcddev.width,lcddev.height,str,size,1);
	}
}   


























		  






