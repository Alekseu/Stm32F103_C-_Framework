/*
 * Pixel.h
 *
 *  Created on: 07 апр. 2017 г.
 *      Author: hudienko_a
 */

#ifndef PIXEL_H_
#define PIXEL_H_

#define MATRIX_WIDTH 128
#define MATRIX_HEIGHT 16
#define SCAN_TYPE 4

#define GRAY_SCALE 16
#define _FRAME_LENGTH (MATRIX_WIDTH*2)
#define FRAME_BUFFER_LENGTH  (_FRAME_LENGTH*GRAY_SCALE)

struct Pixel
		{
			union
			{
				struct
				{

					unsigned Red:5;
					unsigned Green:5;
					unsigned Blue:5;
				};
				unsigned  Data :16;
			};
		};




#endif /* PIXEL_H_ */
