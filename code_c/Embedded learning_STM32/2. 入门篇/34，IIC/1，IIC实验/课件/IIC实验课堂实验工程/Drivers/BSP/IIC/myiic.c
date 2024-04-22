#include "./BSP/IIC/myiic.h"
#include "./SYSTEM/delay/delay.h"

void iic_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    IIC_SCL_GPIO_CLK_ENABLE();  /* SCL引脚时钟使能 */
    IIC_SDA_GPIO_CLK_ENABLE();  /* SDA引脚时钟使能 */

    gpio_init_struct.Pin = IIC_SCL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(IIC_SCL_GPIO_PORT, &gpio_init_struct);    /* SCL */

    gpio_init_struct.Pin = IIC_SDA_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* 开漏输出 */
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &gpio_init_struct);    /* SDA */
    /* SDA引脚模式设置,开漏输出,上拉, 这样就不用再设置IO方向了, 开漏输出的时候(=1), 也可以读取外部信号的高低电平 */
}

static void iic_delay(void)
{
    delay_us(2);
}

/* 起始信号 */
void iic_start(void)
{
    /* SCL为高电平期间, SDA从高电平往低电平跳变*/
    IIC_SDA ( 1 );
    IIC_SCL ( 1 );
    iic_delay( );
    IIC_SDA ( 0 );
    iic_delay( );
    IIC_SCL ( 0 );
    iic_delay( );  /* 钳住总线, 准备发送/接收数据 */
}

/* 停止信号 */
void iic_stop(void)
{
    /* SCL为高电平期间, SDA从低电平往高电平跳变*/
    IIC_SDA ( 0 );
    iic_delay( );
    IIC_SCL ( 1 );
    iic_delay( );
    IIC_SDA ( 1 );  /* 发送总线停止信号*/
    iic_delay( );
}

/* 等待应答信号 */
uint8_t iic_wait_ack (void) /* return 1:fail 0:succeed*/
{
    IIC_SDA (1);    /* 主机释放SDA线 */
    iic_delay( );
    IIC_SCL (1);    /* 从机返回ACK*/
    iic_delay( );
    if ( IIC_READ_SDA ) /* SCL高电平读取SDA状态*/ 
    {
        iic_stop();     /* SDA高电平表示从机nack */ 
        return 1;
    }
    IIC_SCL(0);         /* SCL低电平表示结束ACK检查 */ 
    iic_delay( );
    return 0;
}

/* 应答信号 */
void iic_ack(void)
{ 
    IIC_SCL (0);
    iic_delay( );
    IIC_SDA (0);  /* 数据线为低电平，表示应答 */
    iic_delay( );
    IIC_SCL (1);
    iic_delay( );
}

/* 非应答信号 */
void iic_nack(void)
{ 
    IIC_SCL (0);
    iic_delay( );
    IIC_SDA (1);  /* 数据线为低电平，表示应答 */
    iic_delay( );
    IIC_SCL (1);
    iic_delay( );
}

/* 发送一个字节数据 */
void iic_send_byte(uint8_t data)
{
    for (uint8_t t = 0; t < 8; t++)
    {
        /* 高位先发 */
        IIC_SDA((data & 0x80) >> 7);
        iic_delay( );
        IIC_SCL ( 1 );
        iic_delay( );
        IIC_SCL ( 0 );
        data <<= 1;     /* 左移1位, 用于下一次发送 */
    }
    IIC_SDA ( 1 );      /* 发送完成,主机释放SDA线 */ 
}

/* 读取1字节数据 */
uint8_t iic_read_byte (uint8_t ack)
{ 
    uint8_t receive = 0 ;
    for (uint8_t t = 0; t < 8; t++)
    {
        /* 高位先输出，先收到的数据位要左移 */ 
        receive <<= 1;
        IIC_SCL ( 1 );
        iic_delay( );
        if ( IIC_READ_SDA ) receive++;
        IIC_SCL ( 0 );
        iic_delay( );
    }
    if ( !ack ) iic_nack();
    else iic_nack();
    return receive;
}
