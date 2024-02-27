#include "RELAY.h"

void OppenRelay()
{
	P2 = 0xA0;
	P0 = 0x10;
	P2 = 0x00;
}


void CloseRelay()
{
	
	P2 = 0xA0;
	P0 = 0x00;
	P2 = 0x00;
}