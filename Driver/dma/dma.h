/*
 * Dma.h
 *
 *  Created on: 09 марта 2017 г.
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
	 * Внимание использовать дма нужно осторожно, что-бы каналы не пересикались с разной
	 * переферией, так как на одном канале может быть два переферийных устройства
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

		//todo указать что за дма (устройство или память) или передать аппаратный блок
		Dma();
		virtual ~Dma();


		void Init();


		void InitPherif(void* pherif);

		/*
		 * Приаем половины пакета по дма (переопределяемый метод, прерывание)
		 */
		virtual void HalfRecivedDmaComplete();

		/*
		 * Прием всего пакета по дма  (переопределяемый метод, прерывание)
		 */
		virtual void RecivedDmaComplete();

		/*
		 * передача половины пакета дма (переопределяемый метод, прерывание )
		 */
		virtual void HalfTransmitDmaComplete();

		/*
		 * Передача всего пакета дма (переопределяемый метод, прерывание)
		 */
		virtual void TransmitDmaComplete();



		void MemCpy(char* src, char* dst, int length);


		/*
		 * запустить прием по дма
		 */
		void StartReciveDma(int length);


		/*
		 * пнуть дма на передачу
		 */
		void WriteArrayDma(unsigned char* array, int length);


		void StartTransmitArrayDmaCircle(unsigned char* src,  char* dst, int length);

		const char* toString();

	};
}

#endif /* DRIVER_DMA_H_ */
