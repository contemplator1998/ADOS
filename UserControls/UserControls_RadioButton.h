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

#ifndef USERCONTROLS_RADIOBUTTON_H
#define USERCONTROLS_RADIOBUTTON_H

#include <stdbool.h>
#include <LCD.h>
#include <stdlib.h>
#include <UserControls_MessageBox.h>

#define UC_MAX_RADIOBUTTON 70
#define UC_MAX_RADIOBUTTON_GROUP 10

typedef struct
{
	int x;
	int y;
	char* text;
	bool pressed;
	uint8_t group;
} UserControls_RadioButton_t;

// image 13 x 13 (W x H)
extern const unsigned int Radio1[];

// image 13 x 13 (W x H)
extern const unsigned int Radio2[];

void UserControls_RadioButton_Add(int x, int y, char* text, uint8_t group, bool pressed);
void UserControls_RadioButton_Set(int num, int group);
void UserControls_RadioButton_Clear(void);
void UserControls_RadioButton_Draw(int x, int y, char* text, bool pressed);
int8_t UserControls_RadioButton_Handle(int x, int y, int pressed);
char* UserControls_RadioButton_GetTextFromIndex(int8_t ind);
int UserControls_RadioButton_GetDigitFromIndex(int8_t ind);

#endif

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
