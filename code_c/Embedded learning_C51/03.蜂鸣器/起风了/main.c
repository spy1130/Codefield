#include "reg51.h"

#define uchar unsigned char

sbit beep=P2^5;

uchar m,n;

uchar code T[49][2]={{0,0},
{0xF8,0x8B},{0xF8,0xF2},{0xF9,0x5B},{0xF9,0xB7},{0xFA,0x14},{0xFA,0x66},{0xFA,0xB9},{0xFB,0x03},{0xFB,0x4A},{0xFB,0x8F},{0xFB,0xCF},{0xFC,0x0B},
{0xFC,0x43},{0xFC,0x78},{0xFC,0xAB},{0xFC,0xDB},{0xFD,0x08},{0xFD,0x33},{0xFD,0x5B},{0xFD,0x81},{0xFD,0xA5},{0xFD,0xC7},{0xFD,0xE7},{0xFE,0x05},
{0xFE,0x21},{0xFE,0x3C},{0xFE,0x55},{0xFE,0x6D},{0xFE,0x84},{0xFE,0x99},{0xFE,0xAD},{0xFE,0xC0},{0xFE,0x02},{0xFE,0xE3},{0xFE,0xF3},{0xFF,0x02},
{0xFF,0x10},{0xFF,0x1D},{0xFF,0x2A},{0xFF,0x36},{0xFF,0x42},{0xFF,0x4C},{0xFF,0x56},{0xFF,0x60},{0xFF,0x69},{0xFF,0x71},{0xFF,0x79},{0xFF,0x81}};

uchar code music[][2]={{0,4},
{18,2},{19,2},{21,2},{23,2},{11,4},{26,2},{23,6},{0,2},{18,2},{19,2},{21,2},{23,2},{9,4},{26,2},{23,2},{21,2},{23,2},{19,2},{21,2},{18,2},{19,2},
{14,2},{0,4},{18,2},{19,2},{21,2},{23,2},{11,4},{26,2},{23,6},{0,2},{18,2},{19,2},{21,2},{23,2},{9,4},{26,2},{23,2},{21,2},{23,2},{19,2},{21,2},
{18,2},{19,2},{14,2},{0,4},{9,6},{7,2},{9,6},{7,2},{9,4},{11,4},{14,4},{11,4},{9,6},{7,2},{9,6},{7,2},{9,2},{11,2},{9,2},{7,2},{1,4},{0,4},{9,6},
{7,2},{9,6},{7,2},{9,4},{11,4},{14,4},{11,4},{9,6},{11,2},{9,4},{7,4},{9,8},{0,8},{9,6},{7,2},{9,6},{7,2},{9,4},{11,4},{14,4},{11,4},{9,6},{11,2},
{9,4},{7,4},{3,4},{0,4},{11,2},{9,2},{7,2},{9,2},{7,4},{0,4},{11,2},{9,2},{7,2},{9,2},{7,4},{0,4},{11,2},{9,2},{7,2},{9,2},{7,8},{0,8},{0,4},{7,4},
{9,4},{11,4},{7,4},{16,4},{14,2},{16,6},{0,2},{7,2},{18,4},{16,2},{18,6},{0,4},{18,4},{16,2},{18,6},{11,4},{19,2},{21,2},{19,2},{18,2},{16,4},{14,4},
{16,4},{14,2},{16,4},{14,2},{16,2},{14,2},{16,4},{14,2},{9,4},{14,4},{11,8},{0,8},{7,4},{9,4},{11,4},{7,4},{16,4},{14,2},{16,6},{0,2},{7,2},{18,4},
{16,2},{18,6},{0,4},{18,4},{16,2},{18,6},{11,4},{19,2},{21,2},{19,2},{18,2},{16,4},{14,4},{16,4},{23,2},{23,6},{14,4},{16,4},{23,2},{23,4},{14,4},
{16,2},{16,12},{0,4},{19,4},{21,4},{23,4},{28,2},{26,6},{28,2},{26,6},{28,2},{26,6},{21,4},{23,6},{28,2},{26,6},{28,2},{26,6},{28,2},{26,4},{23,8},
{21,4},{19,2},{16,4},{19,4},{19,2},{21,4},{19,2},{16,4},{19,4},{23,8},{23,4},{21,6},{0,4},{19,4},{21,4},{23,4},{28,2},{26,6},{28,2},{26,6},{28,2},
{26,6},{0,2},{21,2},{23,4},{28,2},{26,6},{28,2},{26,6},{28,2},{26,6},{23,6},{21,4},{19,2},{16,4},{23,4},{21,4},{19,2},{16,4},{16,2},{19,2},{19,8},
{0,4},{16,2},{23,6},{21,4},{19,2},{16,4},{23,4},{21,4},{19,2},{16,4},{16,2},{19,6},{19,8},{0xFF,0xFF}};

void delay(uchar p)
{
    uchar i,j; 
    for(;p>0;p--)
    for(i=185;i>0;i--)
    for(j=185;j>0;j--);
}

void pause()
{
    uchar i,j;
    for(i=150;i>0;i--)
    for(j=150;j>0;j--);
}

void T0_int() interrupt 1
{
 	beep=!beep;
 	TH0=T[m][0]; 
	TL0=T[m][1];
}

void main()
{
 	uchar i=0; 
 	TMOD=0x01;  
	EA=1; 
	ET0=1; 
 	while(1) 
    {
     	m=music[i][0];
		n=music[i][1];
		 
        if(m==0x00)
        {TR0=0;delay(n);i++;} 
        else if(m==0xFF)
        {TR0=0;delay(30);i=0;} 
        else if(m==music[i+1][0]) 
        {TR0=1;delay(n);TR0=0;pause();i++;}
        else
        {TR0=1;delay(n);i++;}
    }
}