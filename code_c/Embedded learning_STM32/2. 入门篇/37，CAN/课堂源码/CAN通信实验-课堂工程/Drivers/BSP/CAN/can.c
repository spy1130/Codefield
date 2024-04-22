#include "./BSP/CAN/can.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

CAN_HandleTypeDef   g_can1_handler;     /* CAN控制句柄 */
CAN_TxHeaderTypeDef g_can1_txheader;    /* CAN发送结构体 */
CAN_RxHeaderTypeDef g_can1_rxheader;    /* CAN接收结构体 */

/* STM32F103，设TS1=8、TS2=7、BRP=3，波特率 = 36000 / [( 9 + 8 + 1 ) * 4] = 500Kbps */
void can_init(void)
{
    g_can1_handler.Instance  = CAN1;
    g_can1_handler.Init.Mode = CAN_MODE_LOOPBACK;        /* 工作模式设置 环回模式:自发自收 */

    /* 波特率相关 */
    g_can1_handler.Init.Prescaler        = 4;            /* 分频系数 */
    g_can1_handler.Init.TimeSeg1         = CAN_BS1_9TQ;  /* 时间段1 */
    g_can1_handler.Init.TimeSeg2         = CAN_BS2_8TQ;  /* 时间段2 */
    g_can1_handler.Init.SyncJumpWidth    = CAN_SJW_1TQ;  /* 重新同步跳跃宽度 */
    
    /* CAN功能设置 */
    g_can1_handler.Init.AutoBusOff           = DISABLE;  /* 禁止自动离线管理 */
    g_can1_handler.Init.AutoRetransmission   = DISABLE;  /* 禁止自动重发 */
    g_can1_handler.Init.AutoWakeUp           = DISABLE;  /* 禁止自动唤醒 */
    g_can1_handler.Init.ReceiveFifoLocked    = DISABLE;  /* 禁止接收FIFO锁定 */
    g_can1_handler.Init.TimeTriggeredMode    = DISABLE;  /* 禁止时间触发通信模式 */
    g_can1_handler.Init.TransmitFifoPriority = DISABLE;  /* 禁止发送FIFO优先级 */
    
    HAL_CAN_Init(&g_can1_handler);
    
    CAN_FilterTypeDef can_filterconfig;
    /* 过滤器是接收所有报文，不筛选 */
    can_filterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
    
    can_filterconfig.FilterIdHigh = 0;
    can_filterconfig.FilterIdLow = 0;
    can_filterconfig.FilterMaskIdHigh = 0;
    can_filterconfig.FilterMaskIdLow = 0;
    
    can_filterconfig.FilterBank = 0;
    can_filterconfig.FilterFIFOAssignment = CAN_FilterFIFO0;
    can_filterconfig.FilterActivation = CAN_FILTER_ENABLE;
    can_filterconfig.SlaveStartFilterBank = 14;
    HAL_CAN_ConfigFilter(&g_can1_handler, &can_filterconfig);
    
    HAL_CAN_Start(&g_can1_handler);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
    /* 使能时钟 */
    CAN_TX_GPIO_CLK_ENABLE();       /* CAN_TX脚时钟使能 */
    CAN_RX_GPIO_CLK_ENABLE();       /* CAN_RX脚时钟使能 */
    __HAL_RCC_CAN1_CLK_ENABLE();    /* 使能CAN1时钟 */
    
    GPIO_InitTypeDef gpio_init_struct;
    
    gpio_init_struct.Pin    = CAN_TX_GPIO_PIN;          /* 配置CAN的发送引脚 */
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;          /* 复用推挽输出 */
    gpio_init_struct.Pull   = GPIO_PULLUP;              /* 上拉 */
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;     /* 高速 */
    HAL_GPIO_Init(CAN_TX_GPIO_PORT, &gpio_init_struct); /* 初始化CAN TX引脚 */

    gpio_init_struct.Pin    = CAN_RX_GPIO_PIN;          /* 配置CAN的接收引脚 */
    gpio_init_struct.Mode   = GPIO_MODE_AF_INPUT;       /* 复用输入 */
    HAL_GPIO_Init(CAN_RX_GPIO_PORT, &gpio_init_struct); /* 初始化CAN RX引脚 */
}

/* 发送消息数据函数 */
void can_send_message(uint32_t id, uint8_t *buf, uint8_t len)
{
    uint32_t tx_mail = CAN_TX_MAILBOX0;
    
    g_can1_txheader.ExtId = id;
    g_can1_txheader.DLC = len;
    g_can1_txheader.IDE = CAN_ID_EXT;
    g_can1_txheader.RTR = CAN_RTR_DATA;
    
    HAL_CAN_AddTxMessage(&g_can1_handler, &g_can1_txheader, buf, &tx_mail);
    
    while(HAL_CAN_GetTxMailboxesFreeLevel(&g_can1_handler) != 3);
}

/* 接收数据函数 */
uint8_t can_receive_message(uint8_t *buf)
{
    if (HAL_CAN_GetRxFifoFillLevel(&g_can1_handler, CAN_RX_FIFO0) == 0)
    {
        return 0;
    }
    
    HAL_CAN_GetRxMessage(&g_can1_handler, CAN_RX_FIFO0, &g_can1_rxheader, buf);
    
    return g_can1_rxheader.DLC;
}

