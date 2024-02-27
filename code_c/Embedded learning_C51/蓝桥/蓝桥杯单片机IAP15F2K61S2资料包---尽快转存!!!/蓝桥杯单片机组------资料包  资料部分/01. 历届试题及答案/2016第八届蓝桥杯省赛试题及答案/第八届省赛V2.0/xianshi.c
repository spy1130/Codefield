sbit wr = P3^6;
uchar ii,temp;
code unsigned char wei_xuan[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
code unsigned char duan_xuan[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff,0xc6};
void xian_shi(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour/10];break;
		case 1:P0 = duan_xuan[hour%10];break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min/10];break;
		case 4:P0 = duan_xuan[min%10];break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec/10];break;
		case 7:P0 = duan_xuan[sec%10];break;
	}
	i++;
	if(i>=8) i=0;
}
void xian_shi1(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour_i/10] | flash;break;
		case 1:P0 = duan_xuan[hour_i%10] | flash;break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min/10];break;
		case 4:P0 = duan_xuan[min%10];break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec/10];break;
		case 7:P0 = duan_xuan[sec%10];break;
	}
	i++;
	if(i>=8) i=0;
}
void xian_shi2(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour/10];break;
		case 1:P0 = duan_xuan[hour%10];break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min_i/10] | flash;break;
		case 4:P0 = duan_xuan[min_i%10] | flash;break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec/10];break;
		case 7:P0 = duan_xuan[sec%10];break;
	}
	i++;
	if(i>=8) i=0;
}
void xian_shi3(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour/10];break;
		case 1:P0 = duan_xuan[hour%10];break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min/10];break;
		case 4:P0 = duan_xuan[min%10];break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec_i/10] | flash;break;
		case 7:P0 = duan_xuan[sec_i%10] | flash;break;
	}
	i++;
	if(i>=8) i=0;
}
void xian_shi4(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour_n/10] | flash;break;
		case 1:P0 = duan_xuan[hour_n%10] | flash;break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min_n/10];break;
		case 4:P0 = duan_xuan[min_n%10];break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec_n/10];break;
		case 7:P0 = duan_xuan[sec_n%10];break;
	}
	i++;
	if(i>=8) i=0;
}
void xian_shi5(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour_n/10];break;
		case 1:P0 = duan_xuan[hour_n%10];break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min_n/10] | flash;break;
		case 4:P0 = duan_xuan[min_n%10] | flash;break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec_n/10];break;
		case 7:P0 = duan_xuan[sec_n%10];break;
	}
	i++;
	if(i>=8) i=0;
}
void xian_shi6(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[hour_n/10];break;
		case 1:P0 = duan_xuan[hour_n%10];break;
		case 2:P0 = duan_xuan[10];break;
		case 3:P0 = duan_xuan[min_n/10];break;
		case 4:P0 = duan_xuan[min_n%10];break;
		case 5:P0 = duan_xuan[10];break;
		case 6:P0 = duan_xuan[sec_n/10] | flash;break;
		case 7:P0 = duan_xuan[sec_n%10] | flash;break;
	}
	i++;
	if(i>=8) i=0;
}
void xian_shi7(void)
{
	uchar i;
	switch(i)
	{
		case 0:P0 = duan_xuan[11];break;
		case 1:P0 = duan_xuan[11];break;
		case 2:P0 = duan_xuan[11];break;
		case 3:P0 = duan_xuan[11];break;
		case 4:P0 = duan_xuan[11];break;
		case 5:P0 = duan_xuan[temp/10];break;
		case 6:P0 = duan_xuan[temp%10];break;
		case 7:P0 = duan_xuan[12];break;
	}
	i++;
	if(i>=8) i=0;
}
void qie_huan(void)
{
	switch(mode)
	{
		case 0: xian_shi();break; 
		case 1: xian_shi1();break;
		case 2: xian_shi2();break;
		case 3: xian_shi3();break;
		case 4: xian_shi4();break;
		case 5: xian_shi5();break;
		case 6: xian_shi6();break;
		case 7: xian_shi7();break;
	}
}
void fa_song(void)
{
	wr = 0;
	P2 = 0xc0;
	P0 = wei_xuan[ii];
	P2 = 0x00;
	P2 = 0xe0;
	qie_huan();
	P2 = 0x00;
	wr = 1;
	ii++;
	if(ii>=8) ii=0;
}