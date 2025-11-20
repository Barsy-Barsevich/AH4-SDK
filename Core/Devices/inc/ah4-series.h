#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include "stdint.h"
#include "ch32v30x.h"
#include "ch32v30x_rcc.h"

int ah4_clock_config();
void ah4_time_init(void);
uint32_t ah4_time_get_ticks(void);
uint32_t ah4_time_get_us(void);
uint32_t ah4_time_get_ms(void);
void ah4_time_delay_us(uint32_t time_us);
void ah4_time_delay_ms(uint32_t time_ms);

#if defined(__cplusplus)
}
#endif