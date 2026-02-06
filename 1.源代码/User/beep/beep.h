#ifndef  _BEEP_H
#define  _BEEP_H

#include "stm32f10x.h"

#define BEEP_UNABLE                    GPIO_ResetBits(GPIOC, GPIO_Pin_14)   //·äÃùÆ÷Æô¶¯
#define BEEP_ENABLE                    GPIO_SetBits(GPIOC, GPIO_Pin_14)     //·äÃùÆ÷¹Ø±Õ

void BEEP_Init(void);
#endif
