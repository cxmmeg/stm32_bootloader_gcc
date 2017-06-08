#include "incs.h"


void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //LED 0
    GPIO_InitStructure.GPIO_Pin  = (1 << Pin_Led0);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_Led0, &GPIO_InitStructure);

    //LED 1
    GPIO_InitStructure.GPIO_Pin  = (1 << Pin_Led1);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_Led1, &GPIO_InitStructure);

    Led0_Off();
    Led1_Off();
}

