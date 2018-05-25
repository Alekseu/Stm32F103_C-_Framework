///*
// * matrix.h
// *
// *  Created on: 06 сент. 2017 г.
// *      Author: hudienko_a
// */
//
//#ifndef DRIVER_MATRIX_LEDMATRIX_H_
//#define DRIVER_MATRIX_LEDMATRIX_H_
//#include "../platform.h"
//
//namespace Driver
//{
//
//
////extern "C" void TIM2_IRQHandler(void) __attribute__ ((signal));
//extern "C" void DMA1_Channel2_IRQHandler(void) ;
//
//	class LedMatrix
//	{
//		GpioInit  GPIO_InitStruct;
//
//
//		friend void DMA1_Channel2_IRQHandler();
//	    public:
//
//			enum MatrixType
//			{
//				Scan_1_4,
//				Scan_1_8,
//				Scan_1_16
//			};
//
//			enum MatrixGrayScale
//			{
//				Scale_2 =1, // два цвета черный и белый
//				Scale_8 =4 // 8 оттенков 2-бита
//
//			};
//
//
//
//		public:
//
//			static LedMatrix* LMObj;
//
//			LedMatrix(uint8_t width,uint8_t height,MatrixType type, MatrixGrayScale scale);
//
//			~LedMatrix();
//
//			void Init();
//
//			void ScreenSaverEnable();
//			void ScreenSaverDisable();
//
//			void DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
//
//			void DrawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
//
//			void DrawCircle(int16_t x0, int16_t y0, int16_t radius);
//
//			void DrawString(int16_t X, int16_t Y, uint8_t FontID, uint8_t *Str, uint8_t color);
//
//			void DrawString(int16_t X, int16_t Y, uint8_t FontID, uint8_t *Str, uint8_t len, uint8_t color);
//
//			void FillOutputBuffer();
//
//			void SetPixel(int x, int y, char data, char color);
//
//			void Clear()
//			{
//				for(int i=0;i<(_height*(_width/2));i++)
//				{
//					_matrixBuffer[i]=0;
//				}
//			}
//
//		protected:
//
//
//		private:
//
//			unsigned char framebuff;
//
//			uint8_t _width;
//			uint8_t _height;
//			uint8_t _currentRow;
//
//			MatrixType _type;
//
//			MatrixGrayScale _scale;
//
//			unsigned char* _matrixBuffer;
//
//			unsigned char _screenSaverText[15];
//			unsigned char _screenSaverFont=0;
//
//
//			bool _second_row;
//			bool _third_row;
//			bool _fourth_row;
//
//
//
//	};
//}
//
//
//
//#endif /* DRIVER_MATRIX_LEDMATRIX_H_ */
