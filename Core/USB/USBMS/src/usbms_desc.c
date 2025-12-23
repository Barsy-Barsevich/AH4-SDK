#include "usbms_desc.h"

// Device descriptor
const uint8_t MyDevDescr[18] __attribute__((aligned(2))) = {
	18,			 // bLength
	1,			 // bDescriptorType (1=device)
	0x00, 0x02,	 // bcdUSB (usb 2.0)
	2,			 // bDeviceClass (2=massstorage)
	0,			 // bDeviceSubClass
	0,			 // bDeviceProtocol
	8,			 // bMaxPacketSize0
	0xDE, 0x08,	 // idVendor (??? = 08DE)
	0x01, 0x00,	 // idProduct (0001)
	0x00, 0x00,	 // bcdDevice
	1,			 // iManufacturer
	2,			 // iProduct
	3,			 // iSerialNumber
	1,			 // bNumConfigurations
};

// /* Configuration Descriptor (HS) */
// const uint8_t  MyCfgDescr_HS[ ] =
// {
//     /* Configure descriptor */
//     0x09, 0x02, 0x43, 0x00, 0x02, 0x01, 0x00, 0x80, 0x32,
//     /* Interface 0 (CDC) descriptor */
//     0x09, 0x04, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01, 0x00,
//     /* CDC Functional Descriptors */
//     0x05, 0x24, 0x00, 0x10, 0x01,
//     /* Length/management descriptor (data class interface 1) */
//     0x05, 0x24, 0x01, 0x00, 0x01,
//     0x04, 0x24, 0x02, 0x02,
//     0x05, 0x24, 0x06, 0x00, 0x01,
//     /* Interrupt upload endpoint descriptor */
//     0x07, 0x05, 0x83, 0x03, (uint8_t)DEF_USB_EP3_FS_SIZE, (uint8_t)( DEF_USB_EP3_FS_SIZE >> 8 ), 0x01,
//     /* Interface 1 (data interface) descriptor */
//     0x09, 0x04, 0x01, 0x00, 0x02, 0x0a, 0x00, 0x00, 0x00,
//     /* Endpoint descriptor */
//     0x07, 0x05, 0x02, 0x02, (uint8_t)DEF_USB_EP2_HS_SIZE, (uint8_t)( DEF_USB_EP2_HS_SIZE >> 8 ), 0x00,
//     /* Endpoint descriptor */
//     0x07, 0x05, 0x82, 0x02, (uint8_t)DEF_USB_EP2_HS_SIZE, (uint8_t)( DEF_USB_EP2_HS_SIZE >> 8 ), 0x00,
// };

const uint8_t MyCfgDescr_HS[0x20] __attribute__((aligned(2))) = {
	// Configuration descriptor
	0x09,			// bLength
	0x02,			// bDescriptorType
	0x20, 0x00,		// wTotalLength
	1,				// bNumInterfaces
	1,				// bConfigurationValue
	0,				// iConfiguration
	0x80,			// bmAttributes
	250,			// bMaxPower

	// Interface descriptor
	9,				// bLength
	4,				// bDescriptorType
	0,				// bInterfaceNumber
	0,				// bAlternateSetting
	2,				// bNumEndpoints
	0x08,			// bInterfaceClass
	0x05,			// bInterfaceSubClass
	0x50,			// bInterfaceProtocol
	0,				// iInterface

	// EP1 IN endpoint descriptor
	7,				// bLength
	5,				// bDescriptorType
	0x81,			// bEndpointAddress
	2,				// bmAttributes
	0x40, 0x00,		// wMaxPacketSize
	0,				// bInterval

	// EP1 OUT endpoint descriptor
	7,				// bLength
	5,				// bDescriptorType
	0x01,			// bEndpointAddress
	2,				// bmAttributes
	0x40, 0x00,		// wMaxPacketSize
	0,				// bInterval
};


// Must be UTF-16, and the first character is a *manually-calculated* total length
// (combined with a 0x03 string descriptor type)
// Non-latin is allowed, but LANGID is hardcoded to be 0x0409 (English (United States))
// further down in the code.
//const uint16_t MyManuInfo[] = u"\u0326Barsotion KOy Inc.";
//const uint16_t MyProdInfo[] = u"\u0332PL1R1 progressive logger";
// Used for outputting serial number from electronic signature bytes
const uint8_t HEXLUT[16] = "0123456789ABCDEF";

/* Manufacturer Descriptor */
const uint8_t  MyManuInfo[ ] =
{
    0x1C, 0x03,
    'B', 0, 'a', 0, 'r', 0, 's', 0, 'o', 0, 't', 0, 'i', 0, 'o', 0, 'n', 0, ' ', 0, 'K', 0, 'O', 0, 'y', 0
};

/* Product Information */
const uint8_t  MyProdInfo[ ] =
{
//    0x16, 0x03, 'U', 0x00, 'S', 0x00, 'B', 0x00, ' ', 0x00, 'S', 0x00, 'e', 0x00,
//                'r', 0x00, 'i', 0x00, 'a', 0x00, 'l', 0x00
	0x38, 0x03, 'P', 0x00, 'L', 0x00, '1', 0x00, 'R', 0x00, '1', 0x00, 'M', 0x00, '4', 0x00, ' ', 0x00,
	'p', 0x00, 'r', 0x00, 'o', 0x00, 'g', 0x00, 'r', 0x00, 'e', 0x00,
	's', 0x00, 's', 0x00, 'i', 0x00, 'v', 0x00, 'e', 0x00, ' ', 0x00,
	'l', 0x00, 'o', 0x00, 'g', 0x00, 'g', 0x00, 'e', 0x00, 'r', 0x00
};


// SCSI INQUIRY standard response
const uint8_t SCSI_INQUIRY_RESPONSE[] __attribute__((aligned(2))) = {
	0x00,   // direct access block device
	0x80,   // RMB removable media
	// (if you don't set the RMB flag, macOS will show an orange drive
	// instead of a grey drive, contrary to expectations of flash drives)
	0x04,   // SPC-2
	0x02,   // fixed, response data format
	0x1F,   // additional length
	0x00,   // xxx mostly obsolete junk
	0x00,
	0x00,
	// Vendor
	'B', 'a', 'r', 's', 'o', 't', 'i', 'o', 'n',
	// Product
	'P', 'L', '1', ' ', '4', 'G', 'B', ' ', 'l', 'o', 'g', 'g', 'e', 'r', ' ',
	// Revision
	'R', '1', ' ', ' ',
};


/* Language Descriptor */
const uint8_t  MyLangDescr[ ] =
{
    0x04, 0x03, 0x09, 0x04
};

/* Serial Number Information */
const uint8_t  MySerNumInfo[ ] =
{
    0x16, 0x03, '0', 0x00, '1', 0x00, '2', 0x00, '3', 0x00, '4', 0x00, '5', 0x00
              , '6', 0x00, '7', 0x00, '8', 0x00, '9', 0x00
};

/* Device Qualified Descriptor */
const uint8_t MyQuaDesc[ ] =
{
    0x0A, 0x06, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0x40, 0x01, 0x00,
};

/* Device BOS Descriptor */
const uint8_t MyBOSDesc[ ] =
{
    0x05, 0x0F, 0x0C, 0x00, 0x01,
    0x07, 0x10, 0x02, 0x02, 0x00, 0x00, 0x00,
};

/* USB Full-Speed Mode, Other speed configuration Descriptor */
uint8_t TAB_USB_FS_OSC_DESC[ sizeof(MyCfgDescr_HS) ] =
{
    /* Other parts are copied through the program */
    0x09, 0x07,
};

/* USB High-Speed Mode, Other speed configuration Descriptor */
uint8_t TAB_USB_HS_OSC_DESC[ sizeof(MyCfgDescr_HS) ] =
{
    /* Other parts are copied through the program */
    0x09, 0x07,
};
