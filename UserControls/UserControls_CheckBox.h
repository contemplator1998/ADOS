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

#ifndef USERCONTROLS_CHECKBOX_H
#define USERCONTROLS_CHECKBOX_H

#include <LCD.h>
#include <stdbool.h>
#include <UserControls_MessageBox.h>

#define UC_MAX_CHECKBOX 70

typedef struct
{
	int x;
	int y;
	char* text;
	bool pressed;
	bool checked;
	int color;
	//void (*f) (bool pressed);
} UserControls_CheckBox_t;

// image 13 x 13 (W x H)
extern const unsigned int Radio1[];

// image 13 x 13 (W x H)
extern const unsigned int Radio2[];

void UserControls_CheckBox_Add(int x, int y, char* text, bool checked, int color);
void UserControls_CheckBox_Clear(void);
void UserControls_CheckBox_Handle(int x, int y, int pressed);
bool UserControls_CheckBox_Get(int num);

void UserControls_CheckBox_Draw(int x, int y, char* text,bool checked, int color);

#endif

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
