#include "./BSP/CAN/can.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

CAN_HandleTypeDef   g_can1_handler;     /* CAN���ƾ�� */
CAN_TxHeaderTypeDef g_can1_txheader;    /* CAN���ͽṹ�� */
CAN_RxHeaderTypeDef g_can1_rxheader;    /* CAN���սṹ�� */

/* STM32F103����TS1=8��TS2=7��BRP=3�������� = 36000 / [( 9 + 8 + 1 ) * 4] = 500Kbps */
void can_init(void)
{
    g_can1_handler.Instance  = CAN1;
    g_can1_handler.Init.Mode = CAN_MODE_LOOPBACK;        /* ����ģʽ���� ����ģʽ:�Է����� */

    /* ��������� */
    g_can1_handler.Init.Prescaler        = 4;            /* ��Ƶϵ�� */
    g_can1_handler.Init.TimeSeg1         = CAN_BS1_9TQ;  /* ʱ���1 */
    g_can1_handler.Init.TimeSeg2         = CAN_BS2_8TQ;  /* ʱ���2 */
    g_can1_handler.Init.SyncJumpWidth    = CAN_SJW_1TQ;  /* ����ͬ����Ծ��� */
    
    /* CAN�������� */
    g_can1_handler.Init.AutoBusOff           = DISABLE;  /* ��ֹ�Զ����߹��� */
    g_can1_handler.Init.AutoRetransmission   = DISABLE;  /* ��ֹ�Զ��ط� */
    g_can1_handler.Init.AutoWakeUp           = DISABLE;  /* ��ֹ�Զ����� */
    g_can1_handler.Init.ReceiveFifoLocked    = DISABLE;  /* ��ֹ����FIFO���� */
    g_can1_handler.Init.TimeTriggeredMode    = DISABLE;  /* ��ֹʱ�䴥��ͨ��ģʽ */
    g_can1_handler.Init.TransmitFifoPriority = DISABLE;  /* ��ֹ����FIFO���ȼ� */
    
    HAL_CAN_Init(&g_can1_handler);
    
    CAN_FilterTypeDef can_filterconfig;
    /* �������ǽ������б��ģ���ɸѡ */
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
    /* ʹ��ʱ�� */
    CAN_TX_GPIO_CLK_ENABLE();       /* CAN_TX��ʱ��ʹ�� */
    CAN_RX_GPIO_CLK_ENABLE();       /* CAN_RX��ʱ��ʹ�� */
    __HAL_RCC_CAN1_CLK_ENABLE();    /* ʹ��CAN1ʱ�� */
    
    GPIO_InitTypeDef gpio_init_struct;
    
    gpio_init_struct.Pin    = CAN_TX_GPIO_PIN;          /* ����CAN�ķ������� */
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;          /* ����������� */
    gpio_init_struct.Pull   = GPIO_PULLUP;              /* ���� */
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;     /* ���� */
    HAL_GPIO_Init(CAN_TX_GPIO_PORT, &gpio_init_struct); /* ��ʼ��CAN TX���� */

    gpio_init_struct.Pin    = CAN_RX_GPIO_PIN;          /* ����CAN�Ľ������� */
    gpio_init_struct.Mode   = GPIO_MODE_AF_INPUT;       /* �������� */
    HAL_GPIO_Init(CAN_RX_GPIO_PORT, &gpio_init_struct); /* ��ʼ��CAN RX���� */
}

/* ������Ϣ���ݺ��� */
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

/* �������ݺ��� */
uint8_t can_receive_message(uint8_t *buf)
{
    if (HAL_CAN_GetRxFifoFillLevel(&g_can1_handler, CAN_RX_FIFO0) == 0)
    {
        return 0;
    }
    
    HAL_CAN_GetRxMessage(&g_can1_handler, CAN_RX_FIFO0, &g_can1_rxheader, buf);
    
    return g_can1_rxheader.DLC;
}

