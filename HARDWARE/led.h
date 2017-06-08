#ifndef __LED_H
#define __LED_H	 

#define 	Pin_WK_UP                   0
#define 	GPIO_WK_UP                  GPIOA

#define 	Pin_Key0                    1
#define 	GPIO_Key0		            GPIOC

#define 	Pin_Key1                    13
#define 	GPIO_Key1		            GPIOC

#define 	Pin_Led0                    8
#define 	GPIO_Led0                   GPIOA
#define 	Led0_On()                	GPIO_Led0->BRR = (1 << Pin_Led0)
#define 	Led0_Off()               	GPIO_Led0->BSRR = (1 << Pin_Led0)
#define     Led0_Inversion()            GPIO_Led0->ODR ^=  (1 << Pin_Led0)

#define 	Pin_Led1                    2
#define 	GPIO_Led1                   GPIOD
#define 	Led1_On()                	GPIO_Led1->BRR = (1 << Pin_Led1)
#define 	Led1_Off()               	GPIO_Led1->BSRR = (1 << Pin_Led1)
#define     Led1_Inversion()            GPIO_Led1->ODR ^=  (1 << Pin_Led1)


void LED_Init(void);//初始化

		 				    
#endif
