#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "ch32v30x.h"
#include "ah4-pindefs.h"

int ah4_clock_config(void);
void ah4_led_init(void);
void ah4_i2cm_dis_led_set(unsigned level);
void ah4_ois_dis_led_set(unsigned level);
void ah4_fsync_en_led_set(unsigned level);
void ah4_ap_dis_led_set(unsigned level);

#if defined(__cplusplus)
}
#endif