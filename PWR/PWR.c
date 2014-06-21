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

#include <PWR.h>

void _PWR_InitUpButton(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	BUTTON_UP_PORT_ENABLE;

	/* Configure up button pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = BUTTON_UP_PIN;
	GPIO_Init(BUTTON_UP_PORT, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(BUTTON_UP_EXTI_PORT, BUTTON_UP_EXTI_PIN);

	EXTI_InitStructure.EXTI_Line = BUTTON_UP_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = BUTTON_UP_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void _PWR_InitDownButton(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	BUTTON_DOWN_PORT_ENABLE;

	/* Configure up button pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = BUTTON_DOWN_PIN;
	GPIO_Init(BUTTON_DOWN_PORT, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(BUTTON_DOWN_EXTI_PORT, BUTTON_DOWN_EXTI_PIN);

	EXTI_InitStructure.EXTI_Line = BUTTON_DOWN_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = BUTTON_DOWN_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void PWR_InitButtons(void)
{
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	_PWR_InitDownButton();
	_PWR_InitUpButton();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);

	/* Clear Wakeup flag */
	PWR_ClearFlag(PWR_FLAG_WU);

	/* Check if the StandBy flag is set */
	if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
	{
		/* Clear StandBy flag */
		PWR_ClearFlag(PWR_FLAG_SB);
	}
	else
	{
		RCC_BackupResetCmd(ENABLE);
		RCC_BackupResetCmd(DISABLE);
	}
	
	/* Enable WKUP pin  */
	//PWR_WakeUpPinCmd(ENABLE);
}

void PWR_CheckStartupCalibrate(void)
{
	if (GPIO_ReadInputDataBit(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN))
	{
		TouchPanel_Calibrate();
	}
}

void PWR_GoToSleep(void)
{
		LCD_Enter_Sleep_Mode();
		PWR_WakeUpPinCmd(ENABLE);
		PWR_EnterSTANDBYMode();
}

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
