#include "ah4-specific.h"
#include "system_timer.h"


int main()
{
	ah4_clock_config();
	ah4_led_init();
	system_timer_init();

	while (1)
	{
		ah4_fsync_en_led_set(1);
		system_timer_delay_ms(500);
		ah4_fsync_en_led_set(0);
		system_timer_delay_ms(500);
	}

    return 0;
}
