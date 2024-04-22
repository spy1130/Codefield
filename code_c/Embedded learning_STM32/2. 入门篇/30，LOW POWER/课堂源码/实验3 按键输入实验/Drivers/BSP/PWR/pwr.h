#ifndef __PWR_H
#define __PWR_H

#include "./SYSTEM/sys/sys.h"


#define PWR_WKUP_GPIO_PORT              GPIOA
#define PWR_WKUP_GPIO_PIN               GPIO_PIN_0
#define PWR_WKUP_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)
  
#define PWR_WKUP_INT_IRQn               EXTI0_IRQn
#define PWR_WKUP_INT_IRQHandler         EXTI0_IRQHandler


void pwr_wkup_key_init(void);
void pwr_pvd_init(void);


#endif
