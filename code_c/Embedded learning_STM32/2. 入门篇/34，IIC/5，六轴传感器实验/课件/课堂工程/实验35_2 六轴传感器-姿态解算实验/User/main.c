/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-4-20
 * @brief       六轴传感器-姿态解算 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 F429开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SDRAM/sdram.h"
#include "./USMART/usmart.h"
#include "./BSP/SH3001/sh3001.h"
#include "./BSP/SH3001/imu.h"
#include <math.h>



/**
 * @brief       显示角度
 * @param       x, y : 坐标
 * @param       title: 标题
 * @param       angle: 角度
 * @retval      无
 */
void user_show_angle(uint16_t x, uint16_t y, char *title, float angle)
{
    char buf[20];

    sprintf(buf,"%s%3.1fC", title, angle);          /* 格式化输出 */
    lcd_fill(x, y, x + 160, y + 16, WHITE);         /* 清除上次数据(最多显示20个字符,20*8=160) */
    lcd_show_string(x, y, 160, 16, 16, buf, BLUE);  /* 显示字符串 */
}

/**
 * @brief       显示温度
 * @param       x, y : 坐标
 * @param       temp : 温度
 * @retval      无
 */
void user_show_temprate(uint16_t x, uint16_t y, float temp)
{
    char buf[20];

    sprintf(buf," Temp :%2.1fC", temp);             /* 格式化输出 */
    lcd_fill(x, y, x + 160, y + 16, WHITE);         /* 清除上次数据(最多显示20个字符,20*8=160) */
    lcd_show_string(x, y, 160, 16, 16, buf, BLUE);  /* 显示字符串 */
}

/**
 * @brief       串口1发送1个字符
 * @param       c  : 要发送的字符
 * @retval      无 
 */
void usart1_send_char(uint8_t c)
{
    while ((USART_UX->SR & 0X40) == 0);            /* 等待上一个字符发送完成 */

    USART_UX->DR = (uint8_t)c;                     /* 将要发送的字符 ch 写入到DR寄存器 */
}

/**
 * @brief       传送数据给 ANO_TC匿名科创地面站v4.exe
 * @param       fun  : 功能字. 0XA0~0XAF
 * @param       data : 数据缓存区,最多28字节!!
 * @param       len  : data区有效数据个数
 * @retval      无 
 */
void usart1_niming_report(uint8_t fun, uint8_t *data, uint8_t len)
{
    uint8_t send_buf[32];
    uint8_t i;

    if (len > 28)
    {
        return;    /* 最多28字节数据 */
    }

    send_buf[len + 4] = 0;  /* 校验数置零 */
    send_buf[0] = 0XAA;     /* 帧头 */
    send_buf[1] = 0XAA;     /* 帧头 */
    send_buf[2] = fun;      /* 功能字 */
    send_buf[3] = len;      /* 数据长度 */

    for (i = 0; i < len; i++)
    {
        send_buf[4 + i] = data[i];             /* 复制数据 */
    }
    
    for (i = 0; i < len + 4; i++)
    {
        send_buf[len + 4] += send_buf[i];      /* 计算校验和 */
    }
    
    for (i = 0; i < len + 5; i++)
    {
        usart1_send_char(send_buf[i]);         /* 发送数据到串口1 */
    }
}

/**
 * @brief       发送加速度传感器数据和陀螺仪数据
 * @param       aacx,aacy,aacz    : x,y,z三个方向上面的加速度值
 * @param       gyrox,gyroy,gyroz : x,y,z三个方向上面的陀螺仪值
 * @retval      无 
 */
void sh3001_send_data(short aacx, short aacy, short aacz, short gyrox, short gyroy, short gyroz)
{
    uint8_t tbuf[18];
    tbuf[0] = (aacx >> 8) & 0XFF;
    tbuf[1] = aacx & 0XFF;
    tbuf[2] = (aacy >> 8) & 0XFF;
    tbuf[3] = aacy & 0XFF;
    tbuf[4] = (aacz >> 8) & 0XFF;
    tbuf[5] = aacz & 0XFF;
    tbuf[6] = (gyrox >> 8) & 0XFF;
    tbuf[7] = gyrox & 0XFF;
    tbuf[8] = (gyroy >> 8) & 0XFF;
    tbuf[9] = gyroy & 0XFF;
    tbuf[10] = (gyroz >> 8) & 0XFF;
    tbuf[11] = gyroz & 0XFF;
    tbuf[12] = 0;      /* 因为开启MPL后,无法直接读取磁力计数据,所以这里直接屏蔽掉.用0替代. */
    tbuf[13] = 0;
    tbuf[14] = 0;
    tbuf[15] = 0;
    tbuf[16] = 0;
    tbuf[17] = 0;
    usart1_niming_report(0X02, tbuf, 18);             /* 自定义帧,0X02 */
}

/**
 * @brief       通过串口1上报结算后的姿态数据给电脑
 * @param       roll     : 横滚角.单位0.1度。 -9000 -> 9000 对应 -90.00  ->  90.00度
 * @param       pitch    : 俯仰角.单位 0.1度。-18000 -> 18000 对应 -180.00 -> 180.00 度
 * @param       yaw      : 航向角.单位为0.1度 -18000 -> 18000  对应 -180.00 -> 180.00 度
 * @param       prs      : 气压计高度,单位:cm
 * @param       fly_mode : 飞行模式
 * @param       armed    : 锁定状态
 * @retval      无 
 */
void usart1_report_imu(short roll, short pitch, short yaw, int prs, uint8_t fly_mode, uint8_t armed)
{
    uint8_t tbuf[12];
  
    tbuf[0] = (roll >> 8) & 0XFF;
    tbuf[1] = roll & 0XFF;
    tbuf[2] = (pitch >> 8) & 0XFF;
    tbuf[3] = pitch & 0XFF;
    tbuf[4] = (yaw >> 8) & 0XFF;
    tbuf[5] = yaw & 0XFF;
    tbuf[6] = (prs >> 24) & 0XFF;
    tbuf[7] = (prs >> 16) & 0XFF;
    tbuf[8] = (prs >> 8) & 0XFF;
    tbuf[9] = prs & 0XFF;
    tbuf[10] = fly_mode;
    tbuf[11] = armed;
    usart1_niming_report(0X01, tbuf, 12);       /* 状态帧,0X01 */
}

int main(void)
{
    uint8_t t = 0;
    eulerian_angles_t e_angles;
    float temperature;                          /* 温度值 */
    short acc_data[3];                          /* 加速度传感器原始数据 */
    short gyro_data[3];                         /* 陀螺仪原始数据 */
    uint8_t key, report = 1;                    /* 默认开启上报 */
    
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(360, 25, 2, 8);        /* 设置时钟,180Mhz */
    delay_init(180);                            /* 延时初始化 */
    usart_init(500000);                         /* 串口初始化 */
    usmart_dev.init(90);                        /* USMART初始化 */
    led_init();                                 /* 初始化LED */
    sdram_init();                               /* 初始化SDRAM */
    lcd_init();                                 /* 初始化LCD */
    key_init();                                 /* 初始化按键 */

    while (sh3001_init())      /* 检测不到SH3001 */
    {
        lcd_show_string(30, 130, 200, 16, 16, "SH3001 Check Failed!", RED);
        delay_ms(500);
        lcd_show_string(30, 130, 200, 16, 16, "Please Check!       ", RED);
        delay_ms(500);
        LED0_TOGGLE();         /* 红灯闪烁 */
    }

    imu_init();                /* 初始化 IMU */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "SH3001 TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:UPLOAD ON/OFF", RED);
    
    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)
        {
            report =! report;
            if (report)
            {
                lcd_show_string(30, 210, 200, 16, 16, " UPLOAD ON ", RED);
            }
            else
            {
                lcd_show_string(30, 210, 200, 16, 16, " UPLOAD OFF ", RED);
            }
        }

        t++;

        /* 读取传感器数据 */
        sh3001_get_imu_compdata(acc_data, gyro_data);
        
        /* 数据校准 */
        imu_data_calibration(&gyro_data[0], &gyro_data[1], &gyro_data[2],
                             &acc_data[0],  &acc_data[1],  &acc_data[2]);

        /* 获取欧拉角 */
        e_angles = imu_get_eulerian_angles(gyro_data[0], gyro_data[1], gyro_data[2],
                                           acc_data[0],  acc_data[1],  acc_data[2]);

        if (report)
        {
            sh3001_send_data(acc_data[0], acc_data[1], acc_data[2], gyro_data[0], gyro_data[1], gyro_data[2]);  /* 发送加速度+陀螺仪原始数据 */
            usart1_report_imu((int)(e_angles.roll * 100), (int)(e_angles.pitch * 100), (int)(e_angles.yaw * 100), 0, 0, 0);
        }
        
        if ((t % 50) == 0)  /* 0.1秒左右更新一次温度/欧拉角 */
        {        
            temperature = sh3001_get_temperature();    /* 读取温度值 */
            user_show_temprate(30, 130, temperature);
            
            user_show_angle(30, 150, "Pitch :", e_angles.pitch);
            user_show_angle(30, 170, " Roll :", e_angles.roll);
            user_show_angle(30, 190, " Yaw  :", e_angles.yaw); 
             
            printf("\r\nPITCH:%f\r\n", e_angles.pitch);
            printf("ROLL:%f\r\n", e_angles.roll);
            printf("YAW:%f\r\n", e_angles.yaw);

            LED0_TOGGLE();
        }
    }
}

