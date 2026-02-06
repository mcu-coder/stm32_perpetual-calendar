#ifndef __BSP_LED_H
#define __BSP_LED_H 


#include "stm32f10x.h"

//*****************STM32推挽输出上电默认电平为低电平,所以动作时应为高电平*****************//
#define RUN_Indicate_ENABLE            GPIO_SetBits(GPIOC, GPIO_Pin_13)     //多功能运行指示灯关
#define RUN_Indicate_UNABLE            GPIO_ResetBits(GPIOC, GPIO_Pin_13)   //多功能运行指示灯开

#define Limit_ENABLE                   GPIO_ResetBits(GPIOA, GPIO_Pin_9)   //温湿度超限报警开
#define Limit_UNABLE                   GPIO_SetBits(GPIOA, GPIO_Pin_9)     //温湿度超限报警关

#define Alarm_ENABLE                   GPIO_ResetBits(GPIOA, GPIO_Pin_8)   //闹钟指示灯开
#define Alarm_UNABLE                   GPIO_SetBits(GPIOA, GPIO_Pin_8)     //闹钟指示灯关


void LED_GPIO_Config(void);

#endif /*__BSP_LED_H */

