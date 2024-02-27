#ifndef __SEG_H
#define __SEG_H
#include "STC15F2K60S2.H"

void Seg_Tran(unsigned char *pucSeg_Buf, unsigned char *pucSeg_Code);
void Seg_Disp(unsigned char *pucSeg_Code, unsigned char ucSeg_Pos);
#endif

