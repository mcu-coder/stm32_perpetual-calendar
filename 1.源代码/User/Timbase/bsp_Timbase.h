#ifndef __BSP_TIMEBASE_H
#define __BSP_TIMEBASE_H

#include "stm32f10x.h"

void GENERAL2_TIM_Init(void);
void GENERAL3_TIM_Init(void);
void GENERAL4_TIM_Init(void);
void CH_SR04_Init(void);
void NVIC_Config(void);
void NVIC_Configuration(void);
void RCC_Configuration(void);
void PWM_Init(u16 arr,u16 psc);
#endif
