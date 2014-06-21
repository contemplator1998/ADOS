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

#include <UserControls_MessageBox.h>

static bool _visible = false;
static uint8_t _result = 0;
static char* _title = 0;
static char* _text = 0;
static uint8_t _textLen;
static uint8_t _titleLen;
static bool _buttEnbl[2] = { true, true };
static bool _but1Pressed = false;
static bool _but2Pressed = false;

bool MessageBox_GetVisible(void) 
{
	return _visible;
}

uint8_t MessageBox_GetResult(void)
{
	return _result;
}

void MessageBox_ResetResult(void)
{
	_result = 0;
}

void MessageBox_Handle(int x, int y, bool pressed)
{
	if (!pressed)
	{
		if (_but1Pressed)
		{
			_result = 1;
			_but1Pressed = false;
			_but2Pressed = false;
			UserControls_Button_DrawSTDsize(190, 248, "OK", ((_buttEnbl[0]) ? RGB(230,230,230) : RGB(100,100,100)), false);
			UserControls_Button_DrawSTDsize(240, 248, "Cancel", ((_buttEnbl[1]) ? RGB(230,230,230) : RGB(100,100,100)), false);
			MessageBox_Hide();
		}
		else if (_but2Pressed)
		{
			_result = 2;
			_but1Pressed = false;
			_but2Pressed = false;
			UserControls_Button_DrawSTDsize(190, 248, "OK", ((_buttEnbl[0]) ? RGB(230,230,230) : RGB(100,100,100)), false);
			UserControls_Button_DrawSTDsize(240, 248, "Cancel", ((_buttEnbl[1]) ? RGB(230,230,230) : RGB(100,100,100)), false);
			MessageBox_Hide();
		}
	}
	else
	{
		if (x >= 190 && x <= 190 + (2*6) + 32 && y >= 248 && y <= 248+UC_BUTTON_HEIGHT)
		{
			if (_but1Pressed != true)
			{
				_but1Pressed = true;
				UserControls_Button_DrawSTDsize(190, 248, "OK", ((_buttEnbl[0]) ? RGB(230,230,230) : RGB(100,100,100)), true);
			}
		}
		else if (_but1Pressed != false)
		{
			_but1Pressed = false;
			UserControls_Button_DrawSTDsize(190, 248, "OK", ((_buttEnbl[0]) ? RGB(230,230,230) : RGB(100,100,100)), false);				
		}
		
		if (x >= 240 && x <= 240 + (6*6) + 32 && y >= 248 && y <= 248+UC_BUTTON_HEIGHT)
		{
			if (_but2Pressed != true)
			{
				_but2Pressed = true;
				UserControls_Button_DrawSTDsize(240, 248, "Cancel", ((_buttEnbl[1]) ? RGB(230,230,230) : RGB(100,100,100)), true);
			}
		}
		else if (_but2Pressed != false)
		{
			_but2Pressed = false;
			UserControls_Button_DrawSTDsize(240, 248, "Cancel", ((_buttEnbl[1]) ? RGB(230,230,230) : RGB(100,100,100)), false);				
		}
	}
}

void MessageBox_Show_Procedure(void)
{
	int x1 = (320 - _titleLen*6) / 2;
	int	x2 = (320 - _textLen*6) / 2;
	int color1 = 0; // 230
	int color2 = 0; // 180
	int iter;
	_visible = true;
	
	for (color1 = 0; color1 <= 140; color1+=2)
	{
		LCD_FillRect(0,0, 320, 80, RGB(color1,color1,color1));
		LCD_FillRect(0,400,320,80, RGB(color1,color1,color1));
	}
	for (iter = 1; iter <= 200; iter++)
	{
		LCD_FillRect(0, iter, 320, 80, RGB(140,140,140));
		LCD_FillRect(0, 480-80-iter, 320, 80, RGB(140,140,140));
		GUI_Line(0, iter-1, 320, iter-1, RGB(iter+45,iter+45,iter+45));
		GUI_Line(0, 480-iter+1, 320, 480-iter+1, RGB(iter+45,iter+45,iter+45));
	}
	
	color1 = 140;
	color2 = 140;
	
	while (color1 < 230 || color2 < 180)
	{
		if (color1 < 230) color1++;
		if (color2 < 180) color2++;
		LCD_FillRect(0, 200, 320, 20, RGB(color1,color1,color1+20));
		LCD_FillRect(0, 220, 320, 60, RGB(color2,color2,color2));
	}
	GUI_Line(0,220,320,220,Black);
	GUI_Text(x1, 206, (uint8_t*)_title, Black, RGB(color1,color1,color1+20));
	GUI_Text(x2, 230, (uint8_t*)_text, Black, RGB(color2,color2,color2));
	UserControls_Button_DrawSTDsize(190, 248, "OK", ((_buttEnbl[0]) ? RGB(color1,color1,color1) : RGB(color2,color2,color2)), false);
	UserControls_Button_DrawSTDsize(240, 248, "Cancel", ((_buttEnbl[1]) ? RGB(color1,color1,color1) : RGB(color2,color2,color2)), false);
}

void MessageBox_Hide(void)
{
	int i;
	
	for (i = 180; i >= 0; i-=5)
	{
		LCD_Clear(RGB(i,i,i));
	}
	
	_visible = false;
}

void MessageBox_Show(char* text, char* title, bool btn1Enabled, bool btn2Enabled)
{
	_text = text;
	_title = title;
	_buttEnbl[0] = btn1Enabled;
	_buttEnbl[1] = btn2Enabled;
	_textLen = strlen(_text);
	_titleLen = strlen(_title);
	MessageBox_Show_Procedure();
	//while(_visible);
}

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
