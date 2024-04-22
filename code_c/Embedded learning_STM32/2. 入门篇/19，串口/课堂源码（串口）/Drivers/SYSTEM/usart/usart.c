#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"


/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)            /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t");  /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");    /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}


/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* MDK下需要重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0);     /* 等待上一个字符发送完成 */

    USART1->DR = (uint8_t)ch;             /* 将要发送的字符 ch 写入到DR寄存器 */
    return ch;
}
#endif
/******************************************************************************************/

uint8_t g_rx_buffer[1];          /* HAL库使用的串口接收数据缓冲区 */
uint8_t g_usart1_rx_flag = 0;    /* 串口接收到数据标志 */

UART_HandleTypeDef g_uart1_handle;  /* UART句柄 */

/* 串口1初始化函数 */
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

/* 串口MSP回调函数 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;
    if(huart->Instance == USART1)                /* 如果是串口1，进行串口1 MSP初始化 */
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_9;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* 推挽式复用输出 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* 高速 */
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);            /* 初始化串口1的TX引脚 */
        
        gpio_init_struct.Pin = GPIO_PIN_10;
        gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;         /* 输入 */
        gpio_init_struct.Pull = GPIO_PULLUP;                /* 上拉 */
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);            /* 初始化串口1的RX引脚 */
        
        HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
}

/* 串口1中断服务函数 */
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&g_uart1_handle);
    HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t*)g_rx_buffer, 1);
}

/* 串口数据接收完成回调函数 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        g_usart1_rx_flag = 1;
    }
}



