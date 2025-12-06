#include "pl1-specific.h"

void pl1_led_init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef gpio_cfg = {
		.GPIO_Mode = GPIO_Mode_Out_PP,
		.GPIO_Speed = GPIO_Speed_50MHz,
	};
	gpio_cfg.GPIO_Pin = PL1_LED_PIN;
	GPIO_Init(PL1_LED_PORT, &gpio_cfg);
}

void pl1_led_set(unsigned level)
{
	GPIO_WriteBit(PL1_LED_PORT, PL1_LED_PIN, level&1);
}
