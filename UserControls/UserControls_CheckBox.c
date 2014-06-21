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

#include <UserControls_CheckBox.h>

static UserControls_CheckBox_t _controls[UC_MAX_CHECKBOX];
static uint8_t _maxNumb = 0;
static uint8_t iterator;

void UserControls_CheckBox_Add(int x, int y, char* text, bool checked, int color)
{
	_controls[_maxNumb].x = x;
	_controls[_maxNumb].y = y;
	_controls[_maxNumb].text = text;
	_controls[_maxNumb].pressed = false;
	_controls[_maxNumb].checked = checked;
	_controls[_maxNumb].color = color;
	UserControls_CheckBox_Draw(x, y, text, checked, color);
	_maxNumb++;
	if (_maxNumb >= UC_MAX_CHECKBOX) 
	{
		MessageBox_Show("CheckBox count overflow", "Fatal Error", true, false);
		_maxNumb = UC_MAX_CHECKBOX-1;
	}
}

void UserControls_CheckBox_Clear(void)
{
	_maxNumb = 0;
}

void UserControls_CheckBox_Handle(int x, int y, int pressed)
{
	for (iterator = 0; iterator < _maxNumb; iterator++)
	{
		if (!pressed)
		{
				if (_controls[iterator].pressed)
				{
					_controls[iterator].pressed = pressed;
					_controls[iterator].checked = !_controls[iterator].checked;
					UserControls_CheckBox_Draw(_controls[iterator].x, 
											 _controls[iterator].y, 
											 _controls[iterator].text, 
											 _controls[iterator].checked,
											 _controls[iterator].color);
				}
		}
		else
		{
			if (x >= _controls[iterator].x && 
				x <= _controls[iterator].x + strlen(_controls[iterator].text)*6 + 20 &&
				y >= _controls[iterator].y && 
				y <= _controls[iterator].y + 13)
			{
				if (_controls[iterator].pressed != true)
				{
					_controls[iterator].pressed = true;
				}
			}
			else
			{
				if (_controls[iterator].pressed != false)
				{
					_controls[iterator].pressed = false;
				}
			}
		}
	}
}

bool UserControls_CheckBox_Get(int num)
{
	return _controls[num].checked;
}

void UserControls_CheckBox_Draw(int x, int y, char* text,bool pressed, int color)
{
	GUI_DrawImage(((pressed) ? Radio2 : Radio1), x, y, 13, 13);
	if (strlen(text) > 0) GUI_Text(x + 20, y+3, (uint8_t*)text, White, Black);
}
