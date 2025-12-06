#include "ah4-specific.h"
#include "ah4-series.h"
#include "ah4-usbd.h"
#include <stdlib.h>

#include "asm-example.h"

static char buffer[1024];

int main()
{
    /* By default, the MCU is clocking by
     * internal 8MHz source. This method sets
     * CPU and peripheral clocks to 144 MHz
     * from external quartz */
	ah4_clock_config();
	/* Sets up system timer. It is necessary
	 * for USB driver */
	ah4_time_init();
    /* USB HS device init */
	ah4_usb_init();
	
	while (1)
	{
		unsigned ans, sw;
		
		strcpy(buffer, "\nTest 1. sw = 0x3243, ans = 0x");
		sw = 0x3243;
		ans = task(&sw);
		itoa(ans, buffer+30, 16);
		//itoa(mysmul(3,2), buffer+30, 16);
		
		strcpy(buffer+strlen(buffer), "\nTest 2. sw = 0x1111, ans = 0x");
		sw = 0x1111;
		ans = task(&sw);
		itoa(ans, buffer+strlen(buffer), 16);
		
		strcpy(buffer+strlen(buffer), "\nTest 3. sw = 0xFFFF, ans = 0x");
		sw = 0xFFFF;
		ans = task(&sw);
		itoa(ans, buffer+strlen(buffer), 16);
		
		ah4_usb_send(buffer, strlen(buffer));
		ah4_time_delay_ms(10000);
	}

    return 0;
}
