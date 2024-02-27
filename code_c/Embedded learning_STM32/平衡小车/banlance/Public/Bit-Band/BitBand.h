#ifndef _SYSTEM_H
#define _SYSTEM_H
//这个"System.h"文件呢，因为其中都是公式，所以作为一个公共调用文件。一定要明白其中的道理。
//这个头文件里面定义了所有的"GPIO"的位，所以这是一个操作GPIO位的文件。
//具体可以去看笔记本，里面有较详细解释。

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr&0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
//"addr"是“位带区地址”，"n"和"bitnum"是“位号”，用于选择“操作哪个位”。
//具体可以去看笔记本，里面有较详细解释。
#define GPIOA_ODR_Addr (GPIOA_BASE+12) //0x4001080C		//这是求GPIO_A的"ODR"寄存器地址.
#define GPIOB_ODR_Addr (GPIOB_BASE+12) //0x40010C0C		//"+12"相当于"+0x0c"
#define GPIOC_ODR_Addr (GPIOC_BASE+12) //0x4001100C		//下面都一样，只不过端口不同
#define GPIOD_ODR_Addr (GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR_Addr (GPIOE_BASE+12) //0x4001180C
#define GPIOF_ODR_Addr (GPIOF_BASE+12) //0x40011A0C
#define GPIOG_ODR_Addr (GPIOG_BASE+12) //0x40011E0C

#define GPIOA_IDR_Addr (GPIOA_BASE+8) //0x40010808		//这是在求GPIO_A的"IDR"寄存器地址
#define GPIOB_IDR_Addr (GPIOB_BASE+8) //0x40010C08		//"+8"相当于"+0x08"
#define GPIOC_IDR_Addr (GPIOC_BASE+8) //0x40011008		//下面都一样，只不过端口不同
#define GPIOD_IDR_Addr (GPIOD_BASE+8) //0x40011408
#define GPIOE_IDR_Addr (GPIOE_BASE+8) //0x40011808
#define GPIOF_IDR_Addr (GPIOF_BASE+8) //0x40011A08
#define GPIOG_IDR_Addr (GPIOG_BASE+8) //0x40011E08


#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr,n)//PA输出 
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr,n)//PA输入
#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr,n)//PB输出
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr,n)//PB输入
#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr,n)//PC输出 
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr,n)//PC输入
#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr,n)//PD输出 
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr,n)//PD输入
#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr,n)//PE输出 
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr,n)//PE输入
#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr,n)//PF输出 
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr,n)//PF输入
#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr,n)//PG输出 
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr,n)//PG输入

#endif
