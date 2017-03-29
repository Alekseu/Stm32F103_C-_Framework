/*
 * UsbCom.h
 *
 *  Created on: 15 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_USB_H_
#define DRIVER_USB_H_

//TODO ¬озможно необходимо разнести по разным классам - классы usb устройств

#define RCC_APB2Periph_GPIO_DISCONNECT RCC_APB2Periph_GPIOA
#define USB_DISCONNECT_PIN	GPIO_Pin_10
#define USB_DISCONNECT  GPIOA


extern "C"
{
	#include "../../StdPeriph/usb/inc/usb_lib.h"
	#include "../../StdPeriph/usb/inc/usb_conf.h"
	#include "../../StdPeriph/cmsis_boot/stm32f10x.h"
	#include "../../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"
	#include "../../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
	#include "../../StdPeriph/stm_lib/inc/misc.h"

}

typedef struct
	{
		uint32_t bitrate;
		uint8_t format;
		uint8_t paritytype;
		uint8_t datatype;
	}LINE_CODING;


typedef enum _DEVICE_STATE
{
  UNCONNECTED,
  ATTACHED,
  POWERED,
  SUSPENDED,
  ADDRESSED,
  CONFIGURED
} DEVICE_STATE;


typedef enum _Usbtype
{
	VirtualComPort,
	HumanInterfaceDevice,
	MassStorageDevice
} UsbType;


class Usb
{
	public:
	static Usb* pUsb;

	USER_STANDARD_REQUESTS User_Standard_Requests;
	DEVICE_PROP Device_Property;
	DEVICE Device_Table;
	uint32_t bDeviceState;
	uint8_t Request = 0;

	ONE_DESCRIPTOR Device_Descriptor;
	ONE_DESCRIPTOR Config_Descriptor;
	ONE_DESCRIPTOR String_Descriptor[5];

	UsbType TypeUsb;

	unsigned int RxBufferSize;
	unsigned int TxBufferSize;

	unsigned char* RxBuffer;
	unsigned char* TxBuffer;

	unsigned int TxBytes;
	unsigned int RxBytes;

	// if use vcp
	LINE_CODING Linecoding;


	Usb();
	virtual ~Usb(){}

	//device property
	static void UsbInit();
	static void UsbReset();
	static void UsbStatus_In();
	static void UsbStatus_Out();
	static RESULT UsbData_Setup(uint8_t RequestNo);
	static RESULT UsbNoData_Setup(uint8_t RequestNo);
	static RESULT UsbGet_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
	static uint8_t* UsbGetDeviceDescriptor(uint16_t Length);
	static uint8_t* UsbGetConfigDescriptor(uint16_t Length);
	static uint8_t* UsbGetStringDescriptor(uint16_t Length);

	//standart requests
	static void UsbGetConfiguration();
	static void UsbSetConfiguration();
	static void UsbGetInterface();
	static void UsbSetInterface();
	static void UsbGetStatus();
	static void UsbClearFeature();
	static void UsbSetEndPointFeature();
	static void UsbSetDeviceFeature();
	static void UsbSetDeviceAddress();

	//for vcp
	static uint8_t* LineCodingStamp(uint16_t t);

	//endpoint events
	virtual void  SendDataToUsb(unsigned char endpoint);
	virtual void RecivedFromUsb(unsigned int endpoint);


	bool Init();

	void SendData(const char* data, int length);
	int ReadData(char* mass);


	private:
	GPIO_InitTypeDef GPIO_InitStructure;


};


#endif /* DRIVER_USB_H_ */
