/*
 * Dma.h
 *
 *  Created on: 09 ����� 2017 �.
 *      Author: Alekseu
 */

#ifndef DRIVER_DMA_H_
#define DRIVER_DMA_H_

#include "../platform.h"
#include "../Interface/DriverObject.h"
#include "../Interface/CommunicationObject.h"

namespace Driver
{

	/*
	 * �������� ������������ ��� ����� ���������, ���-�� ������ �� ������������ � ������
	 * ����������, ��� ��� �� ����� ������ ����� ���� ��� ������������ ����������
	 */




	class Dma :public DriverObject
	{
		DmaInit   DMA_InitStructure;


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


		unsigned char Channel;

		//todo ������� ��� �� ��� (���������� ��� ������) ��� �������� ���������� ����
		Dma();
		virtual ~Dma();


		void Init();


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

		const char* toString();

	};
}

#endif /* DRIVER_DMA_H_ */
