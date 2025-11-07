#include <stdio.h>
#include "ch32v30x_gpio.h"
#include "system_timer.h"

int main()
{
	SystemCoreClockUpdate();
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_WriteBit(GPIOB, GPIO_Pin_1, 0);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, 0);
    GPIO_WriteBit(GPIOC, GPIO_Pin_3, 0);
    
    //RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;

    system_timer_init();    
    
    while (1)
    {
        GPIO_WriteBit(GPIOA, GPIO_Pin_3, 1);
        system_timer_delay_ms(500);
        GPIO_WriteBit(GPIOA, GPIO_Pin_3, 0);
        system_timer_delay_ms(500);
    }
    
    return 0;
}
