#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "ch32v30x.h"

void system_timer_init(void);
uint32_t system_timer_get_time_ticks(void);
uint32_t system_timer_get_time_us(void);
uint32_t system_timer_get_time_ms(void);
void system_timer_delay_us(uint32_t time_us);
void system_timer_delay_ms(uint32_t time_ms);

#if defined(__cplusplus)
}
#endif
