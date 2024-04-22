/**
 ****************************************************************************************************
 * @file        stmflash.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-2
 * @brief       STM32�ڲ�FLASH��д ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200402
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/STMFLASH/stmflash.h"


/**
 * @brief       �õ�FLASH�Ĵ���״̬
 * @param       ��
 * @retval      �������
 *   @arg       0   , �޴���
 *   @arg       ����, ������
 */
static uint8_t stmflash_get_error_status(void)
{
    uint32_t res = 0;
    res = FLASH->SR1;

    if (res & (1 << 17)) return 1;  /* WRPERR=1,д�������� */
    if (res & (1 << 18)) return 2;  /* PGSERR=1,������д��� */
    if (res & (1 << 19)) return 3;  /* STRBERR=1,��д���� */
    if (res & (1 << 21)) return 4;  /* INCERR=1,����һ���Դ��� */
    if (res & (1 << 22)) return 5;  /* OPERR=1,д/�������� */
    if (res & (1 << 23)) return 6;  /* RDPERR=1,���������� */
    if (res & (1 << 24)) return 7;  /* RDSERR=1,�Ƿ����ʼ��������� */
    if (res & (1 << 25)) return 8;  /* SNECCERR=1,1bit eccУ������ */
    if (res & (1 << 26)) return 9;  /* DBECCERR=1,2bit ecc���� */

    return 0;   /* û���κ�״̬/�������. */
}

/**
 * @brief       �ȴ��������
 * @param       time : Ҫ��ʱ�ĳ���
 * @retval      �������
 *   @arg       0   : �����
 *   @arg       1~9 : �������
 *   @arg       0XFF: ��ʱ
 */
static uint8_t stmflash_wait_done(uint32_t time)
{
    uint8_t res = 0;
    uint32_t tempreg = 0;

    while (1)
    {
        tempreg = FLASH->SR1;

        if ((tempreg & 0X07) == 0)
        {
            break;  /* BSY=0,WBNE=0,QW=0,�������� */
        }
        
        time--;

        if (time == 0)return 0XFF;
    }

    res = stmflash_get_error_status();

    if (res)
    {
        FLASH->CCR1 = 0X07EE0000;   /* �����д����־ */
    }
    
    return res;
}

/**
 * @brief       ��������0
 * @param       ��
 * @retval      �������
 *   @arg       0   : �����
 *   @arg       1~9 : �������
 *   @arg       0XFF: ��ʱ
 */
static uint8_t stmflash_erase_sector0(void)
{
    uint8_t res = 0;
    res = stmflash_wait_done(0XFFFFFFFF);   /* �ȴ��ϴβ������� */

    if (res == 0)
    {
        FLASH->CR1 &= ~(7 << 8);    /* SNB1[2:0]=0,���ԭ�������� */
        FLASH->CR1 &= ~(3 << 4);    /* PSIZE1[1:0]=0,���ԭ�������� */
        FLASH->CR1 |= (uint32_t)0 << 8; /* ����Ҫ�������������,0 */
        FLASH->CR1 |= 2 << 4;       /* ����Ϊ32bit�� */
        FLASH->CR1 |= 1 << 2;       /* SER1=1,�������� */
        FLASH->CR1 |= 1 << 7;       /* START1=1,��ʼ���� */
        res = stmflash_wait_done(0XFFFFFFFF);   /* �ȴ��������� */
        FLASH->CR1 &= ~(1 << 2);    /* SER1=0,�������������־ */
    }

    return res;
}

/**
 * @brief       ��FLASHָ����ַд8����,��256bit
 *   @note      ������256bitΪ��λ(32�ֽ�)���!!
 * @param       faddr : ָ����ַ(�˵�ַ����Ϊ4�ı���!!)
 * @param       pdata : Ҫд�������
 * @retval      �������
 *   @arg       0   : д��ɹ�
 *   @arg       ����: �������
 */
static uint8_t stmflash_write_8word(uint32_t faddr, uint32_t *pdata)
{
    volatile uint8_t nword = 8; /* ÿ��д8����,256bit */
    uint8_t res;
    res = stmflash_wait_done(0XFFFF);

    if (res == 0)       /* OK */
    {
        FLASH->CR1 &= ~(3 << 4);/* PSIZE1[1:0]=0,���ԭ�������� */
        FLASH->CR1 |= 2 << 4;   /* ����Ϊ32bit�� */
        FLASH->CR1 |= 1 << 1;   /* PG1=1,���ʹ�� */

        __DSB();

        while (nword)
        {
            *(volatile uint32_t *)faddr = *pdata;   /* д������ */
            faddr += 4;         /* д��ַ+4 */
            pdata++;            /* ƫ�Ƶ���һ�������׵�ַ */
            nword--;
        }

        __DSB();                /* д������ɺ�,��������ͬ��,ʹCPU����ִ��ָ������ */
        res = stmflash_wait_done(0XFFFF);   /* �ȴ��������,һ���ֱ��,���100us. */
        FLASH->CR1 &= ~(1 << 1);/* PG1=0,�������������־ */
    }

    return res;
}

/**
 * @brief       ��ȡָ����ַ��һ����(32λ����)
 * @param       faddr : Ҫ��ȡ�ĵ�ַ
 * @retval      ��ȡ��������
 */
uint32_t stmflash_read_word(uint32_t faddr)
{
    return *(volatile uint32_t *)faddr;
}

/**
 * @brief       ��ָ����ַ��ʼд��ָ�����ȵ�����
 *   @note      �ر�ע�⣺��ΪSTM32H750ֻ��һ��������128K��,������ǹ涨��ǰ16K����BootLoader��
 *              ��112K����APP�ã�����Ҫ��д����ԣ�����ʹ��16K�Ժ�ĵ�ַ���������׳�����
 *              ���⣬����д����ʱ��������0XFF�ſ���д�����ݣ���˲��ɱ������Ҫ��������
 *              �����ڲ���ʱ��Ҫ�ȶ�ǰ16K���������ݱ��棨��ȡ��RAM����Ȼ����д�룬�Ա�֤
 *              ǰ16K���ݵ������ԡ���ִ��д�������ʱ�򣬲��ܷ����κ��жϣ�������д��ʱִ
 *              ���ڲ�FLASH���룬�ؽ�����hardfault����
 * @param       waddr : ��ʼ��ַ(�˵�ַ����Ϊ32�ı���!!,����д�����!)
 * @param       pbuf  : ����ָ��
 * @param       length: ��(32λ)��(����Ҫд���32λ���ݵĸ���,һ������д��32�ֽ�,��8����)
 * @retval      ��
 */

/* FLASH д�����ݻ��� */
uint32_t g_flashbuf[BOOT_FLASH_SIZE / 4];

void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length)
{
    uint8_t status = 0;
    uint32_t addrx = 0;
    uint32_t endaddr = 0;
    uint16_t wbfcyc = BOOT_FLASH_SIZE/32;    /* дbootflashbufʱ,��Ҫִ�е�ѭ���� */
    uint32_t *wbfptr;
    uint32_t wbfaddr;

    if (waddr < (STM32_FLASH_BASE + BOOT_FLASH_SIZE))return;    /* д���ַС��STM32_FLASH_BASE+BOOT_FLASH_SIZE,�Ƿ�. */

    if (waddr > (STM32_FLASH_BASE + STM32_FLASH_SIZE))return;   /* д���ַ����STM32��FLASH��ַ��Χ,�Ƿ�. */

    if (waddr % 32)return;          /* д���ַ����32�ֽڱ���,�Ƿ�. */

    HAL_FLASH_Unlock();             /* ���� */
    addrx = waddr;                  /* д�����ʼ��ַ */
    endaddr = waddr + length * 4;   /* д��Ľ�����ַ */

    while (addrx < endaddr)         /* ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���) */
    {
        if (stmflash_read_word(addrx) != 0XFFFFFFFF) /* �з�0XFFFFFFFF�ĵط�,Ҫ����������� */
        {   
            stmflash_read(STM32_FLASH_BASE, g_flashbuf, BOOT_FLASH_SIZE / 4);  /* ����BOOT_FLASH_SIZE��С���� */
            sys_intx_disable(); /* ��ֹ�����ж� */

            status = stmflash_erase_sector0();
            
            if (status) /* ���������� */
            {
                sys_intx_enable();    /* �����ж� */
                break;                /* ���������� */
            }

            SCB_CleanInvalidateDCache();    /* �����Ч��D-Cache */
            wbfptr = g_flashbuf;            /* ָ��g_flashbuf�׵�ַ */
            wbfaddr = STM32_FLASH_BASE;     /* ָ��STM32 FLASH�׵�ַ */

            while (wbfcyc)                  /* д���� */
            {
                if (stmflash_write_8word(wbfaddr, wbfptr))  /* д������ */
                {
                    break;  /* д���쳣 */
                }
                wbfaddr += 32;
                wbfptr += 8;
                wbfcyc--;
            }
            sys_intx_enable();  /* �����ж� */
        }
        else
        {
            addrx += 4;         /* ƫ�Ƶ���һ��λ�� */
        }
    }

    if (status == HAL_OK)
    {
        while (waddr < endaddr) /* д���� */
        {
            if (stmflash_write_8word(waddr, pbuf))  /* д������ */
            {
                break;  /* д���쳣 */
            }

            waddr += 32;
            pbuf += 8;
        }
    }

    HAL_FLASH_Lock();           /* ���� */
}

/**
 * @brief       ��ָ����ַ��ʼ����ָ�����ȵ�����
 * @param       raddr : ��ʼ��ַ
 * @param       pbuf  : ����ָ��
 * @param       length: Ҫ��ȡ����(32)��,��4���ֽڵ�������
 * @retval      ��
 */
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t i;

    for (i = 0; i < length; i++)
    {
        pbuf[i] = stmflash_read_word(raddr);    /* ��ȡ4���ֽ�. */
        raddr += 4; /* ƫ��4���ֽ�. */
    }
}

/******************************************************************************************/
/* �����ô��� */

/**
 * @brief       ����д����(д1����)
 * @param       waddr : ��ʼ��ַ
 * @param       wdata : Ҫд�������
 * @retval      ��ȡ��������
 */
void test_write(uint32_t waddr, uint32_t wdata)
{
    stmflash_write(waddr, &wdata, 1);   /* д��һ���� */
}














