#include "ah4-specific.h"
#include "ah4-series.h"
#include "ah4-usbd.h"
#include "icm45686_barsotion.h"


int main()
{
	ah4_clock_config();
	ah4_led_init();
	ah4_time_init();
	ah4_usb_init();

	while (1)
	{
		ah4_fsync_en_led_set(1);
		ah4_time_delay_ms(500);
		ah4_fsync_en_led_set(0);
		ah4_time_delay_ms(500);
	}
    return 0;
}
