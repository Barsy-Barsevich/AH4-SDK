#include "ah4-specific.h"


int ah4_usb_clock_init(void)
{
	// USB 48 MHz clock from USBPLL
	RCC_USBCLK48MConfig( RCC_USBCLK48MCLKSource_USBPHY );
    // HSPLLSRC: HSE source
    RCC_USBHSPLLCLKConfig( RCC_HSBHSPLLCLKSource_HSE );
	// USBHSPREDIV = 2
	RCC_USBHSConfig( RCC_USBPLL_Div2 );
	// USBPLL reference clock - 8 MHz
	RCC_USBHSPLLCKREFCLKConfig( RCC_USBHSPLLCKREFCLK_8M );
	// Enable USBHS PHY
	RCC_USBHSPHYPLLALIVEcmd( ENABLE );
	// Enable USBHS domain clock
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_USBHS, ENABLE );
}



void ah4_led_init(void)
{
	RCC_APB2PeriphClockCmd (
		RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,
		ENABLE
	);
	GPIO_InitTypeDef gpio_cfg = {
		.GPIO_Mode = GPIO_Mode_Out_PP,
		.GPIO_Speed = GPIO_Speed_50MHz,
	};
	gpio_cfg.GPIO_Pin = AH4_I2CM_LED_PIN;
	GPIO_Init(AH4_I2CM_LED_PORT, &gpio_cfg);
	gpio_cfg.GPIO_Pin = AH4_OIS_LED_PIN;
	GPIO_Init(AH4_OIS_LED_PORT, &gpio_cfg);
	gpio_cfg.GPIO_Pin = AH4_FSYNC_EN_LED_PIN;
	GPIO_Init(AH4_FSYNC_EN_LED_PORT, &gpio_cfg);
	gpio_cfg.GPIO_Pin = AH4_AP_LED_PIN;
	GPIO_Init(AH4_AP_LED_PORT, &gpio_cfg);
}

void ah4_i2cm_dis_led_set(unsigned level)
{
	GPIO_WriteBit(AH4_I2CM_LED_PORT, AH4_I2CM_LED_PIN, level&1);
}

void ah4_ois_dis_led_set(unsigned level)
{
	GPIO_WriteBit(AH4_OIS_LED_PORT, AH4_OIS_LED_PIN, level&1);
}

void ah4_fsync_en_led_set(unsigned level)
{
	GPIO_WriteBit(AH4_FSYNC_EN_LED_PORT, AH4_FSYNC_EN_LED_PIN, level&1);
}

void ah4_ap_dis_led_set(unsigned level)
{
	GPIO_WriteBit(AH4_AP_LED_PORT, AH4_AP_LED_PIN, level&1);
}

