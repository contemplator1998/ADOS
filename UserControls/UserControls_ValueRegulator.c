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

#include <UserControls_ValueRegulator.h>

static UserControls_ValueRegulator_t _controls[UC_MAX_VALUEREGULATOR];
static uint8_t _maxNumb = 0;
static uint8_t iterator;
static char _totalText[50];
static uint8_t _lenTotalText;
static uint8_t _lastLenTotalText;

void UserControls_ValueRegulator_Add(int x, int y, char* text, int* value, int min, int max)
{
	_controls[_maxNumb].x = x;
	_controls[_maxNumb].y = y;
	_controls[_maxNumb].text = text;
	_controls[_maxNumb].pressed1 = false;
	_controls[_maxNumb].pressed2 = false;
	_controls[_maxNumb].value = value;
	_controls[_maxNumb].min = min;
	_controls[_maxNumb].max = max;
	_lenTotalText = _lastLenTotalText = 0;
	UserControls_ValueRegulator_Draw(x, y, text, value, false, false);
	_maxNumb++;
	if (_maxNumb >= UC_MAX_VALUEREGULATOR) 
	{
		MessageBox_Show("Value Regulators count overflow", "Overflow", true, false);
		_maxNumb = UC_MAX_VALUEREGULATOR-1;
	}
}

void UserControls_ValueRegulator_Clear(void)
{
	_maxNumb = 0;
}

void UserControls_ValueRegulator_Handle(int x, int y, int pressed)
{
	for (iterator = 0; iterator < _maxNumb; iterator++)
	{
		if (!pressed)
		{
			if (_controls[iterator].pressed1)
			{
				if ((*_controls[iterator].value) > _controls[iterator].min)
				{
					(*_controls[iterator].value)--;
				}
				_controls[iterator].pressed1 = false;
				_controls[iterator].pressed2 = false;
				UserControls_ValueRegulator_Draw(_controls[iterator].x,
												 _controls[iterator].y,
												 _controls[iterator].text,
												 _controls[iterator].value,
												 false,
												 false);
			}
			else if (_controls[iterator].pressed2)
			{
				if ((*_controls[iterator].value) < _controls[iterator].max)
				{
					(*_controls[iterator].value)++;
				}
				_controls[iterator].pressed1 = false;
				_controls[iterator].pressed2 = false;
				UserControls_ValueRegulator_Draw(_controls[iterator].x,
												 _controls[iterator].y,
												 _controls[iterator].text,
												 _controls[iterator].value,
												 false,
												 false);
			}
		}
		else
		{
			if (x > _controls[iterator].x+_lenTotalText*6+5 && 
				x < _controls[iterator].x+_lenTotalText*6+11+24 &&
				y > _controls[iterator].y && 
				y < _controls[iterator].y + UC_BUTTON_HEIGHT)
			{
				if (_controls[iterator].pressed1 != true)
				{
					_controls[iterator].pressed1 = true;
					UserControls_ValueRegulator_Draw(_controls[iterator].x,
													 _controls[iterator].y,
													 _controls[iterator].text,
													 _controls[iterator].value,
													 true,
													 false);
				}
			}
			else
			{
				if (_controls[iterator].pressed1 != false)
				{
					_controls[iterator].pressed1 = false;
					UserControls_ValueRegulator_Draw(_controls[iterator].x,
													 _controls[iterator].y,
													 _controls[iterator].text,
													 _controls[iterator].value,
													 false,
													 false);
				}
			}
			
			if (x > _controls[iterator].x+_lenTotalText*6+11+24 && 
				x < _controls[iterator].x+_lenTotalText*6+16+26+24 &&
				y > _controls[iterator].y && 
				y < _controls[iterator].y + UC_BUTTON_HEIGHT)
			{
				if (_controls[iterator].pressed2 != true)
				{
					_controls[iterator].pressed2 = true;
					UserControls_ValueRegulator_Draw(_controls[iterator].x,
													 _controls[iterator].y,
													 _controls[iterator].text,
													 _controls[iterator].value,
													 false,
													 true);
				}
			}
			else
			{
				if (_controls[iterator].pressed2 != false)
				{
					_controls[iterator].pressed2 = false;
					UserControls_ValueRegulator_Draw(_controls[iterator].x,
													 _controls[iterator].y,
													 _controls[iterator].text,
													 _controls[iterator].value,
													 false,
													 false);
				}
			}
		}
	}
}

void UserControls_ValueRegulator_Draw(int x, int y, char* text, int* value, bool pressed1, bool pressed2)
{
	sprintf(_totalText, "%s: %02d", text, *value);
	_lenTotalText = strlen(_totalText);
	if (_lenTotalText != _lastLenTotalText)
	{
		GUI_FillRectEllipce(x, y, _lastLenTotalText*6+16+26+26, UC_BUTTON_HEIGHT, Black);
		_lastLenTotalText = _lenTotalText;
		
	}
	GUI_RectEllipce(x,y,_lenTotalText*6+5,UC_BUTTON_HEIGHT,White);
	UserControls_Button_Draw(x+_lenTotalText*6+9, y, "-",24,24, RGB(230,230,230), pressed1);
	UserControls_Button_Draw(x+_lenTotalText*6+11+27, y, "+",24,24, RGB(230,230,230), pressed2);
	GUI_Text(x+3, y+8, (uint8_t*)_totalText, White, Black);
}

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
