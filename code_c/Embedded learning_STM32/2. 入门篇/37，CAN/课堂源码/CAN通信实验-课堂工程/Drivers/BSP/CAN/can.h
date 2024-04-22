#ifndef __CAN_H
#define __CAN_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* CAN ���� ���� */
#define CAN_RX_GPIO_PORT                GPIOA
#define CAN_RX_GPIO_PIN                 GPIO_PIN_11
#define CAN_RX_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define CAN_TX_GPIO_PORT                GPIOA
#define CAN_TX_GPIO_PIN                 GPIO_PIN_12
#define CAN_TX_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

/* �������� */
void can_init(void);
uint8_t can_receive_message(uint8_t *buf);
void can_send_message(uint32_t id, uint8_t *buf, uint8_t len);

#endif

















