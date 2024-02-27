#include <STC15F2K60S2.H>

void control_138decoder(unsigned char mode)
{
	switch(mode)
	{
		case 0 :P2=P2&0x1F |0x00;break;
		case 4 :P2=P2&0x1F |0x80;break;
		case 5 :P2=P2&0x1F |0xA0;break;
		case 6 :P2=P2&0x1F |0xC0;break;
		case 7 :P2=P2&0x1F |0xE0;break;
	}
}