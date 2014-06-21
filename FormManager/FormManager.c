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

#include <FormManager.h>
#include <config.h>

FORMS _selectForm = FORM_MAIN;

//==================================================================================================
//											Menu
//==================================================================================================

int TIME_h;
int TIME_m;
int TIME_s;

static void _saveClockSettings(void)
{
	RTC_TimeRegulate(TIME_h, TIME_m, TIME_s);
}

static void _Form_Menu_Handle(int x, int y, bool pressed)
{
	if (MessageBox_GetVisible())
	{
		MessageBox_Handle(x,y,pressed);
	}
	else
	{
		UserControls_Button_Handle(x,y,pressed);
		UserControls_ValueRegulator_Handle(x,y,pressed);
	}
}

static void _Form_Main_Show(void);
static void _Form_Menu_Show(void)
{
	int x, y;
	_selectForm = FORM_MENU;
	UserControls_Button_Clear();
	UserControls_RadioButton_Clear();
	UserControls_ValueRegulator_Clear();
	UserControls_CheckBox_Clear();
	UserControls_ListBox_Clear();
	GUI_SmartClear(Black);
	
	UserControls_Button_AddWithSize(3, 412, "Calibrate (RESET & PAGE DOWN)", 230, 25, 
									RGB(235,235,255), TouchPanel_Calibrate);		
	UserControls_Button_AddWithSize(3, 442, "Sleep Mode (PAGE UP TO WAKE UP)", 230, 25, 
									RGB(235,235,255), PWR_GoToSleep);
	UserControls_Button_AddWithSize(252, 412, "OK",55,54, Green, _Form_Main_Show);
		
	/* Clock */
	x = 156; y = 2;
	Clock_GetTime(&TIME_h, &TIME_m, &TIME_s);
	UserControls_ValueRegulator_Add(x+27,y+21, "Hours  ", &TIME_h, 0, 23);
	UserControls_ValueRegulator_Add(x+27,y+47+2, "Minutes", &TIME_m, 0, 59);
	UserControls_ValueRegulator_Add(x+27,y+73+4, "Seconds", &TIME_s, 0, 59);	
	UserControls_Button_AddWithSize(x+27, y+99+6, "Save settings", 155-27, 25, RGB(240,240,250), 
									_saveClockSettings);
	GUI_RectEllipce(x+20, y, 162-20, 127+8, Blue); 
	GUI_Line(x+20, y+16, x+162, y+16, Blue);
	GUI_Text(x+62+15,y+4,"Clock",Blue,Black);
}


//==================================================================================================
//											Main
//==================================================================================================

static void delay(uint16_t val)
{
	uint16_t i = 10;
	while(val--) while(i--);
}

static void _loadPrevious(void)
{
	if (((*(__IO uint32_t*)FLASH_USER_START_ADDR) & 0x2FFE0000 ) == 0x20000000)
  {
		int i;
		typedef  void (*pFunction)(void);
		uint32_t JumpAddress;
		pFunction Jump_To_Application;
		//__disable_interrupt();  (????? ??? ??????? ?? ???????, ?? ???? ??? ??????????)
		__disable_irq ();
		SysTick->CTRL &= 0;
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, FLASH_USER_START_ADDR);
		// Jump to user application 
		JumpAddress = *(__IO uint32_t*) (FLASH_USER_START_ADDR + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		  for(i=0; i<8; i++)
			NVIC->ICER[i] = NVIC->IABR[i];

		// Initialise user application's Stack Pointer 
		__set_MSP(*(__IO uint32_t*) FLASH_USER_START_ADDR);
		Jump_To_Application();
  }
}

static void _startProgram(uint8_t* name)
{
	if (FlashFirmware(name)) _loadPrevious();
}

static void _btnStart(void)
{
	char text[50] = "\\Programs\\";
	strcat(text, (char*)UserControls_ListBox_GetText(0));
	_startProgram((uint8_t*)text);
}

void DrawClock(int CLOCK_X, int CLOCK_Y)
{
	int h=0;
	int m=0;
	int s=0;
	int i=0;
	int bat=0;
	uint8_t text[6];
	if (_selectForm != FORM_MAIN) return;
	Clock_GetTime(&h, &m, &s);
	while(bat==0)
	bat = (Read_Bat()-BAT_MIN)*32/(BAT_MAX-BAT_MIN);
	for (i = 2; i < 10; i++)
	{
		while(Read_Bat()==0);
		bat += (Read_Bat()-BAT_MIN)*32/(BAT_MAX-BAT_MIN);
	}
	bat /= i-1;
	
	if (bat > 32) bat = 32;
	if (bat < 0) bat = 0;

	text[0] = '0'+(h/10);
	text[1] = '0'+(h%10);
	text[2] = ':';
	text[3] = '0'+(m/10);
	text[4] = '0'+(m%10);
	text[5] = 0;
	if (bat == 32)
	{
		LCD_FillRect(CLOCK_X+2, CLOCK_Y, 33, 13, Green);
		LCD_FillRect(CLOCK_X, CLOCK_Y+4, 1, 6, Green);
	}
	else
	{
		int color;
		LCD_FillRect(CLOCK_X+2, CLOCK_Y, 33, 13, White);
		LCD_FillRect(CLOCK_X, CLOCK_Y+4, 1, 6, White);
		color = (bat < 10) ? Red : (bat < 20) ? Yellow : (bat < 32) ? Green : White;
		LCD_FillRect(CLOCK_X+3+32-bat, CLOCK_Y, bat, 13, color);
	}
	GUI_Rect(CLOCK_X+2, CLOCK_Y, 34, 14, Black);
	GUI_Rect(CLOCK_X-1, CLOCK_Y+3, 3, 8, Black);
	GUI_Text(CLOCK_X+5, CLOCK_Y+3, text, Black, Transparent);
}                                                                                                                                                                            

static void _Form_Main_Show(void)
{
	LCD_Led_Off();
	SysTick_Config(SystemCoreClock / 100);
	_selectForm = FORM_MAIN;
	UserControls_Button_Clear();
	UserControls_RadioButton_Clear();
	UserControls_ValueRegulator_Clear();
	UserControls_CheckBox_Clear();
	UserControls_ListBox_Clear();
	
	LCD_Clear(White);
	GUI_DrawImage_SD("\\System\\Graphics\\Startup.bin", 0,480-245,320,480);
	
	GUI_Text(10, 480-11, "v.1.0.0", Blue, Transparent);		
	UserControls_ListBox_Add(2,2,USERCONTROLS_LISTBOX_ITEMS_TEXT_LEN*7+10,220);
	UserControls_Button_AddWithSize(195, 10+(35*0), "Previous program",122-3,30, Green, 
									_loadPrevious);
	UserControls_Button_AddWithSize(195, 10+(35*1), "Select program",122-3,30, Blue, 
									_btnStart);
	UserControls_Button_AddWithSize(195, 10+(35*2), "Sleep Mode",122-3,30, RGB(255,200,0), 
									PWR_GoToSleep);
	UserControls_Button_AddWithSize(195, 10+(35*3), "Options",122-3,20, RGB(240,240,240), 
									_Form_Menu_Show);
	
	scan_programs();
	
	DrawClock(MAIN_CLOCK_X, MAIN_CLOCK_Y);
	LCD_Led_On();
}

static void _Form_Main_Handle(int x, int y, bool pressed)
{
	static bool last = false;
	if (pressed && last == false)
	{
		last = true;
	}
	if (!pressed) last = false;
	UserControls_ListBox_Handle(display.x, display.y, pressed);
	if (!UserControls_ListBox_Active())
	{
		UserControls_Button_Handle(display.x, display.y, pressed);
	}
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

void Form_Show(FORMS form)
{
	_selectForm = form;
	switch(form)
	{
		case FORM_MAIN: _Form_Main_Show(); break;
		case FORM_MENU: _Form_Menu_Show(); break;
	}
}

void Form_Handle(FORMS form)
{
	static bool wasPressed = false;
	if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)) { wasPressed = true; }
	if (wasPressed && GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
	{
		delay(50);
		wasPressed = false;
	}
	do { getDisplayPoint(&display, Read_Ads7846(), &matrix ); }
	while((display.y > 480 || display.x > 320) && (bool)!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5));
	//TP_DrawPoint(display.x,display.y);
	if (!wasPressed)
	{
		switch(form)
		{
			case FORM_MAIN: _Form_Main_Handle(display.x,display.y,(bool)!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)); break;
			case FORM_MENU: _Form_Menu_Handle(display.x,display.y,(bool)!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)); break;
		}
	}
}

FORMS Get_Select_Form(void)
{
	return _selectForm;
}

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
