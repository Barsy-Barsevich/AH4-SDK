#include "ch32v30x.h"
#include <stdint.h>
#include "ah4-specific.h"


int main()
{
	ah4_led_init();
	/*GPIO_InitTypeDef GPIO_InitStructure = { 0 };
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
	GPIO_WriteBit(GPIOC, GPIO_Pin_3, 0);*/

	//clock_config();
	if (ah4_clock_config() == 0)
	{
		//GPIO_WriteBit(GPIOC, GPIO_Pin_2, 1);
		ah4_i2cm_dis_led_set(1);
	}
	else
	{
		ah4_ois_dis_led_set(1);
	}

    return 0;
}
