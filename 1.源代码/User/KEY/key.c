#include "key.h"


void KEY_Init(void) //IO初始化
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;    //校准时间
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
		GPIO_Init(GPIOB,&GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;    //数据减少
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
		GPIO_Init(GPIOB,&GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14;    //数据增加
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
		GPIO_Init(GPIOB,&GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;    //闹钟设定
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
		GPIO_Init(GPIOB,&GPIO_InitStruct);
		
}
