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

#ifndef _RTC_TIME
#define _RTC_TIME

#include <stm32f4xx_rcc.h>
#include <stm32f4xx_rtc.h>
#include <UserControls_MessageBox.h>

void RTC_Time_Init(void);
void RTC_Config(void);
void RTC_TimeRegulate(int h, int m, int s);
void Clock_GetTime(int* h, int* m, int* s);

#endif

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
