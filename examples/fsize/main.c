#include "ff.h"
#include "sdio.h"
#include "string.h"
#include "ah4-series.h"
#include "pl1-specific.h"
#include "ah4-usbd.h"
#include <stdlib.h>


FATFS fs;
FIL fnew;
UINT fnum;
FRESULT res_sd;
BYTE work[FF_MAX_SS];
u8 WriteBuf[]={"Hello WCH!"};
u8 ReadSDBuf[11]={0};
MKFS_PARM opt = {0};


static char buf[1024];

void print(const char *msg)
{
	strcpy(buf, msg);
	ah4_usb_send(buf, strlen(buf));
}

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
}


int main(void)
{
	ah4_clock_config();
	ah4_time_init();
	ah4_usb_init();
	pl1_led_init();
	ah4_time_delay_ms(3000);
	pl1_led_set(1);
	
    opt.fmt = FM_FAT32;
    opt.n_fat = 1;
    opt.align = 0;
    opt.au_size = 0;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    
	while(SD_Init())
	{
	    print("SD Card Error!\n");
	    ah4_time_delay_ms(100);
	}
	show_sdcard_info();
	print("SD Card OK\n");
	res_sd = f_mount(&fs, "1:", 1);
	if(res_sd == FR_OK)
	{
	    print("Disk mounted successfully\n");
	}
	
    res_sd = f_open(&fnew, "1:/testWCH.txt", FA_OPEN_EXISTING|FA_READ);
    FILINFO fno = {0};
    res_sd = f_stat("1:/testWCH.txt", &fno);
    if (res_sd != FR_OK)
    {
    	print("PODSTAVA ocurred\n");
    	while (1);
    }
    itoa(fno.fsize, buf, 10);
    strcpy(buf+strlen(buf), " <- filesize\n");
    ah4_usb_send(buf, strlen(buf));
    
    while(1);
}







