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

#ifndef _USERCONTROLS_VALUEREGULATOR_H
#define _USERCONTROLS_VALUEREGULATOR_H

#include <LCD.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <UserControls_Button.h>

#define UC_MAX_VALUEREGULATOR 50

typedef struct
{
	int x;
	int y;
	char* text;
	int* value;
	int min;
	int max;
	bool pressed1;
	bool pressed2;
} UserControls_ValueRegulator_t;

void UserControls_ValueRegulator_Add(int x, int y, char* text, int* value, int min, int max);
void UserControls_ValueRegulator_Clear(void);
void UserControls_ValueRegulator_Handle(int x, int y, int pressed);

void UserControls_ValueRegulator_Draw(int x, int y, char* text, int* value, bool pressed1, bool pressed2);

#endif

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
