#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"


/* ������ʼ������ */
void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    gpio_init_struct.Pin = GPIO_PIN_2;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);
}

/* ����ɨ�躯�� */
uint8_t key_scan(void)
{
    if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == 0)
    {
        delay_ms(10);   /* ���� */
        if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == 0)
        {
            while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    return 0;   /* ����û�а��� */
}
