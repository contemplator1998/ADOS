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

#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include "stm32f4xx.h"
/**
 * @brief Definition for TouchPanel
 */
 /* Configure TouchPanel pins:   TP_CS-> PC4 and TP_IRQ-> PC5 */


#define Open_TP_CS_PIN					GPIO_Pin_4
#define Open_TP_CS_PORT					GPIOC
#define Open_TP_CS_CLK					RCC_AHB1Periph_GPIOC

#define Open_TP_IRQ_PIN					GPIO_Pin_5
#define Open_TP_IRQ_PORT				GPIOC
#define Open_TP_IRQ_CLK					RCC_AHB1Periph_GPIOC


#define TP_CS(x)	x ? GPIO_SetBits(Open_TP_CS_PORT,Open_TP_CS_PIN): GPIO_ResetBits(Open_TP_CS_PORT,Open_TP_CS_PIN)

#define TP_INT_IN   GPIO_ReadInputDataBit(Open_TP_IRQ_PORT,Open_TP_IRQ_PIN)

/**
 * @brief Definition for TouchPanel  SPI
 */
 /* Configure TouchPanel pins:   TP_CLK-> PB13 and TP_MISO-> PB14 and TP_MOSI-> PB15 */

#define Open_RCC_SPI   	        		RCC_APB1Periph_SPI2
#define Open_GPIO_AF_SPI 				GPIO_AF_SPI2

#define Open_SPI                        SPI2
#define Open_SPI_CLK_INIT               RCC_APB1PeriphClockCmd
#define Open_SPI_IRQn                   SPI2_IRQn
#define Open_SPI_IRQHANDLER             SPI2_IRQHandler

#define Open_SPI_SCK_PIN                GPIO_Pin_13
#define Open_SPI_SCK_GPIO_PORT          GPIOB
#define Open_SPI_SCK_GPIO_CLK           RCC_AHB1Periph_GPIOB
#define Open_SPI_SCK_SOURCE             GPIO_PinSource13

#define Open_SPI_MISO_PIN               GPIO_Pin_14
#define Open_SPI_MISO_GPIO_PORT         GPIOB
#define Open_SPI_MISO_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define Open_SPI_MISO_SOURCE            GPIO_PinSource14

#define Open_SPI_MOSI_PIN               GPIO_Pin_15
#define Open_SPI_MOSI_GPIO_PORT         GPIOB
#define Open_SPI_MOSI_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define Open_SPI_MOSI_SOURCE            GPIO_PinSource15		

#endif	  /*_CONFIG_H*/

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
