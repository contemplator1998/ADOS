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

#ifndef PWR_H
#define PWR_H

#include "stm32f4xx.h"
#include <LCD.h>
#include <TouchPanel.h>

#define BUTTON_UP_PORT GPIOA
#define BUTTON_DOWN_PORT GPIOE

#define BUTTON_UP_PORT_ENABLE RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define BUTTON_DOWN_PORT_ENABLE RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)

#define BUTTON_UP_PIN GPIO_Pin_0
#define BUTTON_DOWN_PIN GPIO_Pin_4

#define BUTTON_UP_EXTI_LINE EXTI_Line0
#define BUTTON_DOWN_EXTI_LINE EXTI_Line4

#define BUTTON_UP_EXTI_IRQ EXTI0_IRQn
#define BUTTON_DOWN_EXTI_IRQ EXTI4_IRQn

#define BUTTON_UP_EXTI_PORT EXTI_PortSourceGPIOA
#define BUTTON_DOWN_EXTI_PORT EXTI_PortSourceGPIOE

#define BUTTON_UP_EXTI_PIN EXTI_PinSource0
#define BUTTON_DOWN_EXTI_PIN EXTI_PinSource4

void PWR_InitButtons(void);
void PWR_GoToSleep(void);
void PWR_CheckStartupCalibrate(void);


#endif

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
