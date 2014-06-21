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

#ifndef USERCONTROLS_BUTTON_H
#define USERCONTROLS_BUTTON_H

#include <LCD.h>
#include <stdbool.h>
#include <Core.h>
#include <UserControls_MessageBox.h>

#define UC_MAX_BUTTON 70
#define UC_BUTTON_HEIGHT 24
#define UC_BUTTON_HIPS_WIDTH 32

typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	char* text;
	bool pressed;
	uint16_t color;
	void (*f) (void);
} UserControls_Button_t;

void UserControls_Button_Add(int x, int y, char* text, int color, void (*f) (void));
void UserControls_Button_AddWithSize(int x, int y, char* text, int w, int h, int color, void (*f) (void));
void UserControls_Button_SizeOffset(uint16_t control, int offset_w, int offset_h);
void UserControls_Button_RedrawAll(void);
void UserControls_Button_RedrawControls(uint8_t numb);
void UserControls_Button_Clear(void);
void UserControls_Button_Handle(int x, int y, int pressed);
uint8_t UserControls_Button_GetControl(char* text);
uint8_t UserControls_Button_GetLastControl(void);
void UserControls_Button_Draw(int x, int y, char* text, int w, int h, int color, bool pressed);
void UserControls_Button_DrawSTDsize(int x, int y, char* text, int color, bool pressed);

#endif

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
