#include "ah4-specific.h"
#include "ah4-series.h"
#include "ah4-usbd.h"



int main()
{
	ah4_clock_config();
	ah4_led_init();
	ah4_time_init();

	ah4_i2cm_dis_led_set(1);
	ah4_usb_init();
	ah4_ois_dis_led_set(1);
	
	
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
