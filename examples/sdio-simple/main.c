#include "ah4-series.h"
#include "ah4-usbd.h"
#include "pl1-specific.h"
#include "sdio.h"
#include <stdlib.h>

static char buf[1024];
static u8 wbuf[512];
static u8 Readbuf[512];

void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
	case SDIO_STD_CAPACITY_SD_CARD_V1_1:
		strcpy(buf, "Card Type:SDSC V1.1\r\n");
		ah4_usb_send(buf, strlen(buf));
		break;
	case SDIO_STD_CAPACITY_SD_CARD_V2_0:
		strcpy(buf, "Card Type:SDSC V2.0\r\n");
		ah4_usb_send(buf, strlen(buf));
		break;
	case SDIO_HIGH_CAPACITY_SD_CARD:
		strcpy(buf, "Card Type:SDHC V2.0\r\n");
		ah4_usb_send(buf, strlen(buf));
		break;
	case SDIO_MULTIMEDIA_CARD:
		strcpy(buf, "Card Type:MMC Card\r\n");
		ah4_usb_send(buf, strlen(buf));
		break;
	}
	//s	(buf, "Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);
	strcpy(buf, "Card ManufacturerID: ");
	itoa(SDCardInfo.SD_cid.ManufacturerID, buf+strlen(buf), 10);
	strcpy(buf+strlen(buf), "\nCard RCA: ");
	itoa(SDCardInfo.RCA, buf+strlen(buf), 10);
	strcpy(buf+strlen(buf), "\nCard Capacity: ");
	itoa((u32)(SDCardInfo.CardCapacity>>20), buf+strlen(buf), 10);
	strcpy(buf+strlen(buf), " MB\nCard BlockSize: ");
	itoa(SDCardInfo.CardBlockSize, buf+strlen(buf), 10);
	
	ah4_usb_send(buf, strlen(buf));
	//s	(buf, "Card RCA:%d\r\n",SDCardInfo.RCA);
	//	ah4_usb_send(buf, strlen(buf));
	//s	(buf, "Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));
	//	ah4_usb_send(buf, strlen(buf));
	//s	(buf, "Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);
	//	ah4_usb_send(buf, strlen(buf));
}

int main()
{
	ah4_clock_config();
	/* Sets up system timer. It is necessary
	 * for USB driver */
	ah4_time_init();
	/* USB HS device init */
	ah4_usb_init();
	pl1_led_init();
	pl1_led_set(1);
	
	buf[0] = '\0';
	strcpy(buf, "Hello from PL1R1!\n");
	ah4_usb_send(buf, strlen(buf));
	
	pl1_led_set(1);
	
	while (1)
	{
		while(SD_Init())
		{
			strcpy(buf, "SD Card Error!\n");
			ah4_usb_send(buf, strlen(buf));
			ah4_time_delay_ms(1000);
		}
		show_sdcard_info();
		strcpy(buf, "SD Card OK!\n");
		ah4_usb_send(buf, strlen(buf));
		uint32_t Sector_Nums = ((u32)(SDCardInfo.CardCapacity>>20))/2;
		//s	(buf, "Sector_Nums:0x%08x\n", Sector_Nums);
		ah4_usb_send(buf, strlen(buf));
		
		for(int i=0; i<Sector_Nums; i++)
		{
			if(SD_WriteDisk(wbuf,i,1))
			{
				strcpy(buf, "Wr 0 sector fail\n");
				ah4_usb_send(buf, strlen(buf));
			}
			else
			{
				strcpy(buf, "Wr 0 sector ok\n");
				ah4_usb_send(buf, strlen(buf));
			}
			if(SD_ReadDisk(Readbuf,i,1))
			{
				strcpy(buf, "Rd 0 sector fail\n");
				ah4_usb_send(buf, strlen(buf));
			}
			else
			{
				strcpy(buf, "Rd 0 sector ok\n");
				ah4_usb_send(buf, strlen(buf));
			}
			if(memcmp(wbuf, Readbuf, 512))
			{
				strcpy(buf, " 0 sector Verify fail\n");
				ah4_usb_send(buf, strlen(buf));
			}
			else
			{
				strcpy(buf, "vse ok\n");
				ah4_usb_send(buf, strlen(buf));
			}
		}
		ah4_time_delay_ms(2000);
	}
	
	return 0;
}
