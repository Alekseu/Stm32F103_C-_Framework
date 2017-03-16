/*
 * UsbCom.cpp
 *
 *  Created on: 15 ����� 2017 �.
 *      Author: hudienko_a
 */
#include "Usb.h"
#include "../Extention/delay.h"
#include <string.h>

#include "Descriptors.h"

Usb *Usb::pUsb = 0;

extern "C"
{
	#include "../StdPeriph/cmsis_boot/stm32f10x.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
	#include "../StdPeriph/stm_lib/inc/misc.h"
	extern __IO BOOL fSuspendEnabled;  /* true when suspend is possible */

	__IO uint16_t wIstr;  /* ISTR register last read value */
	__IO uint8_t bIntPackSOF = 0;  /* SOFs received between 2 consecutive packets */

	#define VCOMPORT_IN_FRAME_INTERVAL             5

	DEVICE Device_Table =
	{
			EP_NUM,
			1
	};

	typedef struct
	{
		uint32_t bitrate;
		uint8_t format;
		uint8_t paritytype;
		uint8_t datatype;
	}LINE_CODING;

	LINE_CODING linecoding =
	{
			115200, /* baud rate*/
			0x00,   /* stop bits-1*/
			0x00,   /* parity - none*/
			0x08    /* no. of bits 8*/
	};

	void EP1_IN_Callback (void)
	{
		 Usb::pUsb->SendDataToUsb(1);
	}
	void EP2_IN_Callback(void)
	{
		Usb::pUsb->SendDataToUsb(2);
	}
	void   EP3_IN_Callback(void)
	{
		Usb::pUsb->SendDataToUsb(3);
	}
	void    EP4_IN_Callback(void)
	{
		Usb::pUsb->SendDataToUsb(4);
	}
	void    EP5_IN_Callback(void)
	{
		Usb::pUsb->SendDataToUsb(5);
	}
	void    EP6_IN_Callback(void)
	{
		Usb::pUsb->SendDataToUsb(6);
	}
	void   EP7_IN_Callback(void)
	{
		Usb::pUsb->SendDataToUsb(7);
	}


	void EP1_OUT_Callback(void)
	{
		 Usb::pUsb->RecivedFromUsb(1);
	}
	void    EP2_OUT_Callback(void)
	{
		 Usb::pUsb->RecivedFromUsb(2);
	}
	void   EP3_OUT_Callback(void)
	{
		 Usb::pUsb->RecivedFromUsb(3);
	}
	void    EP4_OUT_Callback(void)
	{
		 Usb::pUsb->RecivedFromUsb(4);
	}
	void    EP5_OUT_Callback(void)
	{
		 Usb::pUsb->RecivedFromUsb(5);
	}
	void    EP6_OUT_Callback(void)
	{
		 Usb::pUsb->RecivedFromUsb(6);
	}
	void    EP7_OUT_Callback(void)
	{
		 Usb::pUsb->RecivedFromUsb(7);
	}


	void SOF_Callback(void)
	{
		static uint32_t FrameCount = 0;

		  if(Usb::pUsb->bDeviceState == CONFIGURED)
		  {
		    if (FrameCount++ == VCOMPORT_IN_FRAME_INTERVAL)
		    {
		      /* Reset the frame counter */
		      FrameCount = 0;
		      Usb::pUsb->SendDataToUsb(0);
		    }
		  }
	}



void (*pEpInt_IN[7])(void) =
  {
    EP1_IN_Callback,
    EP2_IN_Callback,
    EP3_IN_Callback,
    EP4_IN_Callback,
    EP5_IN_Callback,
    EP6_IN_Callback,
    EP7_IN_Callback,
  };

void (*pEpInt_OUT[7])(void) =
  {
    EP1_OUT_Callback,
    EP2_OUT_Callback,
    EP3_OUT_Callback,
    EP4_OUT_Callback,
    EP5_OUT_Callback,
    EP6_OUT_Callback,
    EP7_OUT_Callback,
  };

	void USB_LP_CAN1_RX0_IRQHandler(void)
	{

		  wIstr = _GetISTR();

		#if (IMR_MSK & ISTR_SOF)
		  if (wIstr & ISTR_SOF & wInterrupt_Mask)
		  {
		    _SetISTR((uint16_t)CLR_SOF);
		    bIntPackSOF++;

		#ifdef SOF_CALLBACK
		    SOF_Callback();
		#endif
		  }
		#endif
		  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

		#if (IMR_MSK & ISTR_CTR)
		  if (wIstr & ISTR_CTR & wInterrupt_Mask)
		  {
		    /* servicing of the endpoint correct transfer interrupt */
		    /* clear of the CTR flag into the sub */
		    CTR_LP();

		  }
		#endif
		  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
		#if (IMR_MSK & ISTR_RESET)
		  if (wIstr & ISTR_RESET & wInterrupt_Mask)
		  {
		    _SetISTR((uint16_t)CLR_RESET);
		    Usb::pUsb->Device_Property.Reset();
		  }
		#endif
		  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
		#if (IMR_MSK & ISTR_DOVR)
		  if (wIstr & ISTR_DOVR & wInterrupt_Mask)
		  {
		    _SetISTR((uint16_t)CLR_DOVR);
		#ifdef DOVR_CALLBACK
		    DOVR_Callback();
		#endif
		  }
		#endif
		  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
		#if (IMR_MSK & ISTR_ERR)
		  if (wIstr & ISTR_ERR & wInterrupt_Mask)
		  {
		    _SetISTR((uint16_t)CLR_ERR);
		#ifdef ERR_CALLBACK
		    ERR_Callback();
		#endif
		  }
		#endif
		  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
		#if (IMR_MSK & ISTR_WKUP)
		  if (wIstr & ISTR_WKUP & wInterrupt_Mask)
		  {
		    _SetISTR((uint16_t)CLR_WKUP);
		    Resume(RESUME_EXTERNAL);
		#ifdef WKUP_CALLBACK
		    WKUP_Callback();
		#endif
		  }
		#endif
		  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
		#if (IMR_MSK & ISTR_SUSP)
		  if (wIstr & ISTR_SUSP & wInterrupt_Mask)
		  {

		    /* check if SUSPEND is possible */
		    if (fSuspendEnabled)
		    {
		      Suspend();
		    }
		    else
		    {
		      /* if not possible then resume after xx ms */
		      Resume(RESUME_LATER);
		    }
		    /* clear of the ISTR bit must be done after setting of CNTR_FSUSP */
		    _SetISTR((uint16_t)CLR_SUSP);
		#ifdef SUSP_CALLBACK
		    SUSP_Callback();
		#endif
		  }
		#endif
		  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

		#if (IMR_MSK & ISTR_ESOF)
		  if (wIstr & ISTR_ESOF & wInterrupt_Mask)
		  {
		    /* clear ESOF flag in ISTR */
		    _SetISTR((uint16_t)CLR_ESOF);

		    if ((_GetFNR()&FNR_RXDP)!=0)
		    {
		      /* increment ESOF counter */
		      esof_counter ++;

		      /* test if we enter in ESOF more than 3 times with FSUSP =0 and RXDP =1=>> possible missing SUSP flag*/
		      if ((esof_counter >3)&&((_GetCNTR()&CNTR_FSUSP)==0))
		      {
		        /* this a sequence to apply a force RESET*/

		        /*Store CNTR value */
		        wCNTR = _GetCNTR();

		        /*Store endpoints registers status */
		        for (i=0;i<8;i++) EP[i] = _GetENDPOINT(i);

		        /*apply FRES */
		        wCNTR|=CNTR_FRES;
		        _SetCNTR(wCNTR);

		        /*clear FRES*/
		        wCNTR&=~CNTR_FRES;
		        _SetCNTR(wCNTR);

		        /*poll for RESET flag in ISTR*/
		        while((_GetISTR()&ISTR_RESET) == 0);

		        /* clear RESET flag in ISTR */
		        _SetISTR((uint16_t)CLR_RESET);

		       /*restore Enpoints*/
		        for (i=0;i<8;i++)
		        _SetENDPOINT(i, EP[i]);

		        esof_counter = 0;
		      }
		    }
		    else
		    {
		        esof_counter = 0;
		    }

		    /* resume handling timing is made with ESOFs */
		    Resume(RESUME_ESOF); /* request without change of the machine state */

		#ifdef ESOF_CALLBACK
		    ESOF_Callback();
		#endif
		  }
		#endif
	}

}//end extern "C"





#define SEND_ENCAPSULATED_COMMAND   0x00
#define GET_ENCAPSULATED_RESPONSE   0x01
#define SET_COMM_FEATURE            0x02
#define GET_COMM_FEATURE            0x03
#define CLEAR_COMM_FEATURE          0x04
#define SET_LINE_CODING             0x20
#define GET_LINE_CODING             0x21
#define SET_CONTROL_LINE_STATE      0x22
#define SEND_BREAK                  0x23



bool Usb::Init()
{
	pUsb = this;
	bDeviceState = UNCONNECTED;


//	Device_Descriptor.Descriptor = (uint8_t*)SomeDev_DeviceDescriptor;//_Virtual_Com_Port_Device_Descriptor;
//	Device_Descriptor.Descriptor_Size = SomeDev_SIZ_DEVICE_DESC;//VIRTUAL_COM_PORT_SIZ_DEVICE_DESC;
//
//	Config_Descriptor.Descriptor =(uint8_t*)SomeDev_ConfigDescriptor;//_Virtual_Com_Port_ConfigDescriptor;
//	Config_Descriptor.Descriptor_Size = SomeDev_SIZ_CONFIG_DESC;//VIRTUAL_COM_PORT_SIZ_CONFIG_DESC;
//
//	String_Descriptor[0] = {(uint8_t*)SomeDev_StringLangID};//Virtual_Com_Port_StringLangID, VIRTUAL_COM_PORT_SIZ_STRING_LANGID};
//	String_Descriptor[1] = {(uint8_t*)SomeDev_StringVendor};//Virtual_Com_Port_StringVendor, VIRTUAL_COM_PORT_SIZ_STRING_VENDOR};
//	String_Descriptor[2] = {(uint8_t*)SomeDev_StringProduct};//Virtual_Com_Port_StringProduct, VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT};
//	String_Descriptor[3] = {(uint8_t*)SomeDev_StringSerial};//Virtual_Com_Port_StringSerial, VIRTUAL_COM_PORT_SIZ_STRING_SERIAL};
//	String_Descriptor[4] = {(uint8_t*)SomeDev_StringInterface};

	Device_Descriptor.Descriptor = (uint8_t*)_Virtual_Com_Port_Device_Descriptor;
	Device_Descriptor.Descriptor_Size = VIRTUAL_COM_PORT_SIZ_DEVICE_DESC;

	Config_Descriptor.Descriptor =(uint8_t*)_Virtual_Com_Port_ConfigDescriptor;
	Config_Descriptor.Descriptor_Size = VIRTUAL_COM_PORT_SIZ_CONFIG_DESC;

	String_Descriptor[0] = {(uint8_t*)Virtual_Com_Port_StringLangID, VIRTUAL_COM_PORT_SIZ_STRING_LANGID};
	String_Descriptor[1] = {(uint8_t*)Virtual_Com_Port_StringVendor, VIRTUAL_COM_PORT_SIZ_STRING_VENDOR};
	String_Descriptor[2] = {(uint8_t*)Virtual_Com_Port_StringProduct, VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT};
	String_Descriptor[3] = {(uint8_t*)Virtual_Com_Port_StringSerial, VIRTUAL_COM_PORT_SIZ_STRING_SERIAL};

	//set system
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Configure USB pull-up pin */
	GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

	//set usb clock
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

	/* Enable the USB clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

	//usb interrupts
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	Device_Property.Init = UsbInit;
	Device_Property.Reset = UsbReset;
	Device_Property.Process_Status_IN = UsbStatus_In;
	Device_Property.Process_Status_OUT = UsbStatus_Out;
	Device_Property.Class_Data_Setup = UsbData_Setup;
	Device_Property.Class_NoData_Setup = UsbNoData_Setup;
	Device_Property.Class_Get_Interface_Setting = UsbGet_Interface_Setting;
	Device_Property.GetDeviceDescriptor = UsbGetDeviceDescriptor;
	Device_Property.GetConfigDescriptor = UsbGetConfigDescriptor;
	Device_Property.GetStringDescriptor = UsbGetStringDescriptor;
	Device_Property.RxEP_buffer =0;
	Device_Property.MaxPacketSize = 0x40;

	User_Standard_Requests.User_GetConfiguration = UsbGetConfiguration;
	User_Standard_Requests.User_SetConfiguration = UsbSetConfiguration;
	User_Standard_Requests.User_GetInterface = UsbGetInterface;
	User_Standard_Requests.User_SetInterface = UsbSetInterface;
	User_Standard_Requests.User_GetStatus = UsbGetStatus;
	User_Standard_Requests.User_ClearFeature = UsbClearFeature;
	User_Standard_Requests.User_SetEndPointFeature = UsbSetEndPointFeature;
	User_Standard_Requests.User_SetDeviceFeature = UsbSetDeviceFeature;
	User_Standard_Requests.User_SetDeviceAddress = UsbSetDeviceAddress;

	//SetDeviceProperty(&Device_Property);
	USB_Init(&Device_Property,&User_Standard_Requests);

	int timeout = 1500;

	while (bDeviceState != CONFIGURED && --timeout)
	{
		_delay_ms(1);
	}

	return timeout>0;
}

void Usb::UsbInit()
{
	 pInformation->Current_Configuration = 0;

	 GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
	 _delay_ms(150);
	 GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
	 _delay_ms(10);

	 uint16_t wRegVal;
	 wRegVal = CNTR_FRES;
	 _SetCNTR(wRegVal);

	 /*** CNTR_FRES = 0 ***/
	 wInterrupt_Mask = 0;
	 _SetCNTR(wInterrupt_Mask);
	 /*** Clear pending interrupts ***/
	 _SetISTR(0);
	 /*** Set interrupt mask ***/
	 wInterrupt_Mask = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM;
	 _SetCNTR(wInterrupt_Mask);

	 USB_SIL_Init();

	 Usb::pUsb->bDeviceState = UNCONNECTED;

}

 void Usb::UsbReset()
 {
	 pInformation->Current_Configuration = 0;

	  /* Current Feature initialization */
	  pInformation->Current_Feature = ((uint8_t*)&Usb::pUsb->Config_Descriptor)[7];

	  /* Set Virtual_Com_Port DEVICE with the default Interface*/
	  pInformation->Current_Interface = 0;

	  SetBTABLE(BTABLE_ADDRESS);

	  /* Initialize Endpoint 0 */
	  SetEPType(ENDP0, EP_CONTROL);
	  SetEPTxStatus(ENDP0, EP_TX_STALL);
	  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
	  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
	  Clear_Status_Out(ENDP0);
	  SetEPRxCount(ENDP0, Usb::pUsb->Device_Property.MaxPacketSize);
	  SetEPRxValid(ENDP0);

	  /* Initialize Endpoint 1 */
	  SetEPType(ENDP1, EP_BULK);
	  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
	  SetEPTxStatus(ENDP1, EP_TX_NAK);
	  SetEPRxStatus(ENDP1, EP_RX_DIS);

	  /* Initialize Endpoint 2 */
	  SetEPType(ENDP2, EP_INTERRUPT);
	  SetEPTxAddr(ENDP2, ENDP2_TXADDR);
	  SetEPRxStatus(ENDP2, EP_RX_DIS);
	  SetEPTxStatus(ENDP2, EP_TX_NAK);

	  /* Initialize Endpoint 3 */
	  SetEPType(ENDP3, EP_BULK);
	  SetEPRxAddr(ENDP3, ENDP3_RXADDR);
	  SetEPRxCount(ENDP3, VIRTUAL_COM_PORT_DATA_SIZE);
	  SetEPRxStatus(ENDP3, EP_RX_VALID);
	  SetEPTxStatus(ENDP3, EP_TX_DIS);

	  /* Set this device to response on default address */
	  SetDeviceAddress(0);

//		SetBTABLE(BTABLE_ADDRESS);
//
//		// ������������� Endpoint 0
//		SetEPType(ENDP0, EP_CONTROL);
//		SetEPTxStatus(ENDP0, EP_TX_NAK);
//		SetEPRxAddr(ENDP0, ENDP0_RXADDR);
//		SetEPRxCount(ENDP0, Usb::pUsb->Device_Property.MaxPacketSize);
//		SetEPTxAddr(ENDP0, ENDP0_TXADDR);
//		Clear_Status_Out(ENDP0);
//		SetEPRxValid(ENDP0);
//
//		// ������������� Endpoint 1
//		SetEPType(ENDP1, EP_BULK);
//		SetEPTxAddr(ENDP1, ENDP1_TXADDR);
//		SetEPTxStatus(ENDP1, EP_TX_NAK);
//		SetEPRxStatus(ENDP1, EP_RX_DIS);
//
//		// ������������� Endpoint 2
//		SetEPType(ENDP2, EP_BULK);
//		SetEPRxAddr(ENDP2, ENDP2_RXADDR);
//		SetEPRxCount(ENDP2, Usb::pUsb->Device_Property.MaxPacketSize);
//		SetEPRxStatus(ENDP2, EP_RX_VALID);
//		SetEPTxStatus(ENDP2, EP_TX_DIS);
//
//
//		SetEPRxCount(ENDP0, Usb::pUsb->Device_Property.MaxPacketSize);
//		SetEPRxValid(ENDP0);
//
//		// ��������� ���������� ������
//		SetDeviceAddress(0);

	  Usb::pUsb->bDeviceState = ATTACHED;
 }

 void Usb::UsbStatus_In()
 {
	 if (Usb::pUsb->Request == SET_LINE_CODING)
	   {
		 Usb::pUsb->Request = 0;
	   }
 }

 void Usb::UsbStatus_Out()
 {

 }

 RESULT Usb::UsbData_Setup(uint8_t RequestNo)
 {
	 uint8_t    *(*CopyRoutine)(uint16_t);

	   CopyRoutine = 0;

	   if (RequestNo == GET_LINE_CODING)
	   {
	     if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
	     {
	       CopyRoutine = LineCodingStamp;
	     }
	   }
	   else if (RequestNo == SET_LINE_CODING)
	   {
	     if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
	     {
	       CopyRoutine = LineCodingStamp;
	     }
	     Usb::pUsb->Request = SET_LINE_CODING;
	   }

	   if (CopyRoutine == NULL)
	   {
	     return USB_UNSUPPORT;
	   }

	   pInformation->Ctrl_Info.CopyData = CopyRoutine;
	   pInformation->Ctrl_Info.Usb_wOffset = 0;
	   (*CopyRoutine)(0);
	   return USB_SUCCESS;
 }

 RESULT Usb::UsbNoData_Setup(uint8_t RequestNo)
 {
	 if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
	 {
		 if (RequestNo == SET_COMM_FEATURE)
		 {
			 return USB_SUCCESS;
		 }
		 else if (RequestNo == SET_CONTROL_LINE_STATE)
		 {
			 return USB_SUCCESS;
		 }
	 }

	 return USB_UNSUPPORT;
 }

 RESULT Usb::UsbGet_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
 {
	 if (AlternateSetting > 0)
	 {
		 return USB_UNSUPPORT;
	 }
	 else if (Interface > 1)
	 {
		 return USB_UNSUPPORT;
	 }
	 return USB_SUCCESS;
 }

 uint8_t* Usb::UsbGetDeviceDescriptor(uint16_t Length)
 {
	 return Standard_GetDescriptorData(Length, &Usb::pUsb->Device_Descriptor);
 }

 uint8_t* Usb::UsbGetConfigDescriptor(uint16_t Length)
 {
	 return Standard_GetDescriptorData(Length, &Usb::pUsb->Config_Descriptor);
 }

 uint8_t* Usb::UsbGetStringDescriptor(uint16_t Length)
 {
	 uint8_t wValue0 = pInformation->USBwValue0;
	 if (wValue0 > 4)
	 {
		 return 0;
	 }
	 else
	 {
		 return Standard_GetDescriptorData(Length, &Usb::pUsb->String_Descriptor[wValue0]);
	 }
 }


 void Usb::UsbGetConfiguration()
 {

 }

 void Usb::UsbSetConfiguration()
 {
	 DEVICE_INFO *pInfo = &Device_Info;

	 if (pInfo->Current_Configuration != 0)
	 {
		 /* Device configured */
		 Usb::pUsb->bDeviceState = CONFIGURED;
		 ClearDTOG_TX(ENDP1);
		 ClearDTOG_TX(ENDP2);
		 ClearDTOG_RX(ENDP3);
	 }
 }

 void  Usb::UsbGetInterface()
 {

 }
 void  Usb::UsbSetInterface()
 {

 }

 void Usb::UsbGetStatus()
 {

 }

 void Usb::UsbClearFeature()
 {

 }

 void Usb::UsbSetEndPointFeature()
 {

 }

 void Usb::UsbSetDeviceFeature()
 {

 }

 void Usb::UsbSetDeviceAddress()
 {
	 Usb::pUsb-> bDeviceState = ADDRESSED;
 }


 void Usb::SendDataToUsb(unsigned char endpoint)
 {
	 if(endpoint == 0)
	 {
		 UserToPMABufferCopy(Usb::pUsb->TxBuffer, ENDP1_TXADDR, _txBytes);
		 SetEPTxCount(ENDP1, _txBytes);
		 SetEPTxValid(ENDP1);
	 }
	 else
	 {
		 UserToPMABufferCopy(Usb::pUsb->TxBuffer, ENDP1_TXADDR, _txBytes);
		 SetEPTxCount(ENDP1, _txBytes);
		 SetEPTxValid(ENDP1);
		 _txBytes=0;
	 }
 }

 void Usb::RecivedFromUsb(unsigned int endpoint)
 {
	 int rx = USB_SIL_Read(EP3_OUT, Usb::pUsb->RxBuffer+_rxBytes);
	  SetEPRxValid(ENDP3);
	 _rxBytes+= rx;
 }

 void Usb::SendData(const char* data, int length)
 {
	 memcpy(TxBuffer,data, strlen(data));
	 _txBytes = strlen(data);
 }


 int Usb::ReadData(char* mass)
 {
	 int length = _rxBytes;
	 memcpy(mass,RxBuffer,_rxBytes);
	 _rxBytes=0;
	 return length;
 }

 uint8_t* Usb::LineCodingStamp(uint16_t t)
 {
	 if (t == 0)
	 {
		 pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
		 return NULL;
	 }
	 return(uint8_t *)&linecoding;
 }

 void Usb::IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
 {
   uint8_t idx = 0;

   for( idx = 0 ; idx < len ; idx ++)
   {
     if( ((value >> 28)) < 0xA )
     {
       pbuf[ 2* idx] = (value >> 28) + '0';
     }
     else
     {
       pbuf[2* idx] = (value >> 28) + 'A' - 10;
     }

     value = value << 4;

     pbuf[ 2* idx + 1] = 0;
   }
 }


