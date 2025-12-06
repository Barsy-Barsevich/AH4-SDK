#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "ch32v30x.h"
#include "ch32v30x_rcc.h"
#include "pl1-pindefs.h"

void pl1_led_init(void);
void pl1_led_set(unsigned level);

#if defined(__cplusplus)
}
#endif

