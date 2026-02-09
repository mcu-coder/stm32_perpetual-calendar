#include "beep.h"

/********************************************************************************/
void BEEP_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能PORTA,

    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_14;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;      //设置成推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStruct);                //初始化

    BEEP_UNABLE;                                             //防止上电响

}
