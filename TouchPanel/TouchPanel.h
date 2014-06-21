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

#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

#include "stm32f4xx.h"
#include "config.h"
#include "LCD.h"
#include <stdbool.h>
#include <ff.h>
#include <stdlib.h>

typedef	struct POINT 
{
   uint16_t x;
   uint16_t y;
}Coordinate;

typedef struct Matrix 
{						
long double An,  
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
} Matrix ;

extern Coordinate ScreenSample[3];
extern Coordinate DisplaySample[3];
extern Matrix matrix ;
extern Coordinate  display ;

#define	CHX 	0x90
#define	CHY 	0xd0
#define CHB		0xa4 /* a0 */
			
void TP_Init(void);	
Coordinate *Read_Ads7846(void);
void TouchPanel_Calibrate(void);
void DrawCross(uint16_t Xpos,uint16_t Ypos);
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos);
int Read_Bat(void);
void TP_SwitchOff(void);
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
FunctionalState getDisplayPoint(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr );

#endif

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
