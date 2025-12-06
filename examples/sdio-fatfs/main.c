/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2024/11/20
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 * This example is used to show how to migrate an SD card to a FATFS file read/write system.
 *   DVP--PIN:
 *   D0--PC8
 *   D1--PC9
 *   D2--PC10
 *   D3--PC11
 *   SCK--PC12
 *   CMD--PD2
 *   Note: Except for SCK, the rest need to pull up 47K resistors
 *
 */
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

/*********************************************************************
 * @fn      show_sdcard_info
 *
 * @brief   SD Card information.
 *
 * @return  none
 */
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
/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
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
	//printf("SystemClk:%d\r\n",SystemCoreClock);
	//printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
	while(SD_Init())
	{
	    print("SD Card Error!\n");
	    ah4_time_delay_ms(100);
	}
	show_sdcard_info();
	print("SD Card OK\n");
	res_sd=f_mount(&fs, "1:", 1);//SD mount
	if(res_sd==FR_OK)
	{
	    print("Disk mounted successfully\n");
	}
	if(res_sd == FR_NO_FILESYSTEM)
	{
	    print("Disk formatting\n");
	    res_sd=f_mkfs("1:", &opt, work, sizeof(work));
	    if (res_sd == FR_OK)
	    {
	        print("Disk formatted successfully\n");
	        res_sd = f_mount(&fs, "1:", 1);
	        if (res_sd == FR_OK)
	        {
	            print("Disk mounted successfully\n");
	        }
	        else
	        {
	            print("Disk mounting failed\n");
	            //print("error code%x\r\n",res_sd);
	        }
	    }
	    else {
	        print("Disk formatting failed！");//（%d）\r\n", res_sd);
	    }
	}
    res_sd= f_open(&fnew,(const char*)"1:/testWCH.txt",FA_CREATE_ALWAYS|FA_WRITE);
      if(res_sd!=FR_OK)
      {
          print("Create file error\n");
      }
      else
      {
          print("Create file successfully\n");
      }
      print("Writing......................\n");
      res_sd= f_write(&fnew,WriteBuf,11,&fnum);
      for(int j=0;j<sizeof(WriteBuf);j++)
      {
          //printf("WriteBuf[%d]=%d\r\n",j,WriteBuf[j]);
      }
      if(res_sd!=FR_OK)
      {
          print("Write error\n");
      }
      f_close(&fnew);
      res_sd= f_open(&fnew,(const char*)"1:/testWCH.txt",FA_OPEN_EXISTING|FA_READ);
      if(res_sd!=FR_OK)
      {
          print("Open file error\n");
      }
      print("Reading......................\n");
      res_sd= f_read(&fnew,ReadSDBuf,11,(UINT*)&fnum);
      if(res_sd!=FR_OK)
      {
          print("Read error\n");
      }
      for(int i=0;i<sizeof(ReadSDBuf);i++)
      {
          //printf("ReadBuf[%d]=%d\r\n",i,ReadSDBuf[i]) ;
      }
      print("End\n");
    while(1);
}







