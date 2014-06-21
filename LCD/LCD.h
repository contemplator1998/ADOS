/*
    Adaptive Operating System
    Copyright (C) 2014  Contemplator (Ivan Zaikov)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __GLCD_H 
#define __GLCD_H

#include "stm32f4xx.h"
#include "AsciiLib.h" 
#include "ILI9481.h"
#include <stdlib.h>

#define White          0xFFFF
#define Black          0x0000
#define Grey           RGB(247,247,247)
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0
#define Transparent    0x1234

#define RGB(red, green, blue) (int) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

// GUI Image
void GUI_DrawImage(const unsigned int* image, int x, int y, int w, int h);
void GUI_DrawInvertImage(const unsigned int* image, int x, int y, int w, int h);
void GUI_DrawImageWithReplace(const unsigned int* image, int x, int y, int w, int h, int color1, int color2);
void GUI_DrawImage_SD(char* path, int x, int y, int w, int h);
void GUI_DrawInvertImage_SD(char* path, int x, int y, int w, int h);
void GUI_DrawImageWithReplace_SD(char* path, int x, int y, int w, int h, int color1, int color2);

// GUI Clear
void GUI_SmartClear(int color);

// GUI Text
void GUI_PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor);
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);

// GUI Geometry
void GUI_Line( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void GUI_Circle(int x, int y, int r,int color);
void GUI_Rect(int x, int y, int w, int h, int color);
void GUI_RectEllipce(int x, int y, int w, int h, int color);
void GUI_FillRectEllipce(int x, int y, int w, int h, int color);
void GUI_FillCircle(int x, int y, int r,int color);

#endif 

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
