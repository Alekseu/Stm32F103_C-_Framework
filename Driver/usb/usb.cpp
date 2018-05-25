/*
 * UsbCom.cpp
 *
 *  Created on: 15 марта 2017 г.
 *      Author: hudienko_a
 */
#include "usb.h"

#include "../../Extention/delay.h"

namespace Driver
{

	#include <string.h>

#include "descriptors.h"



	Usb *Usb::pUsb = 0;
	uint8_t Report_Buf[wMaxPacketSize];
	uint32_t ProtocolValue;

	extern "C"
	{
		#include "../../StdPeriph/usb/inc/usb_bot.h"
		#include "../../StdPeriph/usb/inc/mass_mal.h"
		#include "../../StdPeriph/usb/inc/usb_sil.h"
		#include "keycodes.h"

		#define release_key()  set_key_buf(0,0)
		#define press_key(k)   press_key_mod(k, 0)

		__IO uint8_t PrevXferComplete = 1;

		u32 Max_Lun = 0;
		extern __IO BOOL fSuspendEnabled;  /* true when suspend is possible */
		//for massstorage
		extern Bulk_Only_CBW CBW;
		extern uint8_t Bot_State;
		/* MASS Storage Requests*/
		#define GET_MAX_LUN                0xFE
		#define MASS_STORAGE_RESET         0xFF
		#define LUN_DATA_LENGTH            1

		__IO uint16_t wIstr;  /* ISTR register last read value */
		__IO uint8_t bIntPackSOF = 0;  /* SOFs received between 2 consecutive packets */

		#define VCOMPORT_IN_FRAME_INTERVAL             2

		DEVICE Device_Table =
		{
				EP_NUM,
				1
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



		void KEYBOARD_SEND_key_buf(void) {

			USB_SIL_Write(EP1_IN, key_buf, 9);
			PrevXferComplete = 0;
			SetEPTxValid(ENDP1);
			while (PrevXferComplete == 0)
			{}

		}

		void KEYBOARD_SEND_Char(char ch) {
			press_key(ch);
			KEYBOARD_SEND_key_buf();
			release_key();
			KEYBOARD_SEND_key_buf();
		}

		void KEYBOARD_SEND_word(char *wrd){
			do {
				KEYBOARD_SEND_Char(*wrd);
			} while(*(++wrd));
		}

		void MOUSE_move(int8_t x, int8_t y){
			/*
			 * buf[0]: 1 - report ID
			 * buf[1]: bit2 - middle button, bit1 - right, bit0 - left
			 * buf[2]: move X
			 * buf[3]: move Y
			 * buf[4]: wheel
			 */

			buf[2] = x; buf[3] = y;
			USB_SIL_Write(EP1_IN, buf, 5);
			PrevXferComplete = 0;
			SetEPTxValid(ENDP1);

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

	}





	#define SEND_ENCAPSULATED_COMMAND   0x00
	#define GET_ENCAPSULATED_RESPONSE   0x01
	#define SET_COMM_FEATURE            0x02
	#define GET_COMM_FEATURE            0x03
	#define CLEAR_COMM_FEATURE          0x04
	#define SET_LINE_CODING             0x20
	#define GET_LINE_CODING             0x21
	#define SET_CONTROL_LINE_STATE      0x22
	#define SEND_BREAK                  0x23

	Usb::Usb()
	{
		RxBuffer=0;
		TxBuffer=0;
		RxBufferSize=32;
		TxBufferSize = 32;
		TxBytes = 0;
		RxBytes = 0;
		TypeUsb = VirtualComPort;
		TypeHid = Hid_manual;
		bDeviceState = UNCONNECTED;
		OnRecived=0;

	}

	void Usb::Init()
	{
		pUsb = this;

		MAL_Init(0);
		RxBuffer = new unsigned char[RxBufferSize];
		TxBuffer = new unsigned char[TxBufferSize];

		switch(TypeUsb)
		{
		case VirtualComPort:
			Device_Descriptor.Descriptor = (uint8_t*)_Virtual_Com_Port_Device_Descriptor;
			Device_Descriptor.Descriptor_Size = VIRTUAL_COM_PORT_SIZ_DEVICE_DESC;

			Config_Descriptor.Descriptor =(uint8_t*)_Virtual_Com_Port_ConfigDescriptor;
			Config_Descriptor.Descriptor_Size = VIRTUAL_COM_PORT_SIZ_CONFIG_DESC;

			String_Descriptor[0] = {(uint8_t*)Virtual_Com_Port_StringLangID, VIRTUAL_COM_PORT_SIZ_STRING_LANGID};
			String_Descriptor[1] = {(uint8_t*)Virtual_Com_Port_StringVendor, VIRTUAL_COM_PORT_SIZ_STRING_VENDOR};
			String_Descriptor[2] = {(uint8_t*)Virtual_Com_Port_StringProduct, VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT};
			String_Descriptor[3] = {(uint8_t*)Virtual_Com_Port_StringSerial, VIRTUAL_COM_PORT_SIZ_STRING_SERIAL};
			break;
		case HumanInterfaceDevice:
			switch(TypeHid)
			{
				case Hid_Mouse:
				case Hid_Keyboard:
					if(TypeHid==Hid_Mouse)
					{
						Device_Descriptor.Descriptor = (uint8_t*)RHID_DeviceDescriptor;
						Device_Descriptor.Descriptor_Size = RHID_SIZ_DEVICE_DESC;

						Config_Descriptor.Descriptor =(uint8_t*)RHID_MOUSE_ConfigDescriptor;
						Config_Descriptor.Descriptor_Size = RHID_SIZ_CONFIG_DESC;

						Report_Descriptor.Descriptor =(uint8_t*)RHID_MouseReportDescriptor;
						Report_Descriptor.Descriptor_Size = RHID_SIZ_MOUSE_REPORT_DESC;
					}

					if(TypeHid==Hid_Keyboard)
					{
						Device_Descriptor.Descriptor = (uint8_t*)RHID_DeviceDescriptor;
						Device_Descriptor.Descriptor_Size = RHID_SIZ_DEVICE_DESC;

						Config_Descriptor.Descriptor =(uint8_t*)RHID_KEYB_ConfigDescriptor;
						Config_Descriptor.Descriptor_Size = RHID_SIZ_CONFIG_DESC;

						Report_Descriptor.Descriptor =(uint8_t*)RHID_KeybReportDescriptor;
						Report_Descriptor.Descriptor_Size = RHID_SIZ_KEYB_REPORT_DESC;
					}

					String_Descriptor[0] = {(uint8_t*)RHID_StringLangID,RHID_SIZ_STRING_LANGID};
					String_Descriptor[1] = {(uint8_t*)RHID_StringVendor,RHID_SIZ_STRING_VENDOR};
					String_Descriptor[2] = {(uint8_t*)RHID_StringProduct,RHID_SIZ_STRING_PRODUCT};
					String_Descriptor[3] = {(uint8_t*)RHID_StringSerial,RHID_SIZ_STRING_SERIAL};
					String_Descriptor[4] = {(uint8_t*)SomeDev_StringInterface};
					break;
				case Hid_Joy:
//					Device_Descriptor.Descriptor = (uint8_t*)RHID_DeviceDescriptor;
//					Device_Descriptor.Descriptor_Size = RHID_SIZ_DEVICE_DESC;
//
//					Config_Descriptor.Descriptor =(uint8_t*)RHID_ConfigDescriptor;
//					Config_Descriptor.Descriptor_Size = RHID_SIZ_CONFIG_DESC;
//
//					Report_Descriptor.Descriptor =(uint8_t*)ARC_MOUSE_report_descriptor;
//					Report_Descriptor.Descriptor_Size = RHID_SIZ_REPORT_DESC;
//					String_Descriptor[0] = {(uint8_t*)RHID_StringLangID,RHID_SIZ_STRING_LANGID};
//										String_Descriptor[1] = {(uint8_t*)RHID_StringVendor,RHID_SIZ_STRING_VENDOR};
//										String_Descriptor[2] = {(uint8_t*)RHID_StringProduct,RHID_SIZ_STRING_PRODUCT};
//										String_Descriptor[3] = {(uint8_t*)RHID_StringSerial,RHID_SIZ_STRING_SERIAL};
//										String_Descriptor[4] = {(uint8_t*)SomeDev_StringInterface};
//					break;

				case Hid_manual:
					Device_Descriptor.Descriptor = (uint8_t*)SomeDev_DeviceDescriptor;
					Device_Descriptor.Descriptor_Size = SomeDev_SIZ_DEVICE_DESC;

					Config_Descriptor.Descriptor =(uint8_t*)SomeDev_ConfigDescriptor;
					Config_Descriptor.Descriptor_Size = SomeDev_SIZ_CONFIG_DESC;

					String_Descriptor[0] = {(uint8_t*)SomeDev_StringLangID};
					String_Descriptor[1] = {(uint8_t*)SomeDev_StringVendor};
					String_Descriptor[2] = {(uint8_t*)SomeDev_StringProduct};
					String_Descriptor[3] = {(uint8_t*)SomeDev_StringSerial};
					String_Descriptor[4] = {(uint8_t*)SomeDev_StringInterface};
				break;
			}
			break;
		case MassStorageDevice:

			Device_Descriptor.Descriptor = (uint8_t*)MASS_DeviceDescriptor;
			Device_Descriptor.Descriptor_Size = MASS_SIZ_DEVICE_DESC;

			Config_Descriptor.Descriptor =(uint8_t*)MASS_ConfigDescriptor;
			Config_Descriptor.Descriptor_Size = MASS_SIZ_CONFIG_DESC;

			String_Descriptor[0] = {(uint8_t*)MASS_StringLangID};
			String_Descriptor[1] = {(uint8_t*)MASS_StringVendor};
			String_Descriptor[2] = {(uint8_t*)MASS_StringProduct};
			String_Descriptor[3] = {(uint8_t*)MASS_StringSerial};
			String_Descriptor[4] = {(uint8_t*)MASS_StringInterface};
			break;
		}


		//set system
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

		/* Configure USB pull-up pin */
		GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

		//set usb clock
		RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

		/* Enable the USB clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

//		//usb interrupts
		InterruptController::PriorityGroupConfig(NVIC_PriorityGroup_1);
		InterruptController::SetHandler(USB_LP_CAN1_RX0_IRQn,USB_LP_CAN1_RX0_IRQHandler);

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
		Device_Property.MaxPacketSize = RxBufferSize;

		User_Standard_Requests.User_GetConfiguration = UsbGetConfiguration;
		User_Standard_Requests.User_SetConfiguration = UsbSetConfiguration;
		User_Standard_Requests.User_GetInterface = UsbGetInterface;
		User_Standard_Requests.User_SetInterface = UsbSetInterface;
		User_Standard_Requests.User_GetStatus = UsbGetStatus;
		User_Standard_Requests.User_ClearFeature = UsbClearFeature;
		User_Standard_Requests.User_SetEndPointFeature = UsbSetEndPointFeature;
		User_Standard_Requests.User_SetDeviceFeature = UsbSetDeviceFeature;
		User_Standard_Requests.User_SetDeviceAddress = UsbSetDeviceAddress;


//		NVIC_InitTypeDef NVIC_InitStructure;
//
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//
//		NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//		NVIC_Init(&NVIC_InitStructure);
		InterruptController::EnableChannel(USB_LP_CAN1_RX0_IRQn);

		USB_Init(&Device_Property,&User_Standard_Requests);


		int timeout = 1500;

		while (bDeviceState != CONFIGURED && --timeout)
		{
			_delay_ms(1);
		}

		//return timeout>0;
	}

	void Usb::UsbInit()
	{
		 pInformation->Current_Configuration = 0;

		 GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
		 _delay_ms(150);
		 GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
		 _delay_ms(10);

		 if(Usb::pUsb->TypeUsb!=MassStorageDevice)
		 {
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
		 }

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



		  if(Usb::pUsb->TypeUsb==MassStorageDevice)
		  {
			  SetBTABLE(BTABLE_ADDRESS);

			    /* Initialize Endpoint 0 */
			    SetEPType(ENDP0, EP_CONTROL);
			    SetEPTxStatus(ENDP0, EP_TX_NAK);
			    SetEPRxAddr(ENDP0, ENDP0_RXADDR);
			    SetEPRxCount(ENDP0, Usb::pUsb->Device_Property.MaxPacketSize);
			    SetEPTxAddr(ENDP0, ENDP0_TXADDR);
			    Clear_Status_Out(ENDP0);
			    SetEPRxValid(ENDP0);

			    /* Initialize Endpoint 1 */
			    SetEPType(ENDP1, EP_BULK);
			    SetEPTxAddr(ENDP1, ENDP1_TXADDR);
			    SetEPTxStatus(ENDP1, EP_TX_NAK);
			    SetEPRxStatus(ENDP1, EP_RX_DIS);

			    /* Initialize Endpoint 2 */
			    SetEPType(ENDP2, EP_BULK);
			    SetEPRxAddr(ENDP2, ENDP2_RXADDR);
			    SetEPRxCount(ENDP2, Usb::pUsb->Device_Property.MaxPacketSize);
			    SetEPRxStatus(ENDP2, EP_RX_VALID);
			    SetEPTxStatus(ENDP2, EP_TX_DIS);


			    SetEPRxCount(ENDP0, Usb::pUsb->Device_Property.MaxPacketSize);
			    SetEPRxValid(ENDP0);
			    CBW.dSignature = BOT_CBW_SIGNATURE;
			  	Bot_State = BOT_IDLE;
		  }
		  else  if(Usb::pUsb->TypeUsb==HumanInterfaceDevice && Usb::pUsb->TypeHid!=Hid_manual)
		  {

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
			   SetEPType(ENDP1, EP_INTERRUPT);
			   SetEPTxAddr(ENDP1, ENDP1_TXADDR_);
			   SetEPRxAddr(ENDP1, ENDP1_RXADDR_);
			   SetEPTxCount(ENDP1, EP1TxCount);
			   SetEPRxCount(ENDP1, EP1RxCount);
			   SetEPRxStatus(ENDP1, EP_RX_VALID);
			   SetEPTxStatus(ENDP1, EP_TX_NAK);
		  }
		  else
		  {
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
		  }

		  /* Set this device to response on default address */
		  SetDeviceAddress(0);

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
		 if(Usb::pUsb->TypeUsb==MassStorageDevice)
		 {
			 uint8_t    *(*CopyRoutine)(uint16_t);

			  CopyRoutine = NULL;
			  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
			      && (RequestNo == GET_MAX_LUN) && (pInformation->USBwValue == 0)
			      && (pInformation->USBwIndex == 0) && (pInformation->USBwLength == 0x01))
			  {
			    CopyRoutine = Get_Max_Lun;
			  }
			  else
			  {
			    return USB_UNSUPPORT;
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
		 else
			 if(Usb::pUsb->TypeUsb==HumanInterfaceDevice && Usb::pUsb->TypeHid!=Hid_manual )
			 {
				 uint8_t *(*CopyRoutine)(uint16_t);

				   CopyRoutine = NULL;
				   if ((RequestNo == GET_DESCRIPTOR)
				       && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
				       && (pInformation->USBwIndex0 == 0))
				   {
				     if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
				     {
				       CopyRoutine = UsbGetReportDescriptor;
				     }
				     else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE)
				     {
				       CopyRoutine = UsbGetHIDDescriptor;
				     }

				   } /* End of GET_DESCRIPTOR */

				   /*** GET_PROTOCOL, GET_REPORT, SET_REPORT ***/
				   else if ( (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) )
				   {
				     switch( RequestNo )
				     {
				     case GET_PROTOCOL:
				       CopyRoutine = UsbGetProtocolValue;
				       break;
				     case SET_REPORT:
				       CopyRoutine = UsbSetReport_Feature;
				       Usb::pUsb->Request = SET_REPORT;
				       break;
				     default:
				       break;
				     }
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

		else
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
	 }

	 RESULT Usb::UsbNoData_Setup(uint8_t RequestNo)
	 {
		 if(Usb::pUsb->TypeUsb==MassStorageDevice)
		 {
			 if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
					 && (RequestNo == MASS_STORAGE_RESET) && (pInformation->USBwValue == 0)
					 && (pInformation->USBwIndex == 0) && (pInformation->USBwLength == 0x00))
			 {
				 /* Initialize Endpoint 1 */
				 ClearDTOG_TX(ENDP1);

				 /* Initialize Endpoint 2 */
				 ClearDTOG_RX(ENDP2);

				 /*intialise the CBW signature to enable the clear feature*/
				 CBW.dSignature = BOT_CBW_SIGNATURE;
				 Bot_State = BOT_IDLE;

				 return USB_SUCCESS;
			 }
		 }
		 else
			 if(Usb::pUsb->TypeUsb==HumanInterfaceDevice&& Usb::pUsb->TypeHid!=Hid_manual)
			 {
				 if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
				       && (RequestNo == SET_PROTOCOL))
				   {
				     return UsbSetProtocol();
				   }

				   else
				   {
				     return USB_UNSUPPORT;
				   }
			 }
			 else
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

		 }

		 return USB_UNSUPPORT;
	 }

	 RESULT Usb::UsbSetProtocol(void)
	 {
	   uint8_t wValue0 = pInformation->USBwValue0;
	   ProtocolValue = wValue0;
	   return USB_SUCCESS;
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
	 uint8_t* Usb::UsbGetReportDescriptor(uint16_t Length)
	 {
	   return Standard_GetDescriptorData(Length, &Usb::pUsb->Report_Descriptor);
	 }
	 uint8_t* Usb::UsbGetHIDDescriptor(uint16_t Length)
	 {
	   return Standard_GetDescriptorData(Length, &Usb::pUsb->Config_Descriptor);
	 }

	 uint8_t* Usb::UsbGetProtocolValue(uint16_t Length)
	 {
	   if (Length == 0)
	   {
	     pInformation->Ctrl_Info.Usb_wLength = 1;
	     return NULL;
	   }
	   else
	   {
	     return (uint8_t *)(&ProtocolValue);
	   }
	 }

	 uint8_t* Usb::UsbSetReport_Feature(uint16_t Length)
	 {
	   if (Length == 0)
	   {
	     pInformation->Ctrl_Info.Usb_wLength = wMaxPacketSize;
	     return NULL;
	   }
	   else
	   {
	     return Report_Buf;
	   }
	 }

	 uint8_t* Usb::UsbGetStringDescriptor(uint16_t Length)
	 {
		 uint8_t wValue0 = pInformation->USBwValue0;
		 if (wValue0 > 5)
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
			 Bot_State = BOT_IDLE;

			 if(Usb::pUsb->TypeUsb!=MassStorageDevice)
			 {
				 ClearDTOG_RX(ENDP3);
			 }
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
		 if (CBW.dSignature != BOT_CBW_SIGNATURE)
		    Bot_Abort(BOTH_DIR);
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

	 uint8_t *Usb::Get_Max_Lun(uint16_t Length)
	 {
	   if (Length == 0)
	   {
	     pInformation->Ctrl_Info.Usb_wLength = LUN_DATA_LENGTH;
	     return 0;
	   }
	   else
	   {
	     return((uint8_t*)(&Max_Lun));
	   }
	 }

	 // прерывания конечных точек тут прийдется править код под свои нужды либо в классе наследнике определять свою логику
	 void Usb::SendDataToUsb(unsigned char endpoint)
	 {
		 if(TypeUsb == MassStorageDevice)
		 {
			 if( endpoint==1)
			 {
			  Mass_Storage_In();
			 }
			 return;
		 }
		 else
			 if(TypeUsb==HumanInterfaceDevice && TypeHid!=Hid_manual)
			 {
				 if( endpoint==1)
				 {
					 PrevXferComplete = 1;
				 }
			 }
		 else
		 {

			 if(endpoint == 0)
			 {
				 if(TxBytes>0)
				 {
					 UserToPMABufferCopy(TxBuffer, ENDP1_TXADDR, TxBytes);
					 SetEPTxCount(ENDP1, TxBytes);
					 SetEPTxValid(ENDP1);
					 TxBytes=0;
				 }
			 }
			 else
			 {
				 if(TxBytes>0)
				 {
					 UserToPMABufferCopy(TxBuffer, ENDP1_TXADDR, TxBytes);
					 SetEPTxCount(ENDP1, TxBytes);
					 SetEPTxValid(ENDP1);
					 TxBytes=0;
				}
			 }
		 }
	 }

	 // прерывания конечных точек тут прийдется править код под свои нужды либо в классе наследнике определять свою логику
	 void Usb::RecivedFromUsb(unsigned int endpoint)
	 {
		 if(TypeUsb==MassStorageDevice )
		 {
			 if(endpoint==2)
			 {
			   Mass_Storage_Out();
			 }
			 return;
		 }
		 else
		 			 if(TypeUsb==HumanInterfaceDevice && TypeHid!=Hid_manual)
		 			 {
		 				 if( endpoint==1)
		 				 {
		 					 PrevXferComplete = 1;
		 				 }
		 			 }

		 else
		 {
			 if(RxBytes+VIRTUAL_COM_PORT_DATA_SIZE>RxBufferSize) RxBytes =0;
			 int rx = USB_SIL_Read(EP3_OUT, RxBuffer+RxBytes);
			 SetEPRxValid(ENDP3);
			 RxBytes+= rx;
			 if(OnRecived!=0)
			 {
				 for(int i=0;i<RxBytes;i++)
				 {
					 OnRecived(RxBuffer[i]);
				 }
			 }
		 }
	 }


	 void Usb::SendData(uint8_t* data, uint16_t length)
	 {
		 if(TxBytes+length>TxBufferSize) TxBytes=0;
		 memcpy(TxBuffer,data, length);
		 TxBytes = length;
	 }

	 uint16_t Usb::ReadData(uint8_t* mass)
	 {
		 uint16_t length = RxBytes;
		 memcpy(mass,RxBuffer,RxBytes);
		 RxBytes=0;
		 return length;
	 }

	 uint8_t* Usb::LineCodingStamp(uint16_t t)
	 {
		 if (t == 0)
		 {
			 pInformation->Ctrl_Info.Usb_wLength = sizeof(Linecoding);
			 return NULL;
		 }
		 return(uint8_t *)&Usb::pUsb->Linecoding;
	 }

	 void Usb::Received(uint8_t byte)
	 {

	 }

	 uint8_t Usb::ReadByte(){return 0;}
	 uint8_t Usb::ReadByte(uint8_t addr){return 0;}
	 bool Usb::ReadByte(uint8_t* value, uint16_t timeOut){return false;}

	 void Usb::WriteByte(uint8_t byte){
		 if(TxBytes>TxBufferSize) TxBytes=0;
		 TxBuffer[TxBytes++] = byte;

		 if(TxBytes==64)
		 {
			 UserToPMABufferCopy(TxBuffer, ENDP1_TXADDR, TxBytes);
			 SetEPTxCount(ENDP1, TxBytes);
			 SetEPTxValid(ENDP1);
			 TxBytes=0;
		 }

	 }
	 void Usb::WriteByte(uint8_t byte, uint8_t addr)
	 {

	 }

	 uint16_t Usb::ReadWord(){return 0;}
	 void Usb::WriteWord(uint16_t word){}


	 /*
	  * Keyboard
	  */
	 void Usb::KeyboardSend(const char* msg)
	 {
		 if(bDeviceState == CONFIGURED)
		 {
		 KEYBOARD_SEND_word((char*)msg);
		 }
	 }

	 /*
	  * Mouse
	  */
	 void Usb::MouseMove(int8_t x, int8_t y)
	 {
		 if(bDeviceState==CONFIGURED)
		 {
		 MOUSE_move(x,y);
		 }
	 }
}

