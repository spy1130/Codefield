#include <STC15F2K60S2.H>
#include "decoder_138.h"
void close_init()
{
	control_138decoder(4);
	P0=0X00;//นุตฦ
	control_138decoder(0);
	control_138decoder(6);
	P0=0X00;


}