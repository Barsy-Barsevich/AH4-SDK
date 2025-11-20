#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "string.h"
#include "ch32v30x_usb.h"
#include "usb_desc.h"
#include "ah4-series.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************/
/* Macro Definition */

/* General */
#define pUSBHS_SetupReqPak            ((PUSB_SETUP_REQ)USBHS_EP0_Buf)

#define DEF_UEP_IN                    0x80
#define DEF_UEP_OUT                   0x00
/* Endpoint Number */
#define DEF_UEP_BUSY                  0x01
#define DEF_UEP_FREE                  0x00
#define DEF_UEP_NUM                   16
#define DEF_UEP0                      0x00
#define DEF_UEP1                      0x01
#define DEF_UEP2                      0x02
#define DEF_UEP3                      0x03
#define DEF_UEP4                      0x04
#define DEF_UEP5                      0x05
#define DEF_UEP6                      0x06
#define DEF_UEP7                      0x07
#define DEF_UEP8                      0x08
#define DEF_UEP9                      0x09
#define DEF_UEP10                     0x0A
#define DEF_UEP11                     0x0B
#define DEF_UEP12                     0x0C
#define DEF_UEP13                     0x0D
#define DEF_UEP14                     0x0E
#define DEF_UEP15                     0x0F

#define USBHSD_UEP_CFG_BASE           0x40023410
#define USBHSD_UEP_BUF_MOD_BASE       0x40023418
#define USBHSD_UEP_RXDMA_BASE         0x40023420
#define USBHSD_UEP_TXDMA_BASE         0x4002345C
#define USBHSD_UEP_TXLEN_BASE         0x400234DC
#define USBHSD_UEP_TXCTL_BASE         0x400234DE
#define USBHSD_UEP_TX_EN( N )         ( (uint16_t)( 0x01 << N ) )
#define USBHSD_UEP_RX_EN( N )         ( (uint16_t)( 0x01 << ( N + 16 ) ) )
#define USBHSD_UEP_DOUBLE_BUF( N )    ( (uint16_t)( 0x01 << N ) )
#define DEF_UEP_DMA_LOAD              0 /* Direct the DMA address to the data to be processed */
#define DEF_UEP_CPY_LOAD              1 /* Use memcpy to move data to a buffer */
#define USBHSD_UEP_RXDMA( N )         ( *((volatile uint32_t *)( USBHSD_UEP_RXDMA_BASE + ( N - 1 ) * 0x04 ) ) )
#define USBHSD_UEP_RXBUF( N )         ( (uint8_t *)(*((volatile uint32_t *)( USBHSD_UEP_RXDMA_BASE + ( N - 1 ) * 0x04 ) ) ) + 0x20000000 )
#define USBHSD_UEP_TXCTRL( N )        ( *((volatile uint8_t *)( USBHSD_UEP_TXCTL_BASE + ( N - 1 ) * 0x04 ) ) )
#define USBHSD_UEP_TXDMA( N )         ( *((volatile uint32_t *)( USBHSD_UEP_TXDMA_BASE + ( N - 1 ) * 0x04 ) ) )
#define USBHSD_UEP_TXBUF( N )         ( (uint8_t *)(*((volatile uint32_t *)( USBHSD_UEP_TXDMA_BASE + ( N - 1 ) * 0x04 ) ) ) + 0x20000000 )
#define USBHSD_UEP_TLEN( N )          ( *((volatile uint16_t *)( USBHSD_UEP_TXLEN_BASE + ( N - 1 ) * 0x04 ) ) )

// USB SPEED TYPE
#define USBHS_SPEED_TYPE_MASK         ((uint8_t)(0x03))
#define USBHS_SPEED_LOW               ((uint8_t)(0x02))
#define USBHS_SPEED_FULL              ((uint8_t)(0x00))
#define USBHS_SPEED_HIGH              ((uint8_t)(0x01))

/******************************************************************************/
/* Variable Declaration */
/* SetUp Request Values */
extern const uint8_t *pUSBHS_Descr;

/* Setup Request */
extern volatile uint8_t  USBHS_SetupReqCode;
extern volatile uint8_t  USBHS_SetupReqType;
extern volatile uint16_t USBHS_SetupReqValue;
extern volatile uint16_t USBHS_SetupReqIndex;
extern volatile uint16_t USBHS_SetupReqLen;

/* USB Device Status */
extern volatile uint8_t  USBHS_DevConfig;
extern volatile uint8_t  USBHS_DevAddr;
extern volatile uint8_t  USBHS_DevSleepStatus;
extern volatile uint8_t  USBHS_DevEnumStatus;
extern volatile uint16_t USBHS_DevMaxPackLen;

/* Endpoint tx busy flag */
extern volatile uint8_t  USBHS_Endp_Busy[ ];

/* Endpoint Buffer */
extern __attribute__ ((aligned(4))) uint8_t USBHS_EP0_Buf[ ];
extern __attribute__ ((aligned(4))) uint8_t USBHS_EP2_Tx_Buf[ ];
extern __attribute__ ((aligned(4))) uint8_t USBHS_EP2_Rx_Buf[ ];
extern __attribute__ ((aligned(4))) uint8_t USBHS_EP3_Tx_Buf[ ];

/********************************************************************************/
/* Function Declaration */
extern void USBHS_RCC_Init( void );
extern void USBHS_Device_Endp_Init ( void );
extern void USBHS_Device_Init ( FunctionalState sta );
extern void USBHS_Device_SetAddress( uint32_t address );
extern void USBHS_IRQHandler( void );
extern void USBHS_Sleep_WakeUp_Cfg( void );
extern void USBHD_Sleep_Wakeup_Operate( void );
extern uint8_t USBHS_Endp_DataUp( uint8_t endp, uint8_t *pbuf, uint16_t len, uint8_t mod );

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "ch32v30x_conf.h"

/******************************************************************************/
/* Related macro definitions */
/* Serial buffer related definitions */
#define DEF_UARTx_RX_BUF_LEN       ( 4 * 512 )                                  /* Serial x receive buffer size */
#define DEF_UARTx_TX_BUF_LEN       ( 2 * 512 )                                  /* Serial x transmit buffer size */
//#define DEF_USB_FS_PACK_LEN        64                                         /* USB full speed mode packet size for serial x data */
#define DEF_USB_HS_PACK_LEN        DEF_USBD_HS_PACK_SIZE                        /* USB high speed mode packet size for serial x data */
#define DEF_UARTx_TX_BUF_NUM_MAX   ( DEF_UARTx_TX_BUF_LEN / DEF_USB_HS_PACK_LEN ) /* Serial x transmit buffer size */

/* Serial port receive timeout related macro definition */
#define DEF_UARTx_BAUDRATE         115200                                       /* Default baud rate for serial port */
#define DEF_UARTx_STOPBIT          0                                            /* Default stop bit for serial port */
#define DEF_UARTx_PARITY           0                                            /* Default parity bit for serial port */
#define DEF_UARTx_DATABIT          8                                            /* Default data bit for serial port */
#define DEF_UARTx_RX_TIMEOUT       30                                           /* Serial port receive timeout, in 100uS */
#define DEF_UARTx_USB_UP_TIMEOUT   60000                                        /* Serial port receive upload timeout, in 100uS */

/* Serial port transceiver DMA channel related macro definition */
#define DEF_UART2_TX_DMA_CH        DMA1_Channel7                                /* Serial 2 transmit channel DMA channel */
#define DEF_UART2_RX_DMA_CH        DMA1_Channel6                                /* Serial 1 transmit channel DMA channel */

/************************************************************/
/* Serial port X related structure definition */
typedef struct __attribute__((packed)) _UART_CTL
{
    uint16_t Rx_LoadPtr;                                                         /* Serial x data receive buffer load pointer */
    uint16_t Rx_DealPtr;                                                         /* Pointer to serial x data receive buffer processing */
    volatile uint16_t Rx_RemainLen;                                              /* Remaining unprocessed length of the serial x data receive buffer */
    uint8_t  Rx_TimeOut;                                                         /* Serial x data receive timeout */
    uint8_t  Rx_TimeOutMax;                                                      /* Serial x data receive timeout maximum */

    volatile uint16_t Tx_LoadNum;                                                /* Serial x data send buffer load number */
    volatile uint16_t Tx_DealNum;                                                /* Serial x data send buffer processing number */
    volatile uint16_t Tx_RemainNum;                                              /* Serial x data send buffer remaining unprocessed number */
    volatile uint16_t Tx_PackLen[ DEF_UARTx_TX_BUF_NUM_MAX ];                    /* The current packet length of the serial x data send buffer */
    uint8_t  Tx_Flag;                                                            /* Serial x data send status */
    uint8_t  Recv1;
    uint16_t Tx_CurPackLen;                                                      /* The current packet length sent by serial port x */
    uint16_t Tx_CurPackPtr;                                                      /* Pointer to the packet currently being sent by serial port x */

    uint8_t  USB_Up_IngFlag;                                                     /* Serial xUSB packet being uploaded flag */
    uint8_t  Recv2;
    uint16_t USB_Up_TimeOut;                                                     /* Serial xUSB packet upload timeout timer */
    uint8_t  USB_Up_Pack0_Flag;                                                  /* Serial xUSB data needs to upload 0-length packet flag */
    uint8_t  USB_Down_StopFlag;                                                  /* Serial xUSB packet stop down flag */

    uint8_t  Com_Cfg[ 8 ];                                                       /* Serial x parameter configuration (default baud rate is 115200, 1 stop bit, no parity, 8 data bits) */
    uint8_t  Recv3;
    uint8_t  USB_Int_UpFlag;                                                     /* Serial x interrupt upload status */
    uint16_t USB_Int_UpTimeCount;                                                /* Serial x interrupt upload timing */
}UART_CTL, *PUART_CTL;

/***********************************************************************************************************************/
/* Constant, variable extents */
/* The following are serial port transmit and receive related variables and buffers */
extern volatile UART_CTL Uart;                                                    /* Serial x control related structure */
extern volatile uint32_t UARTx_Rx_DMACurCount;                                    /* Serial x receive DMA current count */
extern volatile uint32_t UARTx_Rx_DMALastCount;                                   /* last count of DMA received by serial x */
extern __attribute__ ((aligned(4))) uint8_t UART2_Tx_Buf[ DEF_UARTx_TX_BUF_LEN ]; /* Serial x transmit buffer */
extern __attribute__ ((aligned(4))) uint8_t UART2_Rx_Buf[ DEF_UARTx_RX_BUF_LEN ]; /* Serial x transmit buffer */

/***********************************************************************************************************************/
/* Function extensibility */
extern void TIM7_Init( void );
extern void UART2_CfgInit( uint32_t baudrate, uint8_t stopbits, uint8_t parity ); /* UART1 initialization */
extern void UART2_ParaInit( uint8_t mode );                                       /* Serial port parameter initialization */
extern void UART2_DMAInit( uint8_t type, uint8_t *pbuf, uint32_t len );           /* Serial port 1-related DMA initialization */
extern void UART2_Init( uint8_t mode, uint32_t baudrate, uint8_t stopbits, uint8_t parity ); /* Serial port 1 initialization */
extern void UART2_DataTx_Deal( void );                                            /* Serial port 1 data sending processing  */
extern void UART2_DataRx_Deal( void );                                            /* Serial port 1 data reception processing */
extern void UART2_USB_Init( void );                                               /* USB serial port initialization*/

void ah4_usb_init(void);
void ah4_usb_deinit(void);
void ah4_usb_send(const char *src, uint16_t len);

#if defined(__cplusplus)
}
#endif