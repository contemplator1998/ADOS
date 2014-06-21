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

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <stdbool.h>
#include <string.h>
#include <LCD.h>
#include <UserControls_Button.h>

bool MessageBox_GetVisible(void);
uint8_t MessageBox_GetResult(void);
void MessageBox_ResetResult(void);
void MessageBox_Handle(int x, int y, bool pressed);
void MessageBox_Show_Procedure(void);
void MessageBox_Show(char* text, char* title, bool btn1Enabled, bool btn2Enabled);
void MessageBox_Hide(void);

#endif

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
