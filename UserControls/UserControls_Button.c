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

#include <UserControls_Button.h>

static UserControls_Button_t _controls[UC_MAX_BUTTON];
static uint8_t _maxNumb = 0;

void UserControls_Button_Add(int x, int y, char* text, int color, void (*f) (void))
{
	_controls[_maxNumb].x = x;
	_controls[_maxNumb].y = y;
	_controls[_maxNumb].w = GetWidth((uint8_t*)text) + UC_BUTTON_HIPS_WIDTH;
	_controls[_maxNumb].h = UC_BUTTON_HEIGHT;
	_controls[_maxNumb].text = text;
	_controls[_maxNumb].pressed = false;
	_controls[_maxNumb].color = color;
	_controls[_maxNumb].f = f;
	UserControls_Button_Draw(x, y, text, _controls[_maxNumb].w, _controls[_maxNumb].h, color, false);
	_maxNumb++;
	if (_maxNumb >= UC_MAX_BUTTON) 
	{
		MessageBox_Show("Buttons count overflow", "Fatal Error", true, false);
		_maxNumb = UC_MAX_BUTTON-1;
	}
}

void UserControls_Button_AddWithSize(int x, int y, char* text, int w, int h, int color, void (*f) (void))
{
	_controls[_maxNumb].x = x;
	_controls[_maxNumb].y = y;
	_controls[_maxNumb].w = w;
	_controls[_maxNumb].h = h;
	_controls[_maxNumb].text = text;
	_controls[_maxNumb].pressed = false;
	_controls[_maxNumb].color = color;
	_controls[_maxNumb].f = f;
	UserControls_Button_Draw(x, y, text, _controls[_maxNumb].w, _controls[_maxNumb].h, color, false);
	_maxNumb++;
	if (_maxNumb >= UC_MAX_BUTTON) 
	{
		MessageBox_Show("Buttons count overflow", "Fatal Error", true, false);
		_maxNumb = UC_MAX_BUTTON-1;
	}
}

void UserControls_Button_SizeOffset(uint16_t control, int offset_w, int offset_h)
{
	_controls[control].w += offset_w;
	_controls[control].h += offset_h;
}

void UserControls_Button_RedrawAll(void)
{
	uint8_t iterator;
	for (iterator = 0; iterator < _maxNumb; iterator++)
	{
		UserControls_Button_Draw(_controls[iterator].x, 
								 _controls[iterator].y,
								 _controls[iterator].text, 
								 _controls[iterator].w,
								 _controls[iterator].h,
								 _controls[iterator].color, 
								 false);
	}
}

void UserControls_Button_RedrawControls(uint8_t numb)
{
	UserControls_Button_Draw(_controls[numb].x, 
							 _controls[numb].y,
							 _controls[numb].text, 
							 _controls[numb].w,
							 _controls[numb].h,
							 _controls[numb].color, 
							 false);
}

void UserControls_Button_Clear(void)
{
	_maxNumb = 0;
}

void UserControls_Button_Handle(int x, int y, int pressed)
{
	uint8_t iterator;
	for (iterator = 0; iterator < _maxNumb; iterator++)
	{
		if (!pressed)
		{
				if (_controls[iterator].pressed)
				{
					_controls[iterator].pressed = pressed;
					UserControls_Button_Draw(_controls[iterator].x, 
											 _controls[iterator].y, 
											 _controls[iterator].text, 
											 _controls[iterator].w,
											 _controls[iterator].h,
											 _controls[iterator].color,
											 false);
					(*_controls[iterator].f)();
				}
		}
		else
		{
			if (x > _controls[iterator].x && 
				x < _controls[iterator].x + _controls[iterator].w &&
				y > _controls[iterator].y && 
				y < _controls[iterator].y + _controls[iterator].h)
			{
				if (_controls[iterator].pressed != true)
				{
					UserControls_Button_Draw(_controls[iterator].x, 
											 _controls[iterator].y, 
											 _controls[iterator].text, 
											 _controls[iterator].w,
											 _controls[iterator].h,
											 _controls[iterator].color,
											 true);
					_controls[iterator].pressed = true;
				}
			}
			else
			{
				if (_controls[iterator].pressed != false)
				{
					UserControls_Button_Draw(_controls[iterator].x, 
											 _controls[iterator].y, 
											 _controls[iterator].text, 
											 _controls[iterator].w,
											 _controls[iterator].h,
											 _controls[iterator].color,
											 false);
					_controls[iterator].pressed = false;
				}
			}
		}
	}
}

uint8_t UserControls_Button_GetControl(char* text)
{
	uint8_t iterator;
	for (iterator = 0; iterator < _maxNumb; iterator++)
	{
		if (strcmp(_controls[iterator].text, text))
		{
			return iterator;
		}
	}
	MessageBox_Show("Fuction: Button Get Control - Button not exist!", "Program Error", true, false);
	return 0;
}

uint8_t UserControls_Button_GetLastControl(void)
{
	MessageBox_Show("Fuction: Button Get Last Control - Collection of buttons is empty!", "Program Error", true, false);
	return _maxNumb-1;
}

void UserControls_Button_Draw(int x, int y, char* text, int w, int h, int color, bool pressed)
{
	if (text[0] != '\\')
	{
		GUI_FillRectEllipce(x, y,  w, h, (pressed) ? Black : color);
		GUI_RectEllipce(x, y,  w, h, (pressed) ? color : Grey);
		SetFont(1);
		GUI_Text(x+w/2-GetWidth((uint8_t*)text)/2,y+h/2-GetCharHeight()/2, (uint8_t*)text, (pressed) ? White : Black, (pressed) ? Black : color);
		SetFont(0);
	}
	else
	{
		GUI_DrawImage_SD(text, x, y, w, h);
		if (pressed)
		{
			GUI_Rect(x,y,w,h, Black);
		}
	}
}

void UserControls_Button_DrawSTDsize(int x, int y, char* text, int color, bool pressed)
{
	GUI_FillRectEllipce(x, y,  GetWidth((uint8_t*)text)+UC_BUTTON_HIPS_WIDTH, UC_BUTTON_HEIGHT, (pressed) ? Black : color);
	SetFont(1);
	GUI_Text(x+GetWidth((uint8_t*)text)/2+UC_BUTTON_HIPS_WIDTH/2-3,y+5, (uint8_t*)text, (pressed) ? White : Black, (pressed) ? Black : color);
	SetFont(0);
	GUI_RectEllipce(x, y, GetWidth((uint8_t*)text)+UC_BUTTON_HIPS_WIDTH, UC_BUTTON_HEIGHT, White);
}

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
