/*
 * GraphicTextFunctions.h
 *
 *  Created on: 07 февр. 2017 г.
 *      Author: hudienko_a
 */

#ifndef EXTENTIONS_GRAPHICTEXTFUNCTIONS_H_
#define EXTENTIONS_GRAPHICTEXTFUNCTIONS_H_

extern "C"
{
 #include "../fonts/font.h"
}

#include <stdlib.h>

extern unsigned char width;
extern unsigned char height;

//extern Settings _settings;
//extern unsigned char* ActiveBuffer;
//Основная функция прорисовки пикселя в графическом буфере
void disp1color_DrawPixel(uint8_t* buff,int16_t X, int16_t Y, uint8_t State, uint8_t color)
{

  // ѕровер€ем, находитс€ ли точка в поле отрисовки диспле€
  if ((X >= width) || (Y >= (height)) || (X < 0) || (Y < 0))
    return;

  uint8_t ByteIdxy = Y/8;
  uint8_t  ByteIdxx = X;

  uint8_t BitIdy = (Y%8); // ¬ысота относительно строки байт (0<=Y<=7)

  char tmp=0;
  tmp =  buff[(ByteIdxx+(ByteIdxy*width))];

  if (State)
  {
	  tmp|= (1 << BitIdy);
  }
  else
  {
	  tmp&= ~(1 << BitIdy);
  }
  buff[(ByteIdxx+(ByteIdxy*width))] = tmp;
}

void disp1color_DrawLine(uint8_t* buff,int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  const int16_t deltaX = abs(x2 - x1);
  const int16_t deltaY = abs(y2 - y1);
  const int16_t signX = x1 < x2 ? 1 : -1;
  const int16_t signY = y1 < y2 ? 1 : -1;

  int16_t error = deltaX - deltaY;

  disp1color_DrawPixel(buff,x2, y2, 1, 255);

  while (x1 != x2 || y1 != y2)
  {
    disp1color_DrawPixel(buff,x1, y1, 1,255);
    const int16_t error2 = error * 2;

    if (error2 > -deltaY)
    {
      error -= deltaY;
      x1 += signX;
    }
    if (error2 < deltaX)
    {
      error += deltaX;
      y1 += signY;
    }
  }
}

void disp1color_DrawRectangle(uint8_t* buff,int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  disp1color_DrawLine(buff,x1, y1, x1, y2);
  disp1color_DrawLine(buff,x2, y1, x2, y2);
  disp1color_DrawLine(buff,x1, y1, x2, y1);
  disp1color_DrawLine(buff,x1, y2, x2, y2);
}

void disp1color_DrawCircle(uint8_t* buff,int16_t x0, int16_t y0, int16_t radius)
{
  int x = 0;
  int y = radius;
  int delta = 1 - 2 * radius;
  int error = 0;

  while (y >= 0)
  {
    disp1color_DrawPixel(buff,x0 + x, y0 + y, 1,255);
    disp1color_DrawPixel(buff,x0 + x, y0 - y, 1,255);
    disp1color_DrawPixel(buff,x0 - x, y0 + y, 1,255);
    disp1color_DrawPixel(buff,x0 - x, y0 - y, 1,255);
    error = 2 * (delta + y) - 1;

    if (delta < 0 && error <= 0)
    {
      ++x;
      delta += 2 * x + 1;
      continue;
    }

    error = 2 * (delta - x) - 1;

    if (delta > 0 && error > 0)
    {
      --y;
      delta += 1 - 2 * y;
      continue;
    }

    ++x;
    delta += 2 * (x - y);
    --y;
  }
}

uint8_t disp1color_DrawChar(uint8_t* buff, int16_t X, int16_t Y, uint8_t FontID, uint8_t Char, uint8_t color)
{
  // ”казатель на подтабличку конкретного символа шрифта
  uint8_t *pCharTable = font_GetFontStruct(FontID, Char);
  uint8_t CharWidth = font_GetCharWidth(pCharTable);    // Ўирина символа
  uint8_t CharHeight = font_GetCharHeight(pCharTable);  // ¬ысота символа
  pCharTable += 2;

  if (FontID == FONTID_6X8M)
  {
    for (uint8_t row = 0; row < CharHeight; row++)
    {
      for (uint8_t col = 0; col < CharWidth; col++)
        disp1color_DrawPixel(buff,X + col, Y + row, pCharTable[row] & (1 << (7 - col)),color);

    }
  }
  else
  {
    for (uint8_t row = 0; row < CharHeight; row++)
    {
      for (uint8_t col = 0; col < CharWidth; col++)
      {
        if (col < 8)
          disp1color_DrawPixel(buff ,X + col, Y + row, pCharTable[row * 2] & (1 << (7 - col)),color);
        else
          disp1color_DrawPixel(buff,X + col, Y + row, pCharTable[(row * 2) + 1] & (1 << (15 - col)),color);
      }
    }
  }

  return CharWidth;
}

void disp1color_DrawString( uint8_t* buff, int16_t X, int16_t Y, uint8_t FontID, uint8_t *Str,uint8_t color)
{
  uint8_t done = 0;             // ‘лаг окончани€ вывода
  int16_t Xstart = X;           // «апоминаем куда будем переводить каретку при переходе на новую строку
  uint8_t StrHeight = 8;        // ¬ысота символов в пиксел€х дл€ перехода на слежующую строку

  // ¬ывод строки
  while (!done)
  {
    switch (*Str)
    {
    case '\0':  //  онец строки
      done = 1;
      break;
    case '\n':  // ѕереход на следующую строку
      Y += StrHeight;
      break;
    case '\r':  // ѕереход в начало строки
      X = Xstart;
      break;
    default:    // ќтображаемый символ
      X += disp1color_DrawChar(buff,X, Y, FontID, *Str, color);
      StrHeight = font_GetCharHeight(font_GetFontStruct(FontID, *Str));
      break;
    }
    Str++;
  }
}

void disp1color_DrawString( uint8_t* buff, int16_t X, int16_t Y, uint8_t FontID, uint8_t *Str, uint8_t len,uint8_t color)
{
  uint8_t done = 0;             // ‘лаг окончани€ вывода
  int16_t Xstart = X;           // «апоминаем куда будем переводить каретку при переходе на новую строку
  uint8_t StrHeight = 8;        // ¬ысота символов в пиксел€х дл€ перехода на слежующую строку

  // ¬ывод строки
  for(int i=0;i<len;i++)
  {
	  X += disp1color_DrawChar(buff,X, Y, FontID, *Str, color);
	        StrHeight = font_GetCharHeight(font_GetFontStruct(FontID, *Str));
    Str++;
  }

}


#endif /* EXTENTIONS_GRAPHICTEXTFUNCTIONS_H_ */
