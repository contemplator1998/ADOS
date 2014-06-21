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

#include <LCD.h>
#include <flash.h>
#include <ff.h>
#include <diskio.h>
#include <UserControls_Button.h>
#include <UserControls_MessageBox.h>
#include <UserControls_CheckBox.h>
#include <UserControls_ValueRegulator.h>
#include <UserControls_RadioButton.h>
#include <UserControls_ListBox.h>
#include <Core.h>
#include <PWR.h>
#include <SDUser.h>
#include <TouchPanel.h>
#include <FormManager.h>

int main(void)
{
	PWR_InitButtons(); 	
	if (disk_initialize(0) == SD_OK)
	{
		f_mount(0, &fs);
	}
	
	/* Start display */
	LCD_Initialization();	
	LCD_Clear(Black);
	LCD_Led_On();
	
	/* TouchPanel Init */
	TP_Init();
	PWR_CheckStartupCalibrate();
	
	/* Real time clock Init */
	RTC_Time_Init();
	
	/* Show main form */
	Form_Show(Get_Select_Form());
				
	while(1)
	{
		Form_Handle(Get_Select_Form());
	}
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
