#include "seg.h"
// 显示转换
void Seg_Tran(unsigned char *pucSeg_Buf, unsigned char *pucSeg_Code)
{   
  unsigned char i, j=0, temp;

  for(i=0; i<8; i++, j++)
  {
    switch(pucSeg_Buf[j]) 
    { // 低电平点亮段，段码[MSB...LSB]对应码顺序为[dp g f e d c b a]
      case '0': temp = 0xc0; break;
      case '1': temp = 0xf9; break;
      case '2': temp = 0xa4; break;
      case '3': temp = 0xb0; break;
      case '4': temp = 0x99; break;
      case '5': temp = 0x92; break;
      case '6': temp = 0x82; break;
      case '7': temp = 0xf8; break;
      case '8': temp = 0x80; break;
      case '9': temp = 0x90; break;
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
      default: temp = 0xff;
    }	
    if(pucSeg_Buf[j+1] == '.')
    {
      temp = temp&0x7f;
      j++;
    }
    pucSeg_Code[i] = temp;
  }
}
// 数码管显示
void Seg_Disp(unsigned char *pucSeg_Code, unsigned char ucSeg_Pos)
{
  P0 = 0xff; 						// 消隐
  P2 = P2 & 0x1F | 0xE0;			// P27~P25清零，再定位Y7C
  P2 &= 0x1F;						// P27~P25清零
  P0 = 1<<ucSeg_Pos; 				// 位选
  P2 = P2 & 0x1F | 0xC0;			// P27~P25清零，再定位Y6C
  P2 &= 0x1F;						// P27~P25清零
  P0 = pucSeg_Code[ucSeg_Pos]; 		// 段码
  P2 = P2 & 0x1F | 0xE0;			// P27~P25清零，再定位Y7C
  P2 &= 0x1F;						// P27~P25清零
}
