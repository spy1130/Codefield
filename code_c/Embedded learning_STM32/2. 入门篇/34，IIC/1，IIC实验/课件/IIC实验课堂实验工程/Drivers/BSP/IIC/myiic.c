#include "./BSP/IIC/myiic.h"
#include "./SYSTEM/delay/delay.h"

void iic_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    IIC_SCL_GPIO_CLK_ENABLE();  /* SCL����ʱ��ʹ�� */
    IIC_SDA_GPIO_CLK_ENABLE();  /* SDA����ʱ��ʹ�� */

    gpio_init_struct.Pin = IIC_SCL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(IIC_SCL_GPIO_PORT, &gpio_init_struct);    /* SCL */

    gpio_init_struct.Pin = IIC_SDA_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* ��©��� */
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &gpio_init_struct);    /* SDA */
    /* SDA����ģʽ����,��©���,����, �����Ͳ���������IO������, ��©�����ʱ��(=1), Ҳ���Զ�ȡ�ⲿ�źŵĸߵ͵�ƽ */
}

static void iic_delay(void)
{
    delay_us(2);
}

/* ��ʼ�ź� */
void iic_start(void)
{
    /* SCLΪ�ߵ�ƽ�ڼ�, SDA�Ӹߵ�ƽ���͵�ƽ����*/
    IIC_SDA ( 1 );
    IIC_SCL ( 1 );
    iic_delay( );
    IIC_SDA ( 0 );
    iic_delay( );
    IIC_SCL ( 0 );
    iic_delay( );  /* ǯס����, ׼������/�������� */
}

/* ֹͣ�ź� */
void iic_stop(void)
{
    /* SCLΪ�ߵ�ƽ�ڼ�, SDA�ӵ͵�ƽ���ߵ�ƽ����*/
    IIC_SDA ( 0 );
    iic_delay( );
    IIC_SCL ( 1 );
    iic_delay( );
    IIC_SDA ( 1 );  /* ��������ֹͣ�ź�*/
    iic_delay( );
}

/* �ȴ�Ӧ���ź� */
uint8_t iic_wait_ack (void) /* return 1:fail 0:succeed*/
{
    IIC_SDA (1);    /* �����ͷ�SDA�� */
    iic_delay( );
    IIC_SCL (1);    /* �ӻ�����ACK*/
    iic_delay( );
    if ( IIC_READ_SDA ) /* SCL�ߵ�ƽ��ȡSDA״̬*/ 
    {
        iic_stop();     /* SDA�ߵ�ƽ��ʾ�ӻ�nack */ 
        return 1;
    }
    IIC_SCL(0);         /* SCL�͵�ƽ��ʾ����ACK��� */ 
    iic_delay( );
    return 0;
}

/* Ӧ���ź� */
void iic_ack(void)
{ 
    IIC_SCL (0);
    iic_delay( );
    IIC_SDA (0);  /* ������Ϊ�͵�ƽ����ʾӦ�� */
    iic_delay( );
    IIC_SCL (1);
    iic_delay( );
}

/* ��Ӧ���ź� */
void iic_nack(void)
{ 
    IIC_SCL (0);
    iic_delay( );
    IIC_SDA (1);  /* ������Ϊ�͵�ƽ����ʾӦ�� */
    iic_delay( );
    IIC_SCL (1);
    iic_delay( );
}

/* ����һ���ֽ����� */
void iic_send_byte(uint8_t data)
{
    for (uint8_t t = 0; t < 8; t++)
    {
        /* ��λ�ȷ� */
        IIC_SDA((data & 0x80) >> 7);
        iic_delay( );
        IIC_SCL ( 1 );
        iic_delay( );
        IIC_SCL ( 0 );
        data <<= 1;     /* ����1λ, ������һ�η��� */
    }
    IIC_SDA ( 1 );      /* �������,�����ͷ�SDA�� */ 
}

/* ��ȡ1�ֽ����� */
uint8_t iic_read_byte (uint8_t ack)
{ 
    uint8_t receive = 0 ;
    for (uint8_t t = 0; t < 8; t++)
    {
        /* ��λ����������յ�������λҪ���� */ 
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
