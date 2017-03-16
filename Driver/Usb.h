/*
 * UsbCom.h
 *
 *  Created on: 15 ����� 2017 �.
 *      Author: hudienko_a
 */

#ifndef DRIVER_USB_H_
#define DRIVER_USB_H_



#define RCC_APB2Periph_GPIO_DISCONNECT RCC_APB2Periph_GPIOA
#define USB_DISCONNECT_PIN	GPIO_Pin_10
#define USB_DISCONNECT  GPIOA



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
	ONE_DESCRIPTOR String_Descriptor[5];

	unsigned char RxBuffer[257];
	unsigned char TxBuffer[257];

	unsigned int _txBytes;
	unsigned int _rxBytes;

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

	virtual void  SendDataToUsb(unsigned char endpoint);
	virtual void RecivedFromUsb(unsigned int endpoint);



	bool Init();
	void SendData(const char* data, int length);
	int ReadData(char* mass);


	static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);

	private:
	GPIO_InitTypeDef GPIO_InitStructure;


};


#endif /* DRIVER_USB_H_ */
