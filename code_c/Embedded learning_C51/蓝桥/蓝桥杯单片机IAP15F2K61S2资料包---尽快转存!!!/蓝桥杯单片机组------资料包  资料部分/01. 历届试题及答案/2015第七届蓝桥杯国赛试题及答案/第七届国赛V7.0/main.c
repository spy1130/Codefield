/******************************************************************************************
ʵ�鹦�ܣ����߽��������������
ʱ�䣺2017.12.28
���ߣ��⿵
*******************************************************************************************/
#include <STC15F2K60S2.H>
#include <delay.h>
#include <iic.h>
#include <ds1302.h>

#define uchar unsigned char  
#define uint unsigned int 

uchar code tab[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar count_1=0,count_2=0,count_3=0,count_4=0,count_5=0;//��ʱ�м����
uchar dsbuff[8]={1,2,3,4,5,6,7,8};											//��ʼ��������ʾ
uchar dma_liang=0,dmi_liang=0;													//�������õ�ѹ������ʱ�������˸��־λ
uchar p_qiehuan=1,c_qiehuan=1;													//Ƶ�ʣ������л���ʾ��־λ,�л���ʾ�¼����Լ������¼��ǵ�ʱ��
uchar shi,fen,miao;																			//�洢��ѹͻ��ʱ��ʱ��
uchar shizhong_flag=0;																	//ʱ����ʾ��־λ
uchar dianya_flag=0;																		//��ѹ��ʾ��־λ
uchar pinlv_flag=0;																			//Ƶ����ʾ��־λ
uchar gongneng_flag=0;																	//�����л�������־λ
uchar jia_flag=0,jian_flag=0;														//�Ӽ�������־λ
uchar chaxun_flag=0;																		//��ѯ������־λ
uchar discount=0;																				//�����ɨ����ʾ����
uchar last_max=0,last_min=0;														//��ѹ�Ƚ�ʱ�ı�־λ
uchar s_keep,f_keep,m_keep;															//�洢��ʱ��ʱ��
uchar thing;																						//�¼���ʾ����
uchar hour=0,minute=0,second=0;													//��־λ
uchar moshi_flag=0;																			//ģʽѡ���־λ
uchar s_liang=0,f_liang=0,m_liang=0;										//ʱ������˸��־λ
uchar dianya_max=20,dianya_min=10;											//��ʼ����ѹ��������
uchar vol_max=0,vol_min=0;															//���õ�ѹ�������ޱ���
uchar jieshu_flag=0;																		//��ʼ��Ƶ��ʱ�ı�־λ
uint zhouqi=0,fre=0,jishu=0;														//���ڣ�Ƶ��
uint voltage=0;																					//�ɼ��ĵ�ѹ
uint count=0;																						//����
uint kk=0;																							//����

/******************************************************************************************
																			��������
*******************************************************************************************/
void display();
void time_chuli();
void chaxun_chuli();
void keyscan();
void Timer0Init();
void Timer1Init();
void Timer2Init();
void dianya_chuli();
void time_shan();
void dianya_shan();
void pinlv_chuli();

/******************************************************************************************
�������ƣ�allinit
���ܣ����ӳ�ʼ��
*******************************************************************************************/
void allinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}

/******************************************************************************************
�������ƣ�main
���ܣ�������
*******************************************************************************************/
void main()
{
	allinit();									//���������ʼ��
	Timer0Init();								//��ʱ��0��ʼ��
	Timer1Init();								//��ʱ��1��ʼ��
	Timer2Init();								//��ʱ��2��ʼ��
	EA=1;												//�����ж�
	ET1=1;											//�򿪶�ʱ��1���ж�
	IE2|=0x04;									//�򿪶�ʱ��2���ж�
	dswrite();									//ʱ�ӳ�ʼ��
	dianya_max=iicread(0x01);		//��ȡ��ַ0x01�������
	Delay2ms();
	dianya_min=iicread(0x02);		//��ȡ��ַ0x02�������
	Delay2ms();
	while(1)
	{
		keyscan();								//����������
		dsread();									//��ȡʱ��
		if(shizhong_flag==1)			
			{
				time_chuli();					//ʱ�䴦����
			}
		if(dianya_flag==1)
			{
				kk++;
				if(kk==1000)   				//ʹ��λ��AD����ʱ�̲��� ����������
					{
						kk=0;
						voltage=adread(0x03)*19.6;//��ȡ��λ���ĵ�ѹ
					}
				dianya_chuli();					//��ѹ������
			}
		if(pinlv_flag==1)
			{  	
				pinlv_chuli();					//Ƶ�ʴ�����
			}
		if(chaxun_flag==1)
			{
				chaxun_chuli();					//��ѯ������
			}
	}
}

/******************************************************************************************
�������ƣ�chaxun_chuli
���ܣ���ѯ����������
*******************************************************************************************/
void chaxun_chuli()
{
	if(c_qiehuan==1)
		{
			dsbuff[0]=11;
			dsbuff[1]=11;
			dsbuff[2]=11;
			dsbuff[3]=11;
			dsbuff[4]=11;
			dsbuff[5]=11;
			dsbuff[6]=thing/10;
			dsbuff[7]=thing%10;	
		}
	else 
		{
			dsbuff[0]=s_keep/10;
			dsbuff[1]=s_keep%10;
			dsbuff[2]=10;
			dsbuff[3]=f_keep/10;
			dsbuff[4]=f_keep%10;
			dsbuff[5]=10;
			dsbuff[6]=m_keep/10;
			dsbuff[7]=m_keep%10;
		}	
}

/******************************************************************************************
�������ƣ�pinlv_chuli
���ܣ�Ƶ�ʼ��㴦����
*******************************************************************************************/
void pinlv_chuli()
{
	if(jieshu_flag==1)						//������s5����ʱ��ʼ����Ƶ��
		{
			jieshu_flag=0;
			count=TH0*256+TL0;
			TH0=TL0=0;								//��Ҫ�������
		}
	if(count>0)
	zhouqi=count*100;							//��������
	fre=1000000/zhouqi;						//����Ƶ��
	if(p_qiehuan==1)							//�жϰ���s4�Ƿ��°��������Ƶ�������л���ʾ
		{
			dsbuff[0]=10;
			dsbuff[1]=2;
			dsbuff[2]=10;
			dsbuff[3]=fre/10000;
			dsbuff[4]=fre/10000%1000;
			dsbuff[5]=fre%1000/100;
			dsbuff[6]=fre%100/10;
			dsbuff[7]=fre%10;	
		}
	else 
		{
			dsbuff[0]=10;
			dsbuff[1]=2;
			dsbuff[2]=10;
			dsbuff[3]=zhouqi/10000;
			dsbuff[4]=zhouqi/10000%1000;
			dsbuff[5]=zhouqi%1000/100;
			dsbuff[6]=zhouqi%100/10;
			dsbuff[7]=zhouqi%10;
		}
}

/******************************************************************************************
�������ƣ�dianya_chuli
���ܣ���ѹ���㴦����
*******************************************************************************************/
void dianya_chuli()
{
	if(gongneng_flag==0) 											//��s4����û�а���ʱ����ʾ��ʱ�ĵ�ѹ
		{
			dsbuff[0]=10;
			dsbuff[1]=1;
			dsbuff[2]=10;
			dsbuff[3]=11;
			dsbuff[4]=voltage/1000;
			dsbuff[5]=voltage%1000/100;
			dsbuff[6]=voltage%100/10;
			dsbuff[7]=voltage%10;
		} 
	else  																		//������s4�����ˣ��ж����Ǹ�ģʽ��
		{
			switch(moshi_flag)
				{
					case 1:vol_max=1; vol_min=0;break;//����������ģʽ��
					case 2:vol_max=0; vol_min=1;break;//����������ģʽ��
					case 3:moshi_flag=1;break;				//�ص�ԭ����ѹ��ʾ����
				}
			if(jia_flag==1)												//������s11�ǰ���
				{
						if(vol_max==1)									//����������ģʽ�����м�
							{
								dianya_max+=5;
								jia_flag=0;
							}
						if(vol_min==1)									//����������ģʽ�����м�
							{
								dianya_min+=5;
								jia_flag=0;
							}	
				 }
			 if(jian_flag==1)											//������s10�ǰ���
				 {
						if(vol_max==1)									//����������ģʽ�����м�
							{
								dianya_max-=5;
								jian_flag=0;
							}
						if(vol_min==1)									//����������ģʽ�����м�
							{
								dianya_min-=5;
								jian_flag=0;
							}	
			 }
			
		} 
	if((voltage>(dianya_max*100))&&(last_max==0))//���бȽ�
		 {
				s_keep=shijian[2];
				f_keep=shijian[1];
				m_keep=shijian[0];
				last_max=1;
				last_min=0;
				iicwrite(0x03,1);											//����ַ��д1
				Delay2ms();
				iicwrite(0x04,s_keep);								//���ոյ���ʱ�䱣���ڵ�ַ0x04��
				Delay2ms();
				iicwrite(0x05,f_keep);								//���ոյķ�ʱ�䱣���ڵ�ַ0x05��
				Delay2ms();
				iicwrite(0x06,m_keep);								//���ոյ�ʱʱ�䱣���ڵ�ַ0x06��
		 }
	else if((voltage<(dianya_min*100))&&(last_min==0))//���бȽ�
		 {
				s_keep=shijian[2];
				f_keep=shijian[1];
				m_keep=shijian[0];
				last_max=0;
				last_min=1;
				iicwrite(0x03,0);											//����ַ��д1
				Delay2ms();
				iicwrite(0x04,s_keep);								//���ոյ���ʱ�䱣���ڵ�ַ0x04��
				Delay2ms();
				iicwrite(0x05,f_keep);								//���ոյķ�ʱ�䱣���ڵ�ַ0x05��
				Delay2ms();
				iicwrite(0x06,m_keep);								//���ոյ�ʱʱ�䱣���ڵ�ַ0x06��
		 }
}

/******************************************************************************************
�������ƣ�keyscan
���ܣ�����������
*******************************************************************************************/
void keyscan()
{
	uchar temp;
	P3=0x7f;P44=0;P42=1;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
		{
			Delay5ms();
			if(temp!=0x0f)
				{
					temp=P3;
					temp=P3&0x0f;
					switch(temp)
						{
							case 0x0e: //ʱ����ʾ����
								if(gongneng_flag==1) gongneng_flag=0; 
								shizhong_flag=1;
								moshi_flag=0;
								dianya_flag=0;
								pinlv_flag=0;
								chaxun_flag=0;
							break;
							case 0x0d://��ѹ��ʾ����
								if(gongneng_flag==1)
									{
										gongneng_flag=0;
										iicwrite(0x01,dianya_max);//�����õĵ�ѹ���ޱ�����0x01��ַ��
										Delay2ms();
										iicwrite(0x02,dianya_min);//�����õĵ�ѹ���ޱ�����0x02��ַ��
									}
								dianya_flag=1;
								shizhong_flag=0;
								pinlv_flag=0;
								moshi_flag=0;
								chaxun_flag=0;
							break;
							case 0x0b://Ƶ����ʾ����
								pinlv_flag=1;
								shizhong_flag=0;
								dianya_flag=0;
								chaxun_flag=0;
								TH0=TL0=0;
							break;
							case 0x07://���ܰ����л�
								if((pinlv_flag!=1)&&(chaxun_flag!=1))
									{
										gongneng_flag=1;
										moshi_flag++;
									}
								else if((pinlv_flag==1)&&(chaxun_flag==0))
									{
										if(p_qiehuan==1) p_qiehuan=0;
										else p_qiehuan=1;
									}	
								else if((pinlv_flag==0)&&(chaxun_flag==1))
									{
										if(c_qiehuan==1) c_qiehuan=0;
										else c_qiehuan=1;
									}
							break;
						} 
					while(P3!=0x7f);
				}
		}
	
	P3=0xbf;P44=1;P42=0;
	temp=P3;
	temp=P3&0x0f;
	if(temp!=0x0f)
		{
			Delay5ms();
			if(temp!=0x0f)
				{
					temp=P3;
					temp=P3&0x0f;
					switch(temp)
						{
							case 0x0e: jia_flag=1;  break;//������
							case 0x0d: jian_flag=1; break;//������
							case 0x0b: 										//��ѯ����
								chaxun_flag=1;
								shizhong_flag=0;
								dianya_flag=0;
								pinlv_flag=0;
								thing=iicread(0x03);
								Delay2ms();
								shi=iicread(0x04);
								Delay2ms();
								fen=iicread(0x05);
								Delay2ms();
								miao=iicread(0x06);
							break;
							case 0x07: break;
						}
				}
			while(P3!=0xbf);
		}

}

/******************************************************************************************
�������ƣ�time1
���ܣ���ʱ��1���жϷ�����
*******************************************************************************************/
void time1() interrupt 3
{
	if((shizhong_flag==1)&&(gongneng_flag==1))//���ñ�־λ������ʱ��ʱ���������һ����������
		{
			time_shan();
		}
	else if((dianya_flag==1)&&(gongneng_flag==1))//���ñ�־λ�����õ�ѹʱ���������һ����������
		{
			dianya_shan();
		}
	
}

/******************************************************************************************
�������ƣ�time2
���ܣ���ʱ��2���жϷ�����
*******************************************************************************************/
void time2() interrupt 12
{
	uchar start;
	display();
	if(pinlv_flag==1)//��ʼ��Ƶ�ʵı�־λ
	{
		jishu++;
		if(jishu==495)
		{
			TR0=1;
			start=1;
		}
		if((start==1)&&(jishu==500))
			{
				TR0=0;
				start=0;
				jieshu_flag=1;
				jishu=0;
			}
	}
}

/******************************************************************************************
�������ƣ�dianya_shan
���ܣ����õ�ѹ������ʱ��˸����
*******************************************************************************************/
void dianya_shan()
{
	if(vol_max==1)
		{
			count_4++;
			if(count_4==200)
				{
					count_4=0;
					if(dma_liang==0)
						{
							dma_liang=1;
							dsbuff[0]=11;
							dsbuff[1]=11;
							dsbuff[2]=0;
							dsbuff[3]=0;
							dsbuff[4]=dianya_min/10;
							dsbuff[5]=dianya_min%10;
							dsbuff[6]=0;
							dsbuff[7]=0;
						}
					else 
						{
							dma_liang=0;
							dsbuff[0]=dianya_max/10;
							dsbuff[1]=dianya_max%10;
							dsbuff[2]=0;
							dsbuff[3]=0;
							dsbuff[4]=dianya_min/10;
							dsbuff[5]=dianya_min%10;
							dsbuff[6]=0;
							dsbuff[7]=0;
						}
				}
		}
	if(vol_min==1)
		{
			count_5++;
			if(count_5==200)
				{
					count_5=0;
					if(dmi_liang==0)
						{
							dmi_liang=1;
							dsbuff[0]=dianya_max/10;
							dsbuff[1]=dianya_max%10;
							dsbuff[2]=0;dsbuff[3]=0;
							dsbuff[4]=11;
							dsbuff[5]=11;
							dsbuff[6]=0;
							dsbuff[7]=0;
						}
					else 
						{
							dmi_liang=0;
							dsbuff[0]=dianya_max/10;
							dsbuff[1]=dianya_max%10;
							dsbuff[2]=0;
							dsbuff[3]=0;
							dsbuff[4]=dianya_min/10;
							dsbuff[5]=dianya_min%10;
							dsbuff[6]=0;
							dsbuff[7]=0;
						}
				}
		}
}

/******************************************************************************************
�������ƣ�display
���ܣ���ʾ������
*******************************************************************************************/
void display()
{
	P2=0xef;
	P0=0xff;
	P2=0x1f;

	P2=0xc0;
	P0=1<<discount;
	P2=0x1f;

	P2=0xef;
  P0=tab[dsbuff[discount]];
	P2=0x1f;

	if(++discount == 8)
	 discount = 0;
}

/******************************************************************************************
�������ƣ�Timer0Init
���ܣ���ʱ��0��ʼ������
*******************************************************************************************/
void Timer0Init(void)		//2΢��@11.0592MHz
{
	AUXR|=0x80;					//��ʱ��ʱ��1Tģʽ
	TMOD&=0xF0;					//���ö�ʱ��ģʽ
	TMOD|=0x04;
	TL0=0x00;						//���ö�ʱ��ֵ
	TH0=0x00;						//���ö�ʱ��ֵ
	TF0=0;							//���TF0��־
}

/******************************************************************************************
�������ƣ�Timer1Init
���ܣ���ʱ��1��ʼ������
*******************************************************************************************/
void Timer1Init(void)		//5����@11.0592MHz
{
	AUXR|=0x40;					//��ʱ��ʱ��1Tģʽ
	TMOD&=0x0F;					//���ö�ʱ��ģʽ
	TL1=0x00;						//���ö�ʱ��ֵ
	TH1=0x28;						//���ö�ʱ��ֵ
	TF1=0;							//���TF1��־
	TR1=1;							//��ʱ��1��ʼ��ʱ
}

/******************************************************************************************
�������ƣ�Timer2Init
���ܣ���ʱ��2��ʼ������
*******************************************************************************************/
void Timer2Init(void)		//2����@11.0592MHz
{
	AUXR|=0x04;					//��ʱ��ʱ��1Tģʽ
	T2L=0x9A;						//���ö�ʱ��ֵ
	T2H=0xA9;						//���ö�ʱ��ֵ
	AUXR|=0x10;					//��ʱ��2��ʼ��ʱ
}

/******************************************************************************************
�������ƣ�time_chuli
���ܣ�ʱ�䴦����
*******************************************************************************************/
void time_chuli()
{
	if(gongneng_flag==0)
			{
				dsbuff[0]=shijian[2]/10;
				dsbuff[1]=shijian[2]%10;
				dsbuff[2]=10;
				dsbuff[3]=shijian[1]/10;
				dsbuff[4]=shijian[1]%10;
				dsbuff[5]=10;
				dsbuff[6]=shijian[0]/10;
				dsbuff[7]=shijian[0]%10;
			}
	else 
			{
				switch(moshi_flag)
					{
						case 1:hour=1;minute=0;second=0;break;
						case 2:minute=1;hour=0;second=0;break;
						case 3:second=1;hour=0;minute=0;break;
						case 4:moshi_flag=1;break;
					}
					if(hour==1)
						{
							if(jia_flag==1)
								{
									if(shijian[2]<=22)
										{
											shijian[2]=shijian[2]+1;
										}
									jia_flag=0;
									dswrite();
								}
							if(jian_flag==1)
								{
									if(shijian[2]>=1)
										{
											shijian[2]-=1;
										}
									jian_flag=0;
									dswrite();
								}	
						}
					else if(minute==1)
						{
							if(jia_flag==1)
								{
									if(shijian[1]<=58)
										shijian[1]+=1;
									jia_flag=0;
									dswrite();
								}
							if(jian_flag==1)
								{
									if(shijian[1]>=1)
									shijian[1]-=1;
									jian_flag=0;
									dswrite();
								}	
						}
					else if(second==1)
						{
							if(jia_flag==1)
								{
									if(shijian[0]<=58)
									shijian[0]+=1;
									jia_flag=0;
									dswrite();
								}
							if(jian_flag==1)
								{
									if(shijian[0]>=1)
									shijian[0]-=1;
									jian_flag=0;
									dswrite();
								}
						}
			}	
}

/******************************************************************************************
�������ƣ�time_shan
���ܣ�����ʱ��ʱ��˸����
*******************************************************************************************/
void time_shan()
{
		if(hour==1)
			{
				count_1++;
				if(count_1==200)
					{
						count_1=0; 
						if(s_liang==0)
							{
								s_liang=1;
								dsbuff[0]=11;
								dsbuff[1]=11;
								dsbuff[2]=10;
								dsbuff[3]=shijian[1]/10;
								dsbuff[4]=shijian[1]%10;
								dsbuff[5]=10;
								dsbuff[6]=shijian[0]/10;
								dsbuff[7]=shijian[0]%10;		
							}
						else 
							{
								s_liang=0; 
								dsbuff[0]=shijian[2]/10;
								dsbuff[1]=shijian[2]%10;
								dsbuff[2]=10;
								dsbuff[3]=shijian[1]/10;
								dsbuff[4]=shijian[1]%10;
								dsbuff[5]=10;
								dsbuff[6]=shijian[0]/10;
								dsbuff[7]=shijian[0]%10;
							}
					}
			}
		else if(minute==1)
			{
				count_2++;
				if(count_2==200)
					{
						count_2=0;
						if(f_liang==0)
							{
								f_liang=1;
								dsbuff[0]=shijian[2]/10;
								dsbuff[1]=shijian[2]%10;
								dsbuff[2]=10;
								dsbuff[3]=11;
								dsbuff[4]=11;
								dsbuff[5]=10;
								dsbuff[6]=shijian[0]/10;
								dsbuff[7]=shijian[0]%10;		
							}
						else 
							{
								f_liang=0;
								dsbuff[0]=shijian[2]/10;
								dsbuff[1]=shijian[2]%10;
								dsbuff[2]=10;
								dsbuff[3]=shijian[1]/10;
								dsbuff[4]=shijian[1]%10;
								dsbuff[5]=10;
								dsbuff[6]=shijian[0]/10;
								dsbuff[7]=shijian[0]%10;
							}
					}
			}
		else if(second==1)
			{
				count_3++;
				if(count_3==200)
					{
						count_3=0;
						if(m_liang==0)
							{
								m_liang=1;
								dsbuff[0]=shijian[2]/10;
								dsbuff[1]=shijian[2]%10;
								dsbuff[2]=10;
								dsbuff[3]=shijian[1]/10;
								dsbuff[4]=shijian[1]%10;
								dsbuff[5]=10;
								dsbuff[6]=11;
								dsbuff[7]=11;		
							}
						else 
							{
								m_liang=0;
								dsbuff[0]=shijian[2]/10;
								dsbuff[1]=shijian[2]%10;
								dsbuff[2]=10;
								dsbuff[3]=shijian[1]/10;
								dsbuff[4]=shijian[1]%10;
								dsbuff[5]=10;
								dsbuff[6]=shijian[0]/10;
								dsbuff[7]=shijian[0]%10;
							}
					}
			}	
}

