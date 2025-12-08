#include "ah4-specific.h"
#include "ah4-series.h"
#include "ah4-usbd.h"

static char buffer[50];

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
		while (ah4_usb_seek() == 0);
		uint16_t received = ah4_usb_seek();
		if (received > sizeof(buffer))
		{
			received = sizeof(buffer);
		}
		// ah4_usb_receive(buffer, received);
		memcpy(buffer, UART2_Tx_Buf, received);
		ah4_usb_send(UART2_Tx_Buf, received);
		ah4_time_delay_ms(500);
	}

    return 0;
}
