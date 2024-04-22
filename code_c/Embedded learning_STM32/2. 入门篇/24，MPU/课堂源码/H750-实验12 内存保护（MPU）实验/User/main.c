/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-22
 * @brief       �ڴ汣��(MPU) ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� Mini Pro H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "stdlib.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/MPU/mpu.h"


#if !(__ARMCC_VERSION >= 6010050)   /* ����AC6����������ʹ��AC5������ʱ */
uint8_t mpudata[128] __attribute__((at(0X20002000)));  /* ����һ������ */
#else
uint8_t mpudata[128] __attribute__((section(".bss.ARM.__at_0X20002000"))); /* ����һ������ */
#endif

int main(void)
{
    uint8_t key = 0;
    uint8_t t = 0; 
    
    sys_cache_enable();                 /* ��L1-Cache */
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(240, 2, 2, 4); /* ����ʱ��, 480Mhz */
    delay_init(480);                    /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    key_init();                         /* ��ʼ������ */
    printf("\r\n\r\nMPU closed!\r\n");  /* ��ʾMPU�ر� */
    mpu_memory_protection();
    
    while (1)
    {
        key = key_scan(0);

        if (key == WKUP_PRES)           /* ʹ��MPU�������� mpudata */
        {
            mpu_set_protection(0X20002000, MPU_REGION_SIZE_128B, MPU_REGION_NUMBER0, MPU_INSTRUCTION_ACCESS_ENABLE, MPU_REGION_PRIV_RO_URO,
                               MPU_ACCESS_NOT_SHAREABLE, MPU_ACCESS_NOT_CACHEABLE, MPU_ACCESS_BUFFERABLE);  /* ֻ��,��ֹ����,��ֹcache,������ */
            
            printf("MPU open!\r\n");    /* ��ʾMPU�� */
        }
        else if (key == KEY0_PRES)      /* ��������д�����ݣ����������MPU�����Ļ�������ڴ���ʴ��� */
        {
            printf("Start Writing data...\r\n");
            sprintf((char *)mpudata, "MPU test array %d", t);
            printf("Data Write finshed!\r\n");
        }
        else if (key == KEY1_PRES)      /* �������ж�ȡ���ݣ�������û�п���MPU��������������ڴ���ʴ��� */
        {
            printf("Array data is:%s\r\n", mpudata);
        }
        else 
        {
            delay_ms(10);
        }

        t++;

        if ((t % 50) == 0) 
        {
            LED0_TOGGLE();      /* LED0ȡ�� */
        }
    }
}









