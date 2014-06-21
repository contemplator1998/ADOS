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

#ifndef FORMMANAGER_H
#define FORMMANAGER_H

#include <LCD.h>
#include <PWR.h>
#include <string.h>
#include <stdbool.h>
#include <RTC_Time.h>
#include <TouchPanel.h>
#include <UserControls_Button.h>
#include <UserControls_RadioButton.h>
#include <UserControls_ValueRegulator.h>
#include <UserControls_MessageBox.h>
#include <UserControls_CheckBox.h>

#define MAIN_CLOCK_X 279
#define MAIN_CLOCK_Y (10+(35*3)+(20*1)+5)
#define BAT_MAX 1300
#define BAT_MIN 1100

typedef enum 
{
	FORM_MAIN,
	FORM_MENU
} FORMS;


void Form_Show(FORMS form);
void Form_Handle(FORMS form);
FORMS Get_Select_Form(void);
void DrawClock(int CLOCK_X, int CLOCK_Y);

#endif

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
