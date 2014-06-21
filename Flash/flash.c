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

#include "flash.h"

uint32_t StartSector = 0, EndSector = 0, Address = 0, i = 0 ;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0 ;

/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }

  return sector;
}

static uint32_t settingsAddress;
bool FlashFirmware(uint8_t* Firmware_Name)
{
	u8 buffer[1024];
	UINT btr = 1024;
	UINT br = 0;
	UINT index = 0;
	uint32_t writeAddress = FLASH_USER_START_ADDR;
	uint32_t flashValue = 0;
	settingsAddress = FLASH_USER_SECTOR_START_ADDR;

	if (disk_initialize(0) == SD_OK)
	{
		f_mount(0, &fs);
	}
	if (f_open(&fsrc, (const TCHAR*)Firmware_Name, FA_READ) != FR_OK)
	{
		while(1);
	}
	res = f_read(&fsrc, &buffer, 4, &br);

	if ((((uint32_t)buffer[0] | 
		(uint32_t)buffer[1] << 8 | 
		(uint32_t)buffer[2] << 16 | 
		(uint32_t)buffer[3] << 24) & 0x2FFE0000 ) != 0x20000000)
	{
		res = f_close(&fsrc);
		return false;
	}
	res = f_close(&fsrc);
	if (f_open(&fsrc, (const TCHAR*)Firmware_Name, FA_READ) != FR_OK)
	{
		while(1);
	}
	
	/* Unlock the Flash to enable the flash control register access *************/ 
  FLASH_Unlock();
    
  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

  /* Get the number of the start and end sectors */
  StartSector = GetSector(FLASH_USER_SECTOR_START_ADDR);
  EndSector = GetSector(FLASH_USER_START_ADDR + fsrc.fsize / 4 + 1);

  for (i = StartSector; i <= EndSector; i += 8)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (FLASH_EraseSector(i, VoltageRange_2) != FLASH_COMPLETE)
    { 
      /* Error occurred while sector erase. 
         User can add here some code to deal with this error  */
      while (1)
      {
      }
    }
  }
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// LCD - Driver
	////////////////////////////////////////////////////////////////////////////////////////////////
	// LCD operations
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_Initialization); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_Clear); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_FillRect); settingsAddress += 4;
	// Pixel operations
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_SetPoint); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_GetPoint); settingsAddress += 4;
	// Hardware Scroll
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_HardwareScroll); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_HardwareScroll_Position); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_HardwareScroll_Position_Reset); settingsAddress += 4;
	// Transfer operations
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_WriteIndex); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_WriteData); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_WriteReg); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_ReadData); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_ReadReg); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_SetCursor); settingsAddress += 4;
	// Modes
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_Enter_Sleep_Mode); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_Exit_Sleep_Mode); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_Enter_Normal_Mode); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_Enter_Invert_Mode); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_Exit_Invert_Mode); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_ON); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_OFF); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_Hardware_InvertMode_On); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_Hardware_InvertMode_Off); settingsAddress += 4;
	// Led
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_Led_On); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&LCD_Led_Off); settingsAddress += 4;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// LCD
	////////////////////////////////////////////////////////////////////////////////////////////////
	//// GUI Image
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_DrawImage); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_DrawInvertImage); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_DrawImageWithReplace); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_DrawImage_SD); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_DrawInvertImage_SD); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_DrawImageWithReplace_SD); settingsAddress += 4;
	//// GUI Clear
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_SmartClear); settingsAddress += 4;
	//// GUI Text
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_PutChar); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_Text); settingsAddress += 4;
	//// GUI Geometry
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_Line); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_Circle); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_Rect); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_RectEllipce); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_FillRectEllipce); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&GUI_FillCircle); settingsAddress += 4;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// FATFS
	////////////////////////////////////////////////////////////////////////////////////////////////
	//// DISKIO
	//FLASH_ProgramWord(settingsAddress, (uint32_t)&assign_drives); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&disk_initialize); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&disk_status); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&disk_read); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&disk_write); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&disk_ioctl); settingsAddress += 4;
	//// FF
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_mount); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_open); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_read); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_lseek); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_close); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_opendir); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_readdir); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_stat); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_write); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_getfree); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_truncate); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_sync); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_unlink); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_mkdir); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_chmod); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_utime); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_rename); settingsAddress += 4;
	//FLASH_ProgramWord(settingsAddress, (uint32_t)&f_chdrive); settingsAddress += 4;
	//FLASH_ProgramWord(settingsAddress, (uint32_t)&f_chdir); settingsAddress += 4;
	//FLASH_ProgramWord(settingsAddress, (uint32_t)&f_getcwd); settingsAddress += 4;
	//FLASH_ProgramWord(settingsAddress, (uint32_t)&f_getlabel); settingsAddress += 4;
	//FLASH_ProgramWord(settingsAddress, (uint32_t)&f_setlabel); settingsAddress += 4;
	//FLASH_ProgramWord(settingsAddress, (uint32_t)&f_forward); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_mkfs); settingsAddress += 4;
	//FLASH_ProgramWord(settingsAddress, (uint32_t)&f_fdisk); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_putc); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_puts); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_printf); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&f_gets); settingsAddress += 4;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// TouchPanel
	////////////////////////////////////////////////////////////////////////////////////////////////	
	FLASH_ProgramWord(settingsAddress, (uint32_t)&TP_Init); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&Read_Ads7846); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&TouchPanel_Calibrate); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&DrawCross); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&TP_DrawPoint); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&Read_Bat); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&TP_SwitchOff); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&setCalibrationMatrix); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&getDisplayPoint); settingsAddress += 4;	
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// UserControls Button
	////////////////////////////////////////////////////////////////////////////////////////////////	
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_Button_Add); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_Button_SizeOffset); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_Button_RedrawAll); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_Button_RedrawControls); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_Button_Clear); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_Button_Handle); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_Button_GetControl); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_Button_GetLastControl); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_Button_Draw); settingsAddress += 4;

	////////////////////////////////////////////////////////////////////////////////////////////////
	// UserControls CheckBox
	////////////////////////////////////////////////////////////////////////////////////////////////
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_CheckBox_Add); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_CheckBox_Clear); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_CheckBox_Handle); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_CheckBox_Get); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_CheckBox_Draw); settingsAddress += 4;	
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// UserControls ListBox
	////////////////////////////////////////////////////////////////////////////////////////////////
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_ListBox_Add); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_ListBox_Item_Draw); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_ListBox_Handle); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_ListBox_Items_Add); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_ListBox_Draw); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_ListBox_GetText); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_ListBox_Clear); settingsAddress += 4;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// UserControls MessageBox
	////////////////////////////////////////////////////////////////////////////////////////////////
	FLASH_ProgramWord(settingsAddress, (uint32_t)&MessageBox_GetVisible); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&MessageBox_GetResult); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&MessageBox_ResetResult); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&MessageBox_Handle); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&MessageBox_Show_Procedure); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&MessageBox_Show); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&MessageBox_Hide); settingsAddress += 4;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// UserControls RadioButton
	////////////////////////////////////////////////////////////////////////////////////////////////
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_RadioButton_Add); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_RadioButton_Set); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_RadioButton_Clear); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_RadioButton_Draw); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_RadioButton_Handle); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_RadioButton_GetTextFromIndex); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_RadioButton_GetDigitFromIndex); settingsAddress += 4;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// UserControls Button
	////////////////////////////////////////////////////////////////////////////////////////////////
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_ValueRegulator_Add); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_ValueRegulator_Clear); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_ValueRegulator_Handle); settingsAddress += 4;
	FLASH_ProgramWord(settingsAddress, (uint32_t)&UserControls_ValueRegulator_Draw); settingsAddress += 4;
	
	do
	{
		//Read the data from the file
		res = f_read(&fsrc, &buffer, btr, &br);

		if(res == FR_OK)
		{
			for(index = 0; index < br; index = index + 4)
			{
				//Flash Data
				flashValue = buffer[index + 3] << 24;
				flashValue |= buffer[index + 2] << 16;
				flashValue |= buffer[index + 1] << 8;
				flashValue |= buffer[index];

				while(FLASH_ProgramWord(writeAddress, flashValue) != FLASH_COMPLETE);
				if (*(uint32_t*)writeAddress != flashValue)
				{
					while(1);
				}

				writeAddress += 4;
			}
		}
		else
		{
			while(1);
		}
	}
	while(br == btr);

	// Close the file
	res = f_close(&fsrc);
	FLASH_Lock(); 
	return true;
}

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
