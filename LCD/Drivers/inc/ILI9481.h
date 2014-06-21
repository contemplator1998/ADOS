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

#ifndef __ILI9481_H 
#define __ILI9481_H 

#include <UserControls_MessageBox.h>
#include <LCD.h>

#define  MAX_X  320
#define  MAX_Y  480   

/* LCD operations */
void LCD_Initialization(void);
void LCD_Clear(uint16_t Color);	
void LCD_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

/* Pixel operations */
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);

/* Hardware Scroll */
uint16_t 	LCD_HardwareScroll(int val, int VERTICAL_SCROLL_AREA);
uint16_t 	LCD_HardwareScroll_Position(void);
void		LCD_HardwareScroll_Position_Reset(int VERTICAL_SCROLL_AREA);

/* Transfer operations */
void 		LCD_WriteIndex(uint16_t index);
void 		LCD_WriteData(uint16_t data);
void 		LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue);
uint16_t 	LCD_ReadData(void);
uint16_t 	LCD_ReadReg(uint16_t LCD_Reg);
void 		LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);

/* Modes */
void LCD_Enter_Sleep_Mode(void);
void LCD_Exit_Sleep_Mode(void);
void LCD_Enter_Normal_Mode(void);
void LCD_Enter_Invert_Mode(void);
void LCD_Exit_Invert_Mode(void);
void LCD_ON(void);
void LCD_OFF(void);
void LCD_Hardware_InvertMode_On(void);
void LCD_Hardware_InvertMode_Off(void);

/* Led */
void LCD_Led_On(void);
void LCD_Led_Off(void);

#endif 

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
