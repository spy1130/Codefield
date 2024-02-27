#ifndef _SYSTEM_H
#define _SYSTEM_H
//���"System.h"�ļ��أ���Ϊ���ж��ǹ�ʽ��������Ϊһ�����������ļ���һ��Ҫ�������еĵ���
//���ͷ�ļ����涨�������е�"GPIO"��λ����������һ������GPIOλ���ļ���
//�������ȥ���ʼǱ��������н���ϸ���͡�

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr&0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
//"addr"�ǡ�λ������ַ����"n"��"bitnum"�ǡ�λ�š�������ѡ�񡰲����ĸ�λ����
//�������ȥ���ʼǱ��������н���ϸ���͡�
#define GPIOA_ODR_Addr (GPIOA_BASE+12) //0x4001080C		//������GPIO_A��"ODR"�Ĵ�����ַ.
#define GPIOB_ODR_Addr (GPIOB_BASE+12) //0x40010C0C		//"+12"�൱��"+0x0c"
#define GPIOC_ODR_Addr (GPIOC_BASE+12) //0x4001100C		//���涼һ����ֻ�����˿ڲ�ͬ
#define GPIOD_ODR_Addr (GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR_Addr (GPIOE_BASE+12) //0x4001180C
#define GPIOF_ODR_Addr (GPIOF_BASE+12) //0x40011A0C
#define GPIOG_ODR_Addr (GPIOG_BASE+12) //0x40011E0C

#define GPIOA_IDR_Addr (GPIOA_BASE+8) //0x40010808		//��������GPIO_A��"IDR"�Ĵ�����ַ
#define GPIOB_IDR_Addr (GPIOB_BASE+8) //0x40010C08		//"+8"�൱��"+0x08"
#define GPIOC_IDR_Addr (GPIOC_BASE+8) //0x40011008		//���涼һ����ֻ�����˿ڲ�ͬ
#define GPIOD_IDR_Addr (GPIOD_BASE+8) //0x40011408
#define GPIOE_IDR_Addr (GPIOE_BASE+8) //0x40011808
#define GPIOF_IDR_Addr (GPIOF_BASE+8) //0x40011A08
#define GPIOG_IDR_Addr (GPIOG_BASE+8) //0x40011E08


#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr,n)//PA��� 
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr,n)//PA����
#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr,n)//PB���
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr,n)//PB����
#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr,n)//PC��� 
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr,n)//PC����
#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr,n)//PD��� 
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr,n)//PD����
#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr,n)//PE��� 
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr,n)//PE����
#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr,n)//PF��� 
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr,n)//PF����
#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr,n)//PG��� 
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr,n)//PG����

#endif
