#ifndef __LTDC_H
#define __LTDC_H

#include "./SYSTEM/sys/sys.h"



/* LCD LTDC��Ҫ������ */
typedef struct  
{
    uint32_t pwidth;        /* LCD���Ŀ��,�̶�����,������ʾ����ı�,���Ϊ0,˵��û���κ�RGB������ */
    uint32_t pheight;       /* LCD���ĸ߶�,�̶�����,������ʾ����ı� */
    uint16_t hsw;           /* ˮƽͬ����� */
    uint16_t vsw;           /* ��ֱͬ����� */
    uint16_t hbp;           /* ˮƽ���� */
    uint16_t vbp;           /* ��ֱ���� */
    uint16_t hfp;           /* ˮƽǰ�� */
    uint16_t vfp;           /* ��ֱǰ��  */
    uint8_t  activelayer;   /* ��ǰ����:0/1 */
    uint8_t  dir;           /* 0,����;1,����; */
    uint16_t width;         /* LCD��� */
    uint16_t height;        /* LCD�߶� */
    uint32_t pixsize;       /* ÿ��������ռ�ֽ��� */
}_ltdc_dev;

extern _ltdc_dev lcdltdc;                  /* ����LCD LTDC����Ҫ���� */


/* �������� */
void ltdc_init(void);
void ltdc_layer1_config(void);
void ltdc_draw_point (uint16_t x, uint16_t y, uint32_t color) ;
uint32_t ltdc_read_point (uint16_t x, uint16_t y) ;

#endif

