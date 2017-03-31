///*
// * Command.cpp
// *
// *  Created on: 07 марта 2017 г.
// *      Author: hudienko_a
// */
//
//#include "CommandProcessor.h"
//
//CommandProcessor* CommandProcessor::pObj=0;
//
//extern "C"
//{
////	void DMA1_Channel3_IRQHandler(void) // USART1_RX
////	{
////
////	  /* Test on DMA Transfer Complete interrupt */
////	  if (DMA_GetITStatus(DMA1_IT_TC3))
////	  {
////		/* Clear DMA Transfer Complete interrupt pending bit */
////		  DMA1->IFCR =DMA1_IT_TC3;
////
////		CommandProcessor::pObj->DmaReciveComplete();
////	  }
////
////	  if (DMA_GetITStatus(DMA1_IT_HT3))
////	 	  {
////	 		/* Clear DMA Transfer Complete interrupt pending bit */
////	 		  DMA1->IFCR =DMA1_IT_HT3;
////
////	 		CommandProcessor::pObj->DmaHalfReciveComplete();
////	 	  }
////
////	}
////
////
////	void DMA1_Channel2_IRQHandler(void) // USART1_RX
////		{
////
////		  /* Test on DMA Transfer Complete interrupt */
////		  if (DMA_GetITStatus(DMA1_IT_TC2))
////		  {
////			/* Clear DMA Transfer Complete interrupt pending bit */
////			  DMA1->IFCR =DMA1_IT_TC2;
////			  CommandProcessor::pObj->DmaTransferComplete();
////		  }
////
////		}
//
//}
