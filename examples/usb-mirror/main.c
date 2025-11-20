#include "ah4-specific.h"
#include "ah4-series.h"
#include "ah4-usbd.h"

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
		while (USBHSD->RX_LEN == 0);
		uint16_t aN = USBHSD->RX_LEN;
		uint32_t aaa[] = {0x41424344, 0x0A474645};
		aaa[0] = (aN & 0xFF) | 0x41415F00 | 0x30;
		// ah4_usb_send((char*)aaa, 4*2);
		ah4_usb_send((char*)UART2_Tx_Buf, aN);
		ah4_time_delay_ms(500);
	}

    return 0;
}
