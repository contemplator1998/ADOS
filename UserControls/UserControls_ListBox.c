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

#include <UserControls_ListBox.h>

static UserControls_ListBox_t _controls[USERCONTROLS_LISTBOX_MAX];
static int _last = 0;
static bool _active = false;

void UserControls_ListBox_Add(int x, int y, int width, int height)
{
	if (_last >= USERCONTROLS_LISTBOX_MAX) 
	{
		MessageBox_Show("ListBox count overflow", "Fatal Error", true, false);
		_last = USERCONTROLS_LISTBOX_MAX-1;
	}
	_controls[_last].x = x;
	_controls[_last].y = y;
	_controls[_last].width = width;
	_controls[_last].height = height;
	_controls[_last].items = 0;
	_controls[_last].page = 0;
	UserControls_ListBox_Draw(_last);
	_last++;
}

uint8_t UserControls_ListBox_PageCount(uint8_t numLB)
{
	return (_controls[numLB].items-1) / (_controls[numLB].height/22);
}

uint8_t UserControls_ListBox_PageItems(uint8_t numLB)
{
	uint8_t ans = 0;
	ans = _controls[numLB].height/22;
	if (ans > _controls[numLB].items - _controls[numLB].page*ans)
	{
		ans = _controls[numLB].items - _controls[numLB].page*ans;
	}
	return ans;
}

void UserControls_ListBox_NextPage(uint8_t numLB)
{
	if (_controls[numLB].page >= UserControls_ListBox_PageCount(numLB)) return;
	_controls[numLB].page++; 
	UserControls_ListBox_Draw(numLB);
}

void UserControls_ListBox_PrevPage(uint8_t numLB)
{
	if (_controls[numLB].page <= 0) return;
	_controls[numLB].page--; 
	UserControls_ListBox_Draw(numLB);
}

uint16_t px=500,py = 500,ux=0,uy=500;
void UserControls_ListBox_Handle(int x, int y, bool pressed)
{
	int _iterator = 0;
	if (pressed == false) 
	{
		if (px == 500 || py == 500 || ux == 500 || uy == 500) return;
		_active = false;
		for (_iterator = 0; _iterator < _last; _iterator++)
		{
			if (px > _controls[_iterator].x && ux > _controls[_iterator].x && 
					px < _controls[_iterator].x + _controls[_iterator].width &&
					ux < _controls[_iterator].x + _controls[_iterator].width &&
					py > _controls[_iterator].y && uy > _controls[_iterator].y &&
					py < _controls[_iterator].y + _controls[_iterator].height && 
					uy < _controls[_iterator].y + _controls[_iterator].height)
			{
				LCD_FillRect(_controls[_iterator].x, _controls[_iterator].y + UserControls_ListBox_PageItems(_iterator)*22, _controls[_iterator].width/2, 3, White);
				LCD_FillRect(_controls[_iterator].x + _controls[_iterator].width/2, _controls[_iterator].y + UserControls_ListBox_PageItems(_iterator)*22, _controls[_iterator].width/2, 3, White);
				if (px < ux)
				{
					if (ux - px >= _controls[_iterator].width/2) UserControls_ListBox_PrevPage(_iterator);
					px = 500; py = 500; ux = 500; uy = 500;
				}
				else
				{
					if (px - ux >= _controls[_iterator].width/2) UserControls_ListBox_NextPage(_iterator);
					px = 500; py = 500; ux = 500; uy = 500;
				}
			}
		}
	}
	else
	{
		for (_iterator = 0; _iterator < _last; _iterator++)
		{
			if (x > _controls[_iterator].x && x < _controls[_iterator].x + _controls[_iterator].width &&
					y > _controls[_iterator].y && y < _controls[_iterator].y + UserControls_ListBox_PageItems(_iterator)*22)
			{
				_active = true;
				if (y <= _controls[_iterator].y+(UserControls_ListBox_PageItems(_iterator))*22)
				{
					if (px == 500 || py == 500)
					{
						px = x;
						py = y;
					}
					else
					{
						ux = x;
						uy = y;
						if (px < ux)
						{
							if (ux - px >= _controls[_iterator].width/2) 
							{
								LCD_FillRect(_controls[_iterator].x, _controls[_iterator].y + UserControls_ListBox_PageItems(_iterator)*22, _controls[_iterator].width/2, 3, Red);
							}
							else
							{
								LCD_FillRect(_controls[_iterator].x, _controls[_iterator].y + UserControls_ListBox_PageItems(_iterator)*22, _controls[_iterator].width/2, 3, White);
							}
						}
						else
						{
							if (px - ux >= _controls[_iterator].width/2)
							{
								LCD_FillRect(_controls[_iterator].x + _controls[_iterator].width/2, _controls[_iterator].y + UserControls_ListBox_PageItems(_iterator)*22, _controls[_iterator].width/2, 3, Red);
							}
							else
							{
								LCD_FillRect(_controls[_iterator].x + _controls[_iterator].width/2, _controls[_iterator].y + UserControls_ListBox_PageItems(_iterator)*22, _controls[_iterator].width/2, 3, White);
							}
						}
					}
					if (_controls[_iterator].selected != (y-_controls[_iterator].y)/22)
					{
						int temp = _controls[_iterator].selected;
						_controls[_iterator].selected = (y-_controls[_iterator].y)/22;
						UserControls_ListBox_Item_Draw(_iterator, temp);
						UserControls_ListBox_Item_Draw(_iterator, _controls[_iterator].selected);
					}
				}
			}
		}
	}
}

void UserControls_ListBox_Items_Add(int numListBox, char* text)
{
	if (strlen(text) > USERCONTROLS_LISTBOX_ITEMS_TEXT_LEN)
	{
		MessageBox_Show("The file name is too long","Error", true, false);
		return;
	}
	strcpy(_controls[numListBox].text[_controls[numListBox].items], text);
	_controls[numListBox].items++;	
	if (_controls[numListBox].items <= _controls[numListBox].height / 22)
	{
		UserControls_ListBox_Item_Draw(numListBox, _controls[numListBox].items-1);
	}
}

void UserControls_ListBox_Item_Draw(int numLB, int num)
{
	if (num < UserControls_ListBox_PageItems(numLB)) 
	{
		bool active = (num == _controls[numLB].selected);
		if (num==0)
		{
			GUI_FillRectEllipce(_controls[numLB].x, _controls[numLB].y + num*22, 
													_controls[numLB].width, 20, (active ? Blue : White));
			GUI_RectEllipce(_controls[numLB].x, _controls[numLB].y + num*22, 
													_controls[numLB].width, 20, Blue);
		}
		else
		{
			GUI_FillRectEllipce(_controls[numLB].x, _controls[numLB].y + num*22 + 1, 
									 _controls[numLB].width, 20 - 1, (active ? Blue : White));
			GUI_RectEllipce(_controls[numLB].x, _controls[numLB].y + num*22 + 1, 
									 _controls[numLB].width, 20 - 1, Blue);
		}
		
		
		SetFont(1);
		GUI_Text(_controls[numLB].x+2, _controls[numLB].y + 6 + num*22 - 3,
						 (uint8_t*)_controls[numLB].text[num+_controls[numLB].page*(_controls[numLB].height / 22)], Black, Transparent);
		SetFont(0);
	}
}

void UserControls_ListBox_Draw(int numListBox)
{
	int _iterator = 0;
	const int offset = _controls[numListBox].page*(_controls[numListBox].height / 22);
	GUI_FillRectEllipce(_controls[numListBox].x, _controls[numListBox].y, _controls[numListBox].width,
				 _controls[numListBox].height, White);
	//GUI_RectEllipce(_controls[numListBox].x, _controls[numListBox].y, _controls[numListBox].width,
	//			 _controls[numListBox].height, Black);
	for (_iterator = offset; _iterator < _controls[numListBox].height / 22 + offset &&
			 _iterator < _controls[numListBox].items; _iterator++)
	{
		UserControls_ListBox_Item_Draw(numListBox, _iterator-offset);
	}
}

uint8_t* UserControls_ListBox_GetText(uint8_t num)
{
	return (uint8_t*)_controls[num].text[_controls[num].selected+
	(_controls[num].page*(_controls[num].height/22))];
}

void UserControls_ListBox_Clear()
{
	int _iterator = 0;
	for (_iterator = 0; _iterator < _last; _iterator++) _controls[_iterator].items = 0;
	_last = 0;
}

bool UserControls_ListBox_Active(void)
{
	return _active;
}

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
