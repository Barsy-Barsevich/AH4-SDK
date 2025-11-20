#include "ah4-series.h"

__attribute__((section(".text.ah4series"))) int ah4_clock_config()
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

void ah4_time_init(void)
{
    uint32_t stk_ctrl = 0;
	stk_ctrl |= (0 << 0); //Turn off the system counter STK and the counter stops counting
	stk_ctrl |= (0 << 1); //Turn off the counter interrupt
	stk_ctrl |= (0 << 2); //HCLK/8 for time base
	stk_ctrl |= (0 << 3); //Continue counting up/down
	stk_ctrl |= (0 << 4); //Counting up
	stk_ctrl |= (1 << 5); //Updated to 0 on up counts, updated to the comparison value on down counts
	stk_ctrl |= (0 << 31); //Turn off the software interrupts
	SysTick->CTLR = stk_ctrl;
	stk_ctrl |= (1 << 0); //Start the system counter STK
	SysTick->CTLR = stk_ctrl;
}

uint32_t ah4_time_get_ticks(void)
{
    uint32_t time = SysTick->CNT & 0xFFFFFFFFUL;
    return time;
}

uint32_t ah4_time_get_us(void)
{
    uint32_t time = SysTick->CNT & 0xFFFFFFFFUL;
    return time >> 1;
}

uint32_t ah4_time_get_ms(void)
{
    uint32_t time = SysTick->CNT & 0xFFFFFFFFUL;
    return time / 2000UL;
}

void ah4_time_delay_us(uint32_t time_us)
{
    uint32_t metka = ah4_time_get_us();
    while (ah4_time_get_us() - metka < time_us);
}

void ah4_time_delay_ms(uint32_t time_ms)
{
    uint32_t metka = ah4_time_get_ms();
    while (ah4_time_get_ms() - metka < time_ms);
}
