#include "bsp_led.h"

void LED_GPIO_Config(void)     //GPIO相关定义集锦
{
  GPIO_InitTypeDef GPIO_InitStruct;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);//开启GPIO时钟
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;    //温湿度超限指示灯
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;    //闹钟显示灯
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;    //RUN  多功能指示灯
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC,&GPIO_InitStruct);

	Alarm_UNABLE;
	Limit_UNABLE;
	
}

