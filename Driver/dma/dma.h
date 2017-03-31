/*
 * Dma.h
 *
 *  Created on: 09 ����� 2017 �.
 *      Author: Alekseu
 */

#ifndef DRIVER_DMA_H_
#define DRIVER_DMA_H_

#include "../platform.h"

namespace Driver
{

	/*
	 * �������� ������������ ��� ����� ���������, ���-�� ������ �� ������������ � ������
	 * ����������, ��� ��� �� ����� ������ ����� ���� ��� ������������ ����������
	 */

	#define CHANNEL_1 1
	#define CHANNEL_2 2
	#define CHANNEL_3 3
	#define CHANNEL_4 4
	#define CHANNEL_5 5
	#define CHANNEL_6 6
	#define CHANNEL_7 7
	#define CHANNEL_8 8
	#define CHANNEL_9 9
	#define CHANNEL_10 10
	#define CHANNEL_11 11
	#define CHANNEL_12 12

	#define _SPI 1
	#define _UART 2
	#define _I2C 3



	class Dma
	{
		DMA_InitTypeDef   DMA_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
		unsigned char _channel;
		bool configured = false;
	public:

		static Dma* pDma0;
		static Dma* pDma1;
		static Dma* pDma2;
		static Dma* pDma3;
		static Dma* pDma4;
		static Dma* pDma5;
		static Dma* pDma6;
		static Dma* pDma7;
		static Dma* pDma8;
		static Dma* pDma9;
		static Dma* pDma10;
		static Dma* pDma11;
		static Dma* pDma12;

		//todo ������� ��� �� ��� (���������� ��� ������) ��� �������� ���������� ����
		Dma();
		virtual ~Dma();


		void Init(unsigned char channel );

		void InitPherif(void* pherif);

		/*
		 * ������ �������� ������ �� ��� (���������������� �����, ����������)
		 */
		virtual void HalfRecivedDmaComplete();

		/*
		 * ����� ����� ������ �� ���  (���������������� �����, ����������)
		 */
		virtual void RecivedDmaComplete();

		/*
		 * �������� �������� ������ ��� (���������������� �����, ���������� )
		 */
		virtual void HalfTransmitDmaComplete();

		/*
		 * �������� ����� ������ ��� (���������������� �����, ����������)
		 */
		virtual void TransmitDmaComplete();



		void MemCpy(char* src, char* dst, int length);


		/*
		 * ��������� ����� �� ���
		 */
		void StartReciveDma(int length);


		/*
		 * ����� ��� �� ��������
		 */
		void WriteArrayDma(unsigned char* array, int length);


		void StartTransmitArrayDmaCircle(unsigned char* src,  char* dst, int length);



	};
}

#endif /* DRIVER_DMA_H_ */
