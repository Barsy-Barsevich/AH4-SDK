#include "ah4-specific.h"


__attribute__((section(".text.ah4specific"))) int ah4_clock_config()
{
	RCC->CTLR |= ((uint32_t)RCC_HSEON);

	/* Wait till HSE is ready and if Time out is reached exit */
	volatile uint32_t hse_status, hse_startup_cnt = 0;
	do
	{
		hse_status = RCC->CTLR & RCC_HSERDY;
		hse_startup_cnt += 1;
  	} while((hse_status == 0) && (hse_startup_cnt != HSE_STARTUP_TIMEOUT));
	if (hse_startup_cnt == HSE_STARTUP_TIMEOUT)
	{
		//errno = ETIMEDOUT;
		return -1;
	}

	if ((RCC->CTLR & RCC_HSERDY) != RESET)
	{
		hse_status = (uint32_t)0x01;
	}
	else
	{
		hse_status = (uint32_t)0x00;
	}

	if (hse_status == 0x1)
	{
		RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;	/* HCLK = SYSCLK */
		RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1;	/* PCLK2 = HCLK */
		RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV2;	/* PCLK1 = HCLK */
		RCC->CFGR2 |= (uint32_t)RCC_PREDIV2_Div4;
		RCC->CFGR2 |= (uint32_t)RCC_PREDIV1_Div2;
		RCC->CFGR2 |= (uint32_t)RCC_PREDIV1_Source_HSE;
		/*  PLL configuration: PLLCLK = HSE / 2(PREDIV1) * 18(PLLMUL) = 144 MHz */
		RCC->CFGR2 |= (uint32_t)RCC_PLL2Mul_4;
		RCC->CFGR0 |= (uint32_t)RCC_PLLMULL18_EXTEN;
		RCC->CTLR |= RCC_PLLON;					/* Enable PLL */
		/* Wait till PLL is ready */
		volatile uint32_t pll_rdy_cnt = 0;
		while((RCC->CTLR & RCC_PLLRDY) == 0 && pll_rdy_cnt < 1000000)
		{
			pll_rdy_cnt += 1;
		}
		/* Select PLL as system clock source */
		RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
		RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;
		/* Wait till PLL is used as system clock source */
		pll_rdy_cnt = 0;
		while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08 && pll_rdy_cnt < 1000000)
		{
			pll_rdy_cnt += 1;
		}
	}
	return 0;
}

void ah4_led_init(void)
{
	RCC_APB2PeriphClockCmd (
		RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,
		ENABLE
	);
	GPIO_InitTypeDef gpio_cfg = {
		.GPIO_Mode = GPIO_Mode_Out_PP,
		.GPIO_Speed = GPIO_Speed_50MHz,
	};
	gpio_cfg.GPIO_Pin = AH4_I2CM_LED_PIN;
	GPIO_Init(AH4_I2CM_LED_PORT, &gpio_cfg);
	gpio_cfg.GPIO_Pin = AH4_OIS_LED_PIN;
	GPIO_Init(AH4_OIS_LED_PORT, &gpio_cfg);
	gpio_cfg.GPIO_Pin = AH4_FSYNC_EN_LED_PIN;
	GPIO_Init(AH4_FSYNC_EN_LED_PORT, &gpio_cfg);
	gpio_cfg.GPIO_Pin = AH4_AP_LED_PIN;
	GPIO_Init(AH4_AP_LED_PORT, &gpio_cfg);
}

void ah4_i2cm_dis_led_set(unsigned level)
{
	GPIO_WriteBit(AH4_I2CM_LED_PORT, AH4_I2CM_LED_PIN, level&1);
}

void ah4_ois_dis_led_set(unsigned level)
{
	GPIO_WriteBit(AH4_OIS_LED_PORT, AH4_OIS_LED_PIN, level&1);
}

void ah4_fsync_en_led_set(unsigned level)
{
	GPIO_WriteBit(AH4_FSYNC_EN_LED_PORT, AH4_FSYNC_EN_LED_PIN, level&1);
}

void ah4_ap_dis_led_set(unsigned level)
{
	GPIO_WriteBit(AH4_AP_LED_PORT, AH4_AP_LED_PIN, level&1);
}

