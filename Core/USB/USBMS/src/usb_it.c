#include <stdint.h>
#include "ch32v30x_it.h"
#include "ah4-usbd.h"

__attribute__((naked)) void TIM7_IRQHandler() {
    __asm volatile ("call TIM7_IRQHandler_real; mret");
}

volatile uint32_t micros;
/*********************************************************************
 * @fn      TIM7_IRQHandler
 *
 * @brief   This function handles TIM7 exception.
 *
 * @return  none
 */
__attribute__((used)) void TIM7_IRQHandler_real( void )
{
    /* uart timeout counts */
    Uart.Rx_TimeOut++;
    Uart.USB_Up_TimeOut++;

    /* clear status */
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
}