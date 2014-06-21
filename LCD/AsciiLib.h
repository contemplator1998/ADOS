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

#ifndef __AsciiLib_H
#define __AsciiLib_H  

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stm32f4xx.h>

uint16_t GetWidth(uint8_t* text);
uint8_t GetCharWidth(uint8_t c);
uint8_t GetCharHeight(void);
void GetCharSize(uint8_t* w, uint8_t* h, uint8_t c);
void SetFont(uint8_t num);
uint8_t GetFont(void);
unsigned char* GetASCIICode(char ASCII);

#endif 


/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
