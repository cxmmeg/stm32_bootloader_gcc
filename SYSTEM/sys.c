#include "incs.h"



void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}

void system_init(void)
{
    //启动各个功能模块时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_AFIO, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); // 关闭JTAG, 引脚作为GPIO使用
    //中断向量表在FLASH(0x08000000)
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
}
