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

#ifndef _SDUSER_H
#define _SDUSER_H

#include <ff.h>
#include <stdio.h>
#include <string.h>
#include <LCD.h>
#include <UserControls_ListBox.h>

extern FATFS fs;      // Work area (file system object) for logical drive
extern FIL fsrc;      // file objects
extern FRESULT res;   // FatFs function common result code

FRESULT scan_files (char* path);
void scan_programs(void);

#endif

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
