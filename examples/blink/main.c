#include "ah4-specific.h"
#include "pl1-specific.h"
#include "ah4-series.h"

//#define AH4
#define PL1

int main()
{
	ah4_clock_config();

#ifdef AH4
	ah4_led_init();
#elif defined( PL1 )
	pl1_led_init();
#endif

	ah4_time_init();

	while (1)
	{
#ifdef AH4
		ah4_fsync_en_led_set(1);
#elif defined( PL1 )
		pl1_led_set(1);
#endif
		ah4_time_delay_ms(500);
#ifdef AH4
		ah4_fsync_en_led_set(0);
#elif defined( PL1 )
		pl1_led_set(0);
#endif
		ah4_time_delay_ms(500);
	}

    return 0;
}
