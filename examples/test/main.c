#include "ah4-specific.h"
#include "ah4-series.h"
#include "UART.h"
#include "ch32v30x_usbhs_device.h"

void usb_config()
{
	RCC_Configuration( );
	// RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	// RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );
	// RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM7, ENABLE );
	// RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE );
	TIM7_Init( );
	// // TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
	// // TIM_DeInit( TIM7 );
	// // /* Time base configuration */
	// // TIM_TimeBaseStructure.TIM_Period = 100 - 1;
	// // TIM_TimeBaseStructure.TIM_Prescaler = 144000000UL / 1000000 - 1;
	// // TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	// // TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// // TIM_TimeBaseInit( TIM7, &TIM_TimeBaseStructure );
	// // /* Clear TIM7 update pending flag */
	// // TIM_ClearFlag( TIM7, TIM_FLAG_Update );
	// // /* TIM IT enable */
	// // TIM_ITConfig( TIM7, TIM_IT_Update, ENABLE );
	// // /* Enable Interrupt */
	// // NVIC_EnableIRQ( TIM7_IRQn );
	// // /* TIM7 enable counter */
	// // TIM_Cmd( TIM7, ENABLE );
	
	UART2_Init( 1, DEF_UARTx_BAUDRATE, DEF_UARTx_STOPBIT, DEF_UARTx_PARITY );
	// // USART_DMACmd( USART2, USART_DMAReq_Rx, DISABLE );
	// // DMA_Cmd( DMA1_Channel6, DISABLE );
	// // DMA_Cmd( DMA1_Channel7, DISABLE );
	// // UART2_CfgInit( DEF_UARTx_BAUDRATE, DEF_UARTx_STOPBIT, DEF_UARTx_PARITY );
	// // UART2_DMAInit( 0, &UART2_Tx_Buf[ 0 ], 0 );
	// // UART2_DMAInit( 1, &UART2_Rx_Buf[ 0 ], DEF_UARTx_RX_BUF_LEN );
	// // USART_DMACmd( USART2, USART_DMAReq_Rx, ENABLE );
	// // UART2_ParaInit( 1 );

	RCC_USBCLK48MConfig( RCC_USBCLK48MCLKSource_USBPHY );
	RCC_USBHSPLLCLKConfig( RCC_HSBHSPLLCLKSource_HSE );
	RCC_USBHSConfig( RCC_USBPLL_Div2 );
	RCC_USBHSPLLCKREFCLKConfig( RCC_USBHSPLLCKREFCLK_8M );
	RCC_USBHSPHYPLLALIVEcmd( ENABLE );
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_USBHS, ENABLE );
	
	USBHSD->CONTROL = USBHS_UC_CLR_ALL | USBHS_UC_RESET_SIE;
	ah4_time_delay_ms(10);
	USBHSD->CONTROL &= ~USBHS_UC_RESET_SIE;
	USBHSD->SPEED_TYPE = 2;
	USBHSD->HOST_CTRL = USBHS_UH_PHY_SUSPENDM;
	USBHSD->CONTROL = USBHS_UC_DMA_EN | USBHS_UC_INT_BUSY | USBHS_UC_SPEED_HIGH;
	USBHSD->INT_EN = USBHS_UIE_SETUP_ACT | USBHS_UIE_TRANSFER | USBHS_UIE_DETECT | USBHS_UIE_SUSPEND;
	USBHS_Device_Endp_Init( );
	// USBHSD->CONTROL |= USBHS_UC_DEV_PU_EN;
	NVIC_EnableIRQ( USBHS_IRQn );
	SetVTFIRQ((uint32_t)USBHS_IRQHandler, USBHS_IRQn, 0, ENABLE);
	USBHSD->CONTROL |= USBHS_UC_DEV_PU_EN;
}

int main()
{
	ah4_clock_config();
	ah4_led_init();
	ah4_time_init();

	ah4_i2cm_dis_led_set(1);
	usb_config();
	ah4_ois_dis_led_set(1);
	
	
	while (1)
	{
		while (USBHSD->RX_LEN == 0);
		uint16_t aN = USBHSD->RX_LEN;
		uint32_t aaa[] = {0x41424344, 0x0A474645};
		aaa[0] = (aN & 0xFF) | 0x41415F00 | 0x30;
		write_usb_data((uint32_t)aaa, 4*2);
		ah4_time_delay_ms(500);
	}

    return 0;
}
