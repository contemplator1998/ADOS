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

#include <Core.h>

uint32_t settingsAddress;

void CORE_DownloadCoreTable(bool eraseSector)
{
	settingsAddress = FLASH_CORE_TABLE_START_ADDR;
	if (eraseSector)
	{
		/* Unlock the Flash to enable the flash control register access *************/ 
		FLASH_Unlock();
    
		/* Clear pending flags (if any) */  
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
						FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
				  
		if (FLASH_EraseSector(GetSector(0x8008000), VoltageRange_3) != FLASH_COMPLETE)
		{ 
		  while (1)
		  {
				;
		  }
		}
	}
	
}

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
