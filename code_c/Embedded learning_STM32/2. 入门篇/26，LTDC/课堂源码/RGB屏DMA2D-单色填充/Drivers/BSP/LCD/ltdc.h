#ifndef __LTDC_H
#define __LTDC_H

#include "./SYSTEM/sys/sys.h"



/* LCD LTDC重要参数集 */
typedef struct  
{
    uint32_t pwidth;        /* LCD面板的宽度,固定参数,不随显示方向改变,如果为0,说明没有任何RGB屏接入 */
    uint32_t pheight;       /* LCD面板的高度,固定参数,不随显示方向改变 */
    uint16_t hsw;           /* 水平同步宽度 */
    uint16_t vsw;           /* 垂直同步宽度 */
    uint16_t hbp;           /* 水平后廊 */
    uint16_t vbp;           /* 垂直后廊 */
    uint16_t hfp;           /* 水平前廊 */
    uint16_t vfp;           /* 垂直前廊  */
    uint8_t  activelayer;   /* 当前层编号:0/1 */
    uint8_t  dir;           /* 0,竖屏;1,横屏; */
    uint16_t width;         /* LCD宽度 */
    uint16_t height;        /* LCD高度 */
    uint32_t pixsize;       /* 每个像素所占字节数 */
}_ltdc_dev;

extern _ltdc_dev lcdltdc;                  /* 管理LCD LTDC的重要参数 */


/* 函数声明 */
void ltdc_init(void);
void ltdc_layer1_config(void);
void ltdc_draw_point (uint16_t x, uint16_t y, uint32_t color) ;
uint32_t ltdc_read_point (uint16_t x, uint16_t y) ;

#endif

