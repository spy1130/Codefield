#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"


/******************************************************************************************/
/* �������´���, ֧��printf����, ������Ҫѡ��use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)            /* ʹ��AC6������ʱ */
__asm(".global __use_no_semihosting\n\t");  /* ������ʹ�ð�����ģʽ */
__asm(".global __ARM_use_no_argv \n\t");    /* AC6����Ҫ����main����Ϊ�޲�����ʽ�����򲿷����̿��ܳ��ְ�����ģʽ */

#else
/* ʹ��AC5������ʱ, Ҫ�����ﶨ��__FILE �� ��ʹ�ð�����ģʽ */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* ��ʹ�ð�����ģʽ��������Ҫ�ض���_ttywrch\_sys_exit\_sys_command_string����,��ͬʱ����AC6��AC5ģʽ */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* ����_sys_exit()�Ա���ʹ�ð�����ģʽ */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}


/* FILE �� stdio.h���涨��. */
FILE __stdout;

/* MDK����Ҫ�ض���fputc����, printf�������ջ�ͨ������fputc����ַ��������� */
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0);     /* �ȴ���һ���ַ�������� */

    USART1->DR = (uint8_t)ch;             /* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
    return ch;
}
#endif
/******************************************************************************************/

uint8_t g_rx_buffer[1];          /* HAL��ʹ�õĴ��ڽ������ݻ����� */
uint8_t g_usart1_rx_flag = 0;    /* ���ڽ��յ����ݱ�־ */

UART_HandleTypeDef g_uart1_handle;  /* UART��� */

/* ����1��ʼ������ */
void usart_init(uint32_t baudrate)
{
    g_uart1_handle.Instance = USART1;
    g_uart1_handle.Init.BaudRate = baudrate;
    g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;
    g_uart1_handle.Init.StopBits = UART_STOPBITS_1;
    g_uart1_handle.Init.Parity = UART_PARITY_NONE;
    g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    g_uart1_handle.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&g_uart1_handle);
    
    HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t*)g_rx_buffer, 1);
}

/* ����MSP�ص����� */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;
    if(huart->Instance == USART1)                /* ����Ǵ���1�����д���1 MSP��ʼ�� */
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_9;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* ����ʽ������� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* ���� */
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);            /* ��ʼ������1��TX���� */
        
        gpio_init_struct.Pin = GPIO_PIN_10;
        gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;         /* ���� */
        gpio_init_struct.Pull = GPIO_PULLUP;                /* ���� */
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);            /* ��ʼ������1��RX���� */
        
        HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
}

/* ����1�жϷ����� */
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&g_uart1_handle);
    HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t*)g_rx_buffer, 1);
}

/* �������ݽ�����ɻص����� */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        g_usart1_rx_flag = 1;
    }
}



