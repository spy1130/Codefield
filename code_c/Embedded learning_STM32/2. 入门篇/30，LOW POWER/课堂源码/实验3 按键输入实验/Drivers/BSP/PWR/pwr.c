#include "./BSP/PWR/pwr.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"

void pwr_wkup_key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    PWR_WKUP_GPIO_CLK_ENABLE();                             /* WKUP����ʱ��ʹ�� */

    gpio_init_struct.Pin = PWR_WKUP_GPIO_PIN;               /* WKUP���� */
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;            /* �ж�,������ */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                  /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(PWR_WKUP_GPIO_PORT, &gpio_init_struct);   /* WKUP���ų�ʼ�� */

    HAL_NVIC_SetPriority(PWR_WKUP_INT_IRQn, 2, 2);          /* ��ռ���ȼ�2�������ȼ�2 */
    HAL_NVIC_EnableIRQ(PWR_WKUP_INT_IRQn); 
}

void PWR_WKUP_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(PWR_WKUP_GPIO_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == PWR_WKUP_GPIO_PIN)
    {
        /* HAL_GPIO_EXTI_IRQHandler()�����Ѿ�Ϊ����������жϱ�־λ���������ǽ��˻ص��������Բ����κ��� */
    }
}

void pwr_pvd_init(void)
{
    PWR_PVDTypeDef pwr_pvd_init_struct;
    
    
    /* 1 ʹ��PWRʱ�� */
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* 2 ����PVD */
    pwr_pvd_init_struct.PVDLevel = PWR_PVDLEVEL_7;
    pwr_pvd_init_struct.Mode = PWR_PVD_MODE_IT_RISING_FALLING;
    HAL_PWR_ConfigPVD(&pwr_pvd_init_struct);
    
    /* 3 ʹ��PVD */
    HAL_PWR_EnablePVD();
    
    /* 4 ����PVD�ж����ȼ� */
    HAL_NVIC_SetPriority(PVD_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(PVD_IRQn); 
}

void PVD_IRQHandler(void)
{
    HAL_PWR_PVD_IRQHandler();
}


void HAL_PWR_PVDCallback(void)
{
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_PVDO))  /* ��ѹ��PLS���õĻ��� */
    {
        printf("PVD Low Voltage \r\n");
        LED1(0);
    }
    else
    {
        printf("PVD Voltage OK\r\n");
        LED1(1);
    }
}

