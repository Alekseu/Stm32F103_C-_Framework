/*
 * UsbCom.h
 *
 *  Created on: 15 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_USB_H_
#define DRIVER_USB_H_



#define RCC_APB2Periph_GPIO_DISCONNECT RCC_APB2Periph_GPIOA
#define USB_DISCONNECT_PIN	GPIO_Pin_10
#define USB_DISCONNECT  GPIOA

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


#define SEND_ENCAPSULATED_COMMAND   0x00
#define GET_ENCAPSULATED_RESPONSE   0x01
#define SET_COMM_FEATURE            0x02
#define GET_COMM_FEATURE            0x03
#define CLEAR_COMM_FEATURE          0x04
#define SET_LINE_CODING             0x20
#define GET_LINE_CODING             0x21
#define SET_CONTROL_LINE_STATE      0x22
#define SEND_BREAK                  0x23

extern "C"
{
	#include "../StdPeriph/usb/inc/usb_lib.h"
	#include "../StdPeriph/usb/inc/usb_conf.h"

	#include "../StdPeriph/cmsis_boot/stm32f10x.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
	#include "../StdPeriph/stm_lib/inc/misc.h"

}

typedef enum _DEVICE_STATE
{
  UNCONNECTED,
  ATTACHED,
  POWERED,
  SUSPENDED,
  ADDRESSED,
  CONFIGURED
} DEVICE_STATE;

class Usb
{
	public:
	static Usb* pUsb;
	USER_STANDARD_REQUESTS User_Standard_Requests;
	DEVICE_PROP Device_Property;
	DEVICE Device_Table;
	__IO uint32_t bDeviceState;
	uint8_t Request = 0;

	ONE_DESCRIPTOR Device_Descriptor;
	ONE_DESCRIPTOR Config_Descriptor;
	ONE_DESCRIPTOR String_Descriptor[4];

	unsigned char RxBuffer[32];
	unsigned char TxBuffer[32];

	bool tr=true;

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
	static void  UsbGetInterface();
	static void  UsbSetInterface();
	static void UsbGetStatus();
	static void UsbClearFeature();
	static void UsbSetEndPointFeature();
	static void UsbSetDeviceFeature();
	static void UsbSetDeviceAddress();

	static uint8_t* LineCodingStamp(uint16_t t);

	virtual int  SendDataToUsb();
	virtual void RecivedFromUsb(unsigned int rx);



	bool Init();
	void SendData(const char* data);
	//char* ReadData();

	private:
	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned int _txBytes;
	unsigned int _rxBytes;

};


#endif /* DRIVER_USB_H_ */
