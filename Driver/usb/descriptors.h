/*
 * Descriptors.h
 *
 *  Created on: 16 РјР°СЂС‚Р° 2017 Рі.
 *      Author: hudienko_a
 */

#ifndef DRIVER_DESCRIPTORS_H_
#define DRIVER_DESCRIPTORS_H_

#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define VIRTUAL_COM_PORT_DATA_SIZE              64
#define VIRTUAL_COM_PORT_INT_SIZE               8

#define VIRTUAL_COM_PORT_SIZ_DEVICE_DESC        18
#define VIRTUAL_COM_PORT_SIZ_CONFIG_DESC        67
#define VIRTUAL_COM_PORT_SIZ_STRING_LANGID      4
#define VIRTUAL_COM_PORT_SIZ_STRING_VENDOR      38
#define VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT     50
#define VIRTUAL_COM_PORT_SIZ_STRING_SERIAL      26

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

uint8_t _Virtual_Com_Port_Device_Descriptor[] =  {
		    0x12,   /* bLength */
		    USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */
		    0x00,
		    0x02,   /* bcdUSB = 2.00 */
		    0x02,   /* bDeviceClass: CDC */
		    0x00,   /* bDeviceSubClass */
		    0x00,   /* bDeviceProtocol */
		    0x40,   /* bMaxPacketSize0 */
		    0x83,
		    0x04,   /* idVendor = 0x0483 */
		    0x40,
		    0x57,   /* idProduct = 0x7540 */
		    0x01,
		    0x00,   /* bcdDevice = 1.00 */
		    0x00,              /* Index of string descriptor describing manufacturer */
		    0x01,              /* Index of string descriptor describing product */
		    0x03,              /* Index of string descriptor describing the device's serial number */
		    0x01    /* bNumConfigurations */
		  };

	uint8_t _Virtual_Com_Port_ConfigDescriptor[] =
	  {
	    /*Configuration Descriptor*/
	    0x09,   /* bLength: Configuration Descriptor size */
	    USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
	    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC,       /* wTotalLength:no of returned bytes */
	    0x00,
	    0x02,   /* bNumInterfaces: 2 interface */
	    0x01,   /* bConfigurationValue: Configuration value */
	    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
	    0xC0,   /* bmAttributes: self powered */
	    0x32,   /* MaxPower 100 mA */
	    /*Interface Descriptor*/
	    0x09,   /* bLength: Interface Descriptor size */
	    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
	    /* Interface descriptor type */
	    0x00,   /* bInterfaceNumber: Number of Interface */
	    0x00,   /* bAlternateSetting: Alternate setting */
	    0x01,   /* bNumEndpoints: One endpoints used */
	    0x02,   /* bInterfaceClass: Communication Interface Class */
	    0x02,   /* bInterfaceSubClass: Abstract Control Model */
	    0x01,   /* bInterfaceProtocol: Common AT commands */
	    0x00,   /* iInterface: */
	    /*Header Functional Descriptor*/
	    0x05,   /* bLength: Endpoint Descriptor size */
	    0x24,   /* bDescriptorType: CS_INTERFACE */
	    0x00,   /* bDescriptorSubtype: Header Func Desc */
	    0x10,   /* bcdCDC: spec release number */
	    0x01,
	    /*Call Management Functional Descriptor*/
	    0x05,   /* bFunctionLength */
	    0x24,   /* bDescriptorType: CS_INTERFACE */
	    0x01,   /* bDescriptorSubtype: Call Management Func Desc */
	    0x00,   /* bmCapabilities: D0+D1 */
	    0x01,   /* bDataInterface: 1 */
	    /*ACM Functional Descriptor*/
	    0x04,   /* bFunctionLength */
	    0x24,   /* bDescriptorType: CS_INTERFACE */
	    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
	    0x02,   /* bmCapabilities */
	    /*Union Functional Descriptor*/
	    0x05,   /* bFunctionLength */
	    0x24,   /* bDescriptorType: CS_INTERFACE */
	    0x06,   /* bDescriptorSubtype: Union func desc */
	    0x00,   /* bMasterInterface: Communication class interface */
	    0x01,   /* bSlaveInterface0: Data Class Interface */


	    /*Endpoint 2 Descriptor*/
	    0x07,   /* bLength: Endpoint Descriptor size */
	    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
	    0x82,   /* bEndpointAddress: (IN2) */
	    0x03,   /* bmAttributes: Interrupt */
	    VIRTUAL_COM_PORT_INT_SIZE,      /* wMaxPacketSize: */
	    0x00,
	    0xFF,   /* bInterval: */

	    /*Data class interface descriptor*/
	    0x09,   /* bLength: Endpoint Descriptor size */
	    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */
	    0x01,   /* bInterfaceNumber: Number of Interface */
	    0x00,   /* bAlternateSetting: Alternate setting */
	    0x02,   /* bNumEndpoints: Two endpoints used */
	    0x0A,   /* bInterfaceClass: CDC */
	    0x00,   /* bInterfaceSubClass: */
	    0x00,   /* bInterfaceProtocol: */
	    0x00,   /* iInterface: */

	    /*Endpoint 3 Descriptor*/
	    0x07,   /* bLength: Endpoint Descriptor size */
	    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
	    0x03,   /* bEndpointAddress: (OUT3) */
	    0x02,   /* bmAttributes: Bulk */
	    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */
	    0x00,
	    0x00,   /* bInterval: ignore for Bulk transfer */

	    /*Endpoint 1 Descriptor*/
	    0x07,   /* bLength: Endpoint Descriptor size */
	    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
	    0x81,   /* bEndpointAddress: (IN1) */
	    0x02,   /* bmAttributes: Bulk */
	    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */
	    0x00,
	    0x00    /* bInterval */
	  };

	const uint8_t Virtual_Com_Port_StringLangID[VIRTUAL_COM_PORT_SIZ_STRING_LANGID] =
	  {
	    VIRTUAL_COM_PORT_SIZ_STRING_LANGID,
	    USB_STRING_DESCRIPTOR_TYPE,
	    0x09,
	    0x04 /* LangID = 0x0409: U.S. English */
	  };

	const uint8_t Virtual_Com_Port_StringVendor[VIRTUAL_COM_PORT_SIZ_STRING_VENDOR] =
	  {
	    VIRTUAL_COM_PORT_SIZ_STRING_VENDOR,     /* Size of Vendor string */
	    USB_STRING_DESCRIPTOR_TYPE,             /* bDescriptorType*/
	    /* Manufacturer: "STMicroelectronics" */
	    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
	    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
	    'c', 0, 's', 0
	  };

	const uint8_t Virtual_Com_Port_StringProduct[VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT] =
	  {
	    VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT,          /* bLength */
	    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
	    /* Product name: "STM32 Virtual COM Port" */
	    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'V', 0, 'i', 0,
	    'r', 0, 't', 0, 'u', 0, 'a', 0, 'l', 0, ' ', 0, 'C', 0, 'O', 0,
	    'M', 0, ' ', 0, 'P', 0, 'o', 0, 'r', 0, 't', 0, ' ', 0, ' ', 0
	  };

	uint8_t Virtual_Com_Port_StringSerial[VIRTUAL_COM_PORT_SIZ_STRING_SERIAL] =
	  {
	    VIRTUAL_COM_PORT_SIZ_STRING_SERIAL,           /* bLength */
	    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
	    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
	  };


#define SomeDev_SIZ_DEVICE_DESC         	     18
#define SomeDev_SIZ_CONFIG_DESC          	     32

#define SomeDev_SIZ_STRING_LANGID       	     4
#define SomeDev_SIZ_STRING_VENDOR       	     22
#define SomeDev_SIZ_STRING_PRODUCT       		 38

	const uint8_t SomeDev_DeviceDescriptor[SomeDev_SIZ_DEVICE_DESC] =
	  {
	    0x12,   /* bLength  */
	    0x01,   /* bDescriptorType */
	    0x00,   /* bcdUSB, version 2.00 */
	    0x02,
	    0xff,   /* bDeviceClass : each interface define the device class */
	    0xff,   /* bDeviceSubClass */
	    0xff,   /* bDeviceProtocol */
	    0x40,   /* bMaxPacketSize0 0x40 = 64 */

		// $USBCONFIG - СЃСЋРґР° РІСЃС‚Р°РІРёС‚СЊ vendor ID (VID). в‰€РіРѕ РјРѕР¶РЅРѕ РєСѓРїРёС‚СЊ Сѓ usb.org Р·Р° РЅРµСЃРєРѕР»СЊРєРѕ С‚С‹СЃВ¤С‡ РґРѕР»Р»Р°СЂРѕРІ :)
		// В¬ РґР°РЅРЅРѕРј РїСЂРёРјРµСЂРµ РёСЃРїРѕР»СЊР·СѓРµС‚СЃВ¤ VID РѕС‚ ST Microelectrinics
		// вЂќ РЅРёС… РЅР° РєР°РєРёС…-С‚Рѕ СѓСЃР»РѕРІРёВ¤СЉ РјРѕР¶РЅРѕ РїРѕР»СѓС‡РёС‚СЊ PID РґР»В¤ СЃРІРѕРµРіРѕ СѓСЃС‚СЂРѕР№СЃС‚РІР°
		0x83,   /* idVendor     (0483) */
	    0x04,
		// $USBCONFIG - СЃСЋРґР° РІСЃС‚Р°РІРёС‚СЊ product ID (PID)
		// В¬ РґР°РЅРЅРѕРј РїСЂРёРјРµСЂРµ _вЂњСњР‹в„–В Сњ вЂ” Г·в‰€Р‹в„–С‘ СњВ«РЊС�В СњС›Р‹в‰€РЊВ»СЏ В» вЂњв‰€вЂ”вЂњВ»вЂ“СњВ¬С�РЊВ»СЏ_ РёСЃРїРѕР»СЊР·СѓРµС‚СЃВ¤ PID "РѕС‚ Р±Р°Р»РґС‹".
	    0xF0,   /* idProduct	(FFF0) */
	    0xFF,
	    0x00,   /* bcdDevice 2.00*/
	    0x02,
	    1,              /* index of string Manufacturer  */
	    /**/
	    2,              /* index of string descriptor of product*/
	    /* */
	    3,              /* */
	    /* */
	    /* */
	    0x01    /*bNumConfigurations */
	  };




	const uint8_t SomeDev_ConfigDescriptor[SomeDev_SIZ_CONFIG_DESC] =
	  {

	    0x09,   /* bLength: Configuation Descriptor size */
	    0x02,   /* bDescriptorType: Configuration */
	    SomeDev_SIZ_CONFIG_DESC,

	    0x00,
	    0x01,   /* bNumInterfaces: 1 interface */
	    0x01,   /* bConfigurationValue: */
	    /*      Configuration value */
	    0x00,   /* iConfiguration: */
	    /*      Index of string descriptor */
	    /*      describing the configuration */
	    0xC0,   /* bmAttributes: */
	    /*      bus powered */
	    0x32,   /* MaxPower 100 mA */

	    /******************** Descriptor of Mass Storage interface ********************/
	    /* 09 */
	    0x09,   /* bLength: Interface Descriptor size */
	    0x04,   /* bDescriptorType: */
	    /*      Interface descriptor type */
	    0x00,   /* bInterfaceNumber: Number of Interface */
	    0x00,   /* bAlternateSetting: Alternate setting */
	    0x02,   /* bNumEndpoints*/
	    0xff,   /* bInterfaceClass */
	    0xff,   /* bInterfaceSubClass */
	    0xff,   /* nInterfaceProtocol */
	    4,          /* iInterface: */
	    /* 18 */
	    0x07,   /*Endpoint descriptor length = 7*/
	    0x05,   /*Endpoint descriptor type */
	    0x81,   /*Endpoint address (IN, address 1) */
	    0x02,   /*Bulk endpoint type */
	    0x40,   /*Maximum packet size (64 bytes) */
	    0x00,
	    0x00,   /*Polling interval in milliseconds */
	    /* 25 */
	    0x07,   /*Endpoint descriptor length = 7 */
	    0x05,   /*Endpoint descriptor type */
	    0x02,   /*Endpoint address (OUT, address 2) */
	    0x02,   /*Bulk endpoint type */
	    0x40,   /*Maximum packet size (64 bytes) */
	    0x00,
	    0x00,     /*Polling interval in milliseconds*/
	    /*32*/
		// */
	  };

	// вЂ”С‚СЂРѕРєРѕРІС‹Рµ РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂС‹

	#define SomeDev_SIZ_STRING_SERIAL            16

#define SomeDev_SIZ_STRING_INTERFACE        	 20
	// СЏР·С‹Рє
	const uint8_t SomeDev_StringLangID[SomeDev_SIZ_STRING_LANGID] =
	  {
	    SomeDev_SIZ_STRING_LANGID,
	    0x03,
	    0x09,
	    0x04
	  };      /* LangID = 0x0409: U.S. English */

	// Vendor
	const uint8_t SomeDev_StringVendor[SomeDev_SIZ_STRING_VENDOR] =
	  {
	    SomeDev_SIZ_STRING_VENDOR, /* Size of manufaturer string */
	    0x03,           /* bDescriptorType = String descriptor */
	    /* Manufacturer */
	    'O', 0,
		'A', 0,
		'O', 0,
		' ', 0,
		'V', 0,
		'E', 0,
		'N', 0,
		'D', 0,
		'O', 0,
		'R', 0
	  };

	// С•СЂРѕРґСѓРєС‚
	const uint8_t SomeDev_StringProduct[SomeDev_SIZ_STRING_PRODUCT] =
	  {
	    SomeDev_SIZ_STRING_PRODUCT,
	    0x03,
	    /* Product name */
	    'M', 0,
		'y', 0,
		' ', 0,
		'T', 0,
		'e', 0,
		's', 0,
		't', 0,
		' ', 0,
	    'D', 0,
		'e', 0,
		'v', 0,
		'i', 0,
		'c', 0,
		'e', 0,
		' ', 0,
		'v', 0,
		'0', 0,
		'1', 0

	  };

	// $BOARDSPECIFIC - вЂ”РµСЂРёР№РЅС‹Р№ РЅРѕРјРµСЂ
	uint8_t SomeDev_StringSerial[SomeDev_SIZ_STRING_SERIAL] =
	  {
	    SomeDev_SIZ_STRING_SERIAL,
	    0x03,
	    /* Serial number */

	    'D', 0,
		'E', 0,
		'F', 0,
		'T', 0,
		'0', 0,
		'0', 0,
		'2', 0

	  };

	// В»РЅС‚РµСЂС„РµР№СЃ
	const uint8_t SomeDev_StringInterface[SomeDev_SIZ_STRING_INTERFACE] =
	  {
	    SomeDev_SIZ_STRING_INTERFACE,
	    0x03,
	    /* Interface 0 */
	    'I', 0,
		'n', 0,
		't', 0,
		'e', 0,
		'r', 0,
		'f', 0,
		'a', 0,
		'c', 0,
		'e', 0
	  };


	//Massstorage

#define MASS_SIZ_DEVICE_DESC              18
#define MASS_SIZ_CONFIG_DESC              32

#define MASS_SIZ_STRING_LANGID            4
#define MASS_SIZ_STRING_VENDOR            34
#define MASS_SIZ_STRING_PRODUCT           70
#define MASS_SIZ_STRING_SERIAL            26
#define MASS_SIZ_STRING_INTERFACE         16

	const uint8_t MASS_DeviceDescriptor[MASS_SIZ_DEVICE_DESC] =
	  {
	    0x12,   /* bLength  */
	    0x01,   /* bDescriptorType */
	    0x00,   /* bcdUSB, version 2.00 */
	    0x02,
	    0x00,   /* bDeviceClass : each interface define the device class */
	    0x00,   /* bDeviceSubClass */
	    0x00,   /* bDeviceProtocol */
	    0x40,   /* bMaxPacketSize0 0x40 = 64 */
	    0x83,   /* idVendor     (0483) */
	    0x04,
	    0x20,   /* idProduct */
	    0x57,
	    0x00,   /* bcdDevice 2.00*/
	    0x02,
	    1,              /* index of string Manufacturer  */
	    /**/
	    2,              /* index of string descriptor of product*/
	    /* */
	    3,              /* */
	    /* */
	    /* */
	    0x01    /*bNumConfigurations */
	  };
	const uint8_t MASS_ConfigDescriptor[MASS_SIZ_CONFIG_DESC] =
	  {

	    0x09,   /* bLength: Configuation Descriptor size */
	    0x02,   /* bDescriptorType: Configuration */
	    MASS_SIZ_CONFIG_DESC,

	    0x00,
	    0x01,   /* bNumInterfaces: 1 interface */
	    0x01,   /* bConfigurationValue: */
	    /*      Configuration value */
	    0x00,   /* iConfiguration: */
	    /*      Index of string descriptor */
	    /*      describing the configuration */
	    0xC0,   /* bmAttributes: */
	    /*      bus powered */
	    0x32,   /* MaxPower 100 mA */

	    /******************** Descriptor of Mass Storage interface ********************/
	    /* 09 */
	    0x09,   /* bLength: Interface Descriptor size */
	    0x04,   /* bDescriptorType: */
	    /*      Interface descriptor type */
	    0x00,   /* bInterfaceNumber: Number of Interface */
	    0x00,   /* bAlternateSetting: Alternate setting */
	    0x02,   /* bNumEndpoints*/
	    0x08,   /* bInterfaceClass: MASS STORAGE Class */
	    0x06,   /* bInterfaceSubClass : SCSI transparent*/
	    0x50,   /* nInterfaceProtocol */
	    4,          /* iInterface: */
	    /* 18 */
	    0x07,   /*Endpoint descriptor length = 7*/
	    0x05,   /*Endpoint descriptor type */
	    0x81,   /*Endpoint address (IN, address 1) */
	    0x02,   /*Bulk endpoint type */
	    0x40,   /*Maximum packet size (64 bytes) */
	    0x00,
	    0x00,   /*Polling interval in milliseconds */
	    /* 25 */
	    0x07,   /*Endpoint descriptor length = 7 */
	    0x05,   /*Endpoint descriptor type */
	    0x02,   /*Endpoint address (OUT, address 2) */
	    0x02,   /*Bulk endpoint type */
	    0x40,   /*Maximum packet size (64 bytes) */
	    0x00,
	    0x00     /*Polling interval in milliseconds*/
	    /*32*/
	  };
	const uint8_t MASS_StringLangID[MASS_SIZ_STRING_LANGID] =
	  {
	    MASS_SIZ_STRING_LANGID,
	    0x03,
	    0x09,
	    0x04
	  }
	  ;      /* LangID = 0x0409: U.S. English */
	const uint8_t MASS_StringVendor[MASS_SIZ_STRING_VENDOR] =
	  {
	    MASS_SIZ_STRING_VENDOR, /* Size of manufaturer string */
	    0x03,           /* bDescriptorType = String descriptor */
	    /* Manufacturer: "WWW.powermcu.com" */
	    'W', 0, 'W', 0, 'W', 0, '.', 0, 'P', 0, 'o', 0, 'w', 0, 'e', 0,
	    'r', 0, 'M', 0, 'C', 0, 'U', 0, '.', 0, 'C', 0, 'O', 0, 'M', 0,
	  };
	const uint8_t MASS_StringProduct[MASS_SIZ_STRING_PRODUCT] =
	  {
	    MASS_SIZ_STRING_PRODUCT,
	    0x03,
	  	/* Multi Media Development Board V1.0 */
	    'M', 0, 'u', 0, 'l', 0, 't', 0, 'i', 0, ' ', 0, 'M', 0,
	    'e', 0, 'd', 0, 'i', 0, 'a', 0, ' ', 0, 'D', 0, 'e', 0,
	    'v', 0, 'e', 0, 'l', 0, 'o', 0, 'p', 0, 'm', 0, 'e', 0,
		'n', 0, 't', 0, ' ', 0, 'B', 0, 'o', 0, 'a', 0, 'r', 0,
		'd', 0, ' ', 0, 'V', 0, '1', 0, '.', 0, '0'
	  };

	uint8_t MASS_StringSerial[MASS_SIZ_STRING_SERIAL] =
	  {
	    MASS_SIZ_STRING_SERIAL,
	    0x03,
	    /* Serial number*/
	    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
	  };
	const uint8_t MASS_StringInterface[MASS_SIZ_STRING_INTERFACE] =
	  {
	    MASS_SIZ_STRING_INTERFACE,
	    0x03,
	    /* Interface 0: "ST Mass" */
	    'S', 0, 'T', 0, ' ', 0, 'M', 0, 'a', 0, 's', 0, 's', 0
	  };

/// ------------------------  Hid keyboard -----------------------------------

#define HID_DESCRIPTOR_TYPE                     0x21
#define HID_REPORT_DESCRIPTOR_TYPE				0x22

#define RHID_SIZ_HID_DESC                   0x09
#define RHID_OFF_HID_DESC                   0x12

#define RHID_SIZ_DEVICE_DESC                18
#define RHID_SIZ_CONFIG_DESC                41

#define RHID_SIZ_STRING_LANGID              4
#define RHID_SIZ_STRING_VENDOR              10
#define RHID_SIZ_STRING_PRODUCT             22
#define RHID_SIZ_STRING_SERIAL              26

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

#define DEVICE_VER_H 0x01
#define DEVICE_VER_L 0x00

//HID Maximum packet size in bytes
#define wMaxPacketSize  0x40
#define EP1TxCount wMaxPacketSize
#define EP1RxCount 2

#define RPT3_COUNT 0x01 //PC->STM32
#define RPT4_COUNT 0x04 //STM32->PC


#define RHID_SIZ_MOUSE_REPORT_DESC                115
#define RHID_SIZ_KEYB_REPORT_DESC                65
#define RHID_SIZ_REPORT_DESC                115


	const uint8_t RHID_DeviceDescriptor[RHID_SIZ_DEVICE_DESC] =
	  {
			    RHID_SIZ_DEVICE_DESC,         // общая длина дескриптора устройства в байтах
			    USB_DEVICE_DESCRIPTOR_TYPE, // bDescriptorType - показывает, что это за дескриптор. В данном случае - Device descriptor
			    0x00, 0x02,                 // bcdUSB - какую версию стандарта USB поддерживает устройство. 2.0

				// класс, подкласс устройства и протокол, по стандарту USB. У нас нули, означает каждый интерфейс сам за себя
			    0x00,                       //bDeviceClass
			    0x00,                       //bDeviceSubClass
			    0x00,                       //bDeviceProtocol

			    0x40,                       //bMaxPacketSize - максимальный размер пакетов для Endpoint 0 (при конфигурировании)

				// те самые пресловутые VID и PID,  по которым и определяется, что же это за устройство.
				// в реальных устройствах надо покупать VID, чтобы устройства можно было различать и подсовывать нужные драйвера
			    0x83, 0x04,                 //idVendor (0x0483)
			    0x10, 0x57,                 //idProduct (0x5711)

			    DEVICE_VER_L, DEVICE_VER_H,                 // bcdDevice rel. DEVICE_VER_H.DEVICE_VER_L  номер релиза устройства

				// дальше идут индексы строк, описывающих производителя, устройство и серийный номер.
				// Отображаются в свойствах устройства в диспетчере устройств
				// А по серийному номеру подключенные устройства с одинаковым VID/PID различаются системой.
			    1,                          //Index of string descriptor describing manufacturer
			    2,                          //Index of string descriptor describing product
			    3,                          //Index of string descriptor describing the device serial number
			    0x01                        // bNumConfigurations - количество возможных конфигураций. У нас одна.
	  }
	  ; /* CustomHID_DeviceDescriptor */


	/* USB Configuration Descriptor */
	/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
	const uint8_t RHID_MOUSE_ConfigDescriptor[RHID_SIZ_CONFIG_DESC] =
	  {
			  0x09, 			// bLength: длина дескриптора конфигурации
			  		    USB_CONFIGURATION_DESCRIPTOR_TYPE, // bDescriptorType: тип дескриптора - конфигурация
			  		    RHID_SIZ_CONFIG_DESC, 0x00, // wTotalLength: общий размер всего дерева под данной конфигурацией в байтах

			  		    0x01,         // bNumInterfaces: в конфигурации всего один интерфейс
			  		    0x01,         // bConfigurationValue: индекс данной конфигурации
			  		    0x00,         // iConfiguration: индекс строки, которая описывает эту конфигурацию
			  		    0xE0,         // bmAttributes: признак того, что устройство будет питаться от шины USB
			  		    0x32,         // MaxPower 100 mA: и ему хватит 100 мА

			  				/************** Дескриптор интерфейса ****************/
			  				0x09,         // bLength: размер дескриптора интерфейса
			  				USB_INTERFACE_DESCRIPTOR_TYPE, // bDescriptorType: тип дескриптора - интерфейс
			  				0x00,         // bInterfaceNumber: порядковый номер интерфейса - 0
			  				0x00,         // bAlternateSetting: признак альтернативного интерфейса, у нас не используется
			  				0x02,         // bNumEndpoints - количество эндпоинтов.

			  				0x03,         // bInterfaceClass: класс интерфеса - HID
			  				// если бы мы косили под стандартное устройство, например клавиатуру или мышь, то надо было бы указать правильно класс и подкласс
			  				// а так у нас общее HID-устройство
			  				0x00,         // bInterfaceSubClass : подкласс интерфейса.
			  				0x00,         // nInterfaceProtocol : протокол интерфейса

			  				0,            // iInterface: индекс строки, описывающей интерфейс

			  					// теперь отдельный дескриптор для уточнения того, что данный интерфейс - это HID устройство
			  					/******************** HID дескриптор ********************/
			  					0x09,         // bLength: длина HID-дескриптора
			  					HID_DESCRIPTOR_TYPE, // bDescriptorType: тип дескриптора - HID
			  					0x01, 0x01,   // bcdHID: номер версии HID 1.1
			  					0x00,         // bCountryCode: код страны (если нужен)
			  					0x01,         // bNumDescriptors: Сколько дальше будет report дескрипторов
			  						HID_REPORT_DESCRIPTOR_TYPE,         // bDescriptorType: Тип дескриптора - report
			  						RHID_SIZ_REPORT_DESC,	0x00, // wItemLength: длина report-дескриптора


			  					/******************** дескриптор конечных точек (endpoints) ********************/
			  					0x07,          // bLength: длина дескриптора
			  					USB_ENDPOINT_DESCRIPTOR_TYPE, // тип дескриптора - endpoints

			  					0x81,          // bEndpointAddress: адрес конечной точки и направление 1(IN)
			  					0x03,          // bmAttributes: тип конечной точки - Interrupt endpoint
			  					wMaxPacketSize, 0x00,    // wMaxPacketSize:  Bytes max
			  					0x20,          // bInterval: Polling Interval (32 ms)

			            0x07,	/* bLength: Endpoint Descriptor size */
			            USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
			              /*	Endpoint descriptor type */
			            0x01,	/* bEndpointAddress: */
			              /*	Endpoint Address (OUT) */
			            0x03,	/* bmAttributes: Interrupt endpoint */
			            wMaxPacketSize,	/* wMaxPacketSize:  Bytes max  */
			            0x00,
			            0x20,	/* bInterval: Polling Interval (32 ms) */
	};
//	   /* RHID_ConfigDescriptor */
	const uint8_t RHID_MouseReportDescriptor[RHID_SIZ_MOUSE_REPORT_DESC] =
	  {
			  0x05,   0x01, // USAGE_PAGE (Generic Desktop)
			    0x09,   0x05, // USAGE (Game Pad)
			    0xA1,   0x01, // COLLECTION (Application)

			    0xA1,   0x00, // COLLECTION (Physical)
			    0x05,   0x09, // USAGE_PAGE (Button)
			    0x19,   0x01, // USAGE_MINIMUM (Button 1)
			    0x29,   0x03, // USAGE_MAXIMUM (Button 3)

			    0x15,   0x00, // LOGICAL_MINIMUM (0)
			    0x25,   0x01, // LOGICAL_MAXIMUM (1)
			    0x95,   0x03, // REPORT_COUNT (3)
			    0x75,   0x01, // REPORT_SIZE (1)

			    0x81,   0x02, // INPUT (Data,Var,Abs)
			    0x95,   0x01, // REPORT_COUNT (1)
			    0x75,   0x05, // REPORT_SIZE (5)
			    0x81,   0x07, // INPUT (Cnst,Var,Rel)

			    0x05,   0x01, // USAGE_PAGE (Generic Desktop)
			    0x09,   0x30, // USAGE (X)
			    0x09,   0x31, // USAGE (Y)

			    0x15,   0x81, // LOGICAL_MINIMUM (-127)
			    0x25,   0x7F, // LOGICAL_MAXIMUM (127)
			    0x75,   0x08, // REPORT_SIZE (8)
			    0x95,   0x02, // REPORT_COUNT (2)

			    0x81,   0x02, // INPUT (Data,Var,Abs)
			    0xC0,   0xC0  // END_COLLECTION x2
	  };



	const uint8_t RHID_KEYB_ConfigDescriptor[RHID_SIZ_CONFIG_DESC] =
	  {
			    0x09, 			// bLength: äëèíà äåñêðèïòîðà êîíôèãóðàöèè
			    USB_CONFIGURATION_DESCRIPTOR_TYPE, // bDescriptorType: òèï äåñêðèïòîðà - êîíôèãóðàöèÿ
			    RHID_SIZ_CONFIG_DESC, 0x00, // wTotalLength: îáùèé ðàçìåð âñåãî äåðåâà ïîä äàííîé êîíôèãóðàöèåé â áàéòàõ

			    0x01,         // bNumInterfaces: â êîíôèãóðàöèè âñåãî îäèí èíòåðôåéñ
			    0x01,         // bConfigurationValue: èíäåêñ äàííîé êîíôèãóðàöèè
			    0x00,         // iConfiguration: èíäåêñ ñòðîêè, êîòîðàÿ îïèñûâàåò ýòó êîíôèãóðàöèþ
			    0xE0,         // bmAttributes: ïðèçíàê òîãî, ÷òî óñòðîéñòâî áóäåò ïèòàòüñÿ îò øèíû USB
			    0x32,         // MaxPower 100 mA: è åìó õâàòèò 100 ìÀ

					/************** Äåñêðèïòîð èíòåðôåéñà ****************/
					0x09,         // bLength: ðàçìåð äåñêðèïòîðà èíòåðôåéñà
					USB_INTERFACE_DESCRIPTOR_TYPE, // bDescriptorType: òèï äåñêðèïòîðà - èíòåðôåéñ
					0x00,         // bInterfaceNumber: ïîðÿäêîâûé íîìåð èíòåðôåéñà - 0
					0x00,         // bAlternateSetting: ïðèçíàê àëüòåðíàòèâíîãî èíòåðôåéñà, ó íàñ íå èñïîëüçóåòñÿ
					0x02,         // bNumEndpoints - êîëè÷åñòâî ýíäïîèíòîâ.

					0x03,         // bInterfaceClass: êëàññ èíòåðôåñà - HID
					// åñëè áû ìû êîñèëè ïîä ñòàíäàðòíîå óñòðîéñòâî, íàïðèìåð êëàâèàòóðó èëè ìûøü, òî íàäî áûëî áû óêàçàòü ïðàâèëüíî êëàññ è ïîäêëàññ
					// à òàê ó íàñ îáùåå HID-óñòðîéñòâî
					0x00,         // bInterfaceSubClass : ïîäêëàññ èíòåðôåéñà.
					0x00,         // nInterfaceProtocol : ïðîòîêîë èíòåðôåéñà

					0,            // iInterface: èíäåêñ ñòðîêè, îïèñûâàþùåé èíòåðôåéñ

						// òåïåðü îòäåëüíûé äåñêðèïòîð äëÿ óòî÷íåíèÿ òîãî, ÷òî äàííûé èíòåðôåéñ - ýòî HID óñòðîéñòâî
						/******************** HID äåñêðèïòîð ********************/
						0x09,         // bLength: äëèíà HID-äåñêðèïòîðà
						HID_DESCRIPTOR_TYPE, // bDescriptorType: òèï äåñêðèïòîðà - HID
						0x01, 0x01,   // bcdHID: íîìåð âåðñèè HID 1.1
						0x00,         // bCountryCode: êîä ñòðàíû (åñëè íóæåí)
						0x01,         // bNumDescriptors: Ñêîëüêî äàëüøå áóäåò report äåñêðèïòîðîâ
							HID_REPORT_DESCRIPTOR_TYPE,         // bDescriptorType: Òèï äåñêðèïòîðà - report
							RHID_SIZ_KEYB_REPORT_DESC,	0x00, // wItemLength: äëèíà report-äåñêðèïòîðà


						/******************** äåñêðèïòîð êîíå÷íûõ òî÷åê (endpoints) ********************/
						0x07,          // bLength: äëèíà äåñêðèïòîðà
						USB_ENDPOINT_DESCRIPTOR_TYPE, // òèï äåñêðèïòîðà - endpoints

						0x81,          // bEndpointAddress: àäðåñ êîíå÷íîé òî÷êè è íàïðàâëåíèå 1(IN)
						0x03,          // bmAttributes: òèï êîíå÷íîé òî÷êè - Interrupt endpoint
						wMaxPacketSize, 0x00,    // wMaxPacketSize:  Bytes max
						0x20,          // bInterval: Polling Interval (32 ms)

	          0x07,	/* bLength: Endpoint Descriptor size */
	          USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
	            /*	Endpoint descriptor type */
	          0x01,	/* bEndpointAddress: */
	            /*	Endpoint Address (OUT) */
	          0x03,	/* bmAttributes: Interrupt endpoint */
	          wMaxPacketSize,	/* wMaxPacketSize:  Bytes max  */
	          0x00,
	          0x20,	/* bInterval: Polling Interval (32 ms) */
	};


	const uint8_t RHID_KeybReportDescriptor[RHID_SIZ_KEYB_REPORT_DESC] =
		  {
				0x05, 0x01, /* Usage Page (Generic Desktop)             */
			    0x09, 0x06, /*		Usage (Keyboard)                    */
			    0xA1, 0x01, /*		Collection (Application)            */
			    0x85, 0x02,  /*   Report ID  */
			    0x05, 0x07, /*  	Usage (Key codes)                   */
			    0x19, 0xE0, /*      Usage Minimum (224)                 */
			    0x29, 0xE7, /*      Usage Maximum (231)                 */
			    0x15, 0x00, /*      Logical Minimum (0)                 */
			    0x25, 0x01, /*      Logical Maximum (1)                 */
			    0x75, 0x01, /*      Report Size (1)                     */
			    0x95, 0x08, /*      Report Count (8)                    */
			    0x81, 0x02, /*      Input (Data, Variable, Absolute)    */
			    0x95, 0x01, /*      Report Count (1)                    */
			    0x75, 0x08, /*      Report Size (8)                     */
			    0x81, 0x01, /*      Input (Constant)    ;5 bit padding  */
			    0x95, 0x05, /*      Report Count (5)                    */
			    0x75, 0x01, /*      Report Size (1)                     */
			    0x05, 0x08, /*      Usage Page (Page# for LEDs)         */
			    0x19, 0x01, /*      Usage Minimum (01)                  */
			    0x29, 0x05, /*      Usage Maximum (05)                  */
			    0x91, 0x02, /*      Output (Data, Variable, Absolute)   */
			    0x95, 0x01, /*      Report Count (1)                    */
			    0x75, 0x03, /*      Report Size (3)                     */
			    0x91, 0x01, /*      Output (Constant)                   */
			    0x95, 0x06, /*      Report Count (1)                    */
			    0x75, 0x08, /*      Report Size (3)                     */
			    0x15, 0x00, /*      Logical Minimum (0)                 */
			    0x25, 0x65, /*      Logical Maximum (101)               */
			    0x05, 0x07, /*  	Usage (Key codes)                   */
			    0x19, 0x00, /*      Usage Minimum (00)                  */
			    0x29, 0x65, /*      Usage Maximum (101)                 */
			    0x81, 0x00, /*      Input (Data, Array)                 */
			    0xC0 /* 		End Collection,End Collection       */
	};



	/* USB String Descriptors (optional) */
	const uint8_t RHID_StringLangID[RHID_SIZ_STRING_LANGID] =
	  {
	    RHID_SIZ_STRING_LANGID,
	    USB_STRING_DESCRIPTOR_TYPE,
	    0x09,
	    0x04
	  }
	  ; /* LangID = 0x0409: U.S. English */

	const uint8_t RHID_StringVendor[RHID_SIZ_STRING_VENDOR] =
	  {
	    RHID_SIZ_STRING_VENDOR, /* Size of Vendor string */
	    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
	    /* Manufacturer: "STMicroelectronics" */
	    'R', 0, 'a', 0, 'J', 0, 'a', 0
	  };

	const uint8_t RHID_StringProduct[RHID_SIZ_STRING_PRODUCT] =
	  {
	    RHID_SIZ_STRING_PRODUCT,          /* bLength */
	    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
	    'R', 0, 'H', 0, 'I', 0, 'D', 0, ' ', 0, 'S', 0, 'T', 0,
	    'M', 0, '3', 0, '2', 0
	  };
	uint8_t RHID_StringSerial[RHID_SIZ_STRING_SERIAL] =
	  {
	    RHID_SIZ_STRING_SERIAL,           /* bLength */
	    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
	    'R', 0, 'H', 0, '-', 0, '0', 0, '0', 0, '0', 0, '1', 0
	  };

#endif /* DRIVER_DESCRIPTORS_H_ */
