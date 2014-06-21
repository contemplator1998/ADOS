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

#ifndef USERCONTROLS_LISTBOX_H
#define USERCONTROLS_LISTBOX_H

#include <stdbool.h>
#include <LCD.h>
#include <Core.h>

#define USERCONTROLS_LISTBOX_MAX 2
#define USERCONTROLS_LISTBOX_ITEMS_MAX 20
#define USERCONTROLS_LISTBOX_ITEMS_TEXT_LEN 25

typedef struct 
{
	uint8_t selected, items, page;
	int x, y, width, height;
	char text[USERCONTROLS_LISTBOX_ITEMS_MAX][USERCONTROLS_LISTBOX_ITEMS_TEXT_LEN]; 
} UserControls_ListBox_t;

void UserControls_ListBox_Add(int x, int y, int width, int height);
void UserControls_ListBox_Item_Draw(int numLB, int num);
void UserControls_ListBox_Handle(int x, int y, bool pressed);
void UserControls_ListBox_Items_Add(int numListBox, char* text);
void UserControls_ListBox_Draw(int numListBox);
uint8_t* UserControls_ListBox_GetText(uint8_t num);
void UserControls_ListBox_Clear(void);
bool UserControls_ListBox_Active(void);

#endif

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
