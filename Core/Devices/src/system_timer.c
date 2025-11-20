#include "system_timer.h"

void system_timer_init(void)
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

uint32_t system_timer_get_time_ticks(void)
{
    uint32_t time = SysTick->CNT & 0xFFFFFFFFUL;
    return time;
}

uint32_t system_timer_get_time_us(void)
{
    uint32_t time = SysTick->CNT & 0xFFFFFFFFUL;
    return time >> 1;
}

uint32_t system_timer_get_time_ms(void)
{
    uint32_t time = SysTick->CNT & 0xFFFFFFFFUL;
    return time / 2000UL;
}

void system_timer_delay_us(uint32_t time_us)
{
    uint32_t metka = system_timer_get_time_us();
    while (system_timer_get_time_us() - metka < time_us);
}

void system_timer_delay_ms(uint32_t time_ms)
{
    uint32_t metka = system_timer_get_time_ms();
    while (system_timer_get_time_ms() - metka < time_ms);
}

