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

#include "ILI9481.h"

#define LCD_REG              (*((volatile unsigned short *) 0x60000000)) /* RS = 0 */
#define LCD_RAM              (*((volatile unsigned short *) 0x60020000)) /* RS = 1 */

static uint32_t _scrollPos;
static bool _useInvertColor = false;

////////////////////////////////////////////////////////////////////////////////////////////////////
//										System operations 
////////////////////////////////////////////////////////////////////////////////////////////////////
static void LCD_CtrlLinesConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE);
	
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); 

	/* Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
	PD.10(D15), PD.11(A16), PD.14(D0), PD.15(D1) as alternate function push pull */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | 
															GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
															GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
	 PE.14(D11), PE.15(D12) as alternate function push pull */	
	/* GPIOE configuration */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_7 | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
	                              GPIO_Pin_11| GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

static void LCD_FSMCConfig(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 15;
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;	   
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 15;	   
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;	  
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
	
	/* Enable FSMC Bank4_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}

static void LCD_Configuration(void)
{
	u32 i=0x1fffff;
	
	/* Configure the LCD Control pins */
	LCD_CtrlLinesConfig();
	while(i--);
	
	/* Configure the FSMC Parallel interface -------------------------------------*/
	LCD_FSMCConfig();
}

static void LCD_delay(uint16_t ms)    
{ 
	uint16_t i,j; 
	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 1141; j++ );
	}
} 

static void LCD_CS_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOD , GPIO_Pin_7);		 //CS=0;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOB , GPIO_Pin_12);		 	 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//										LCD operations 
////////////////////////////////////////////////////////////////////////////////////////////////////
void LCD_Initialization(void)
{
	LCD_CS_Init();
	LCD_Configuration();

	LCD_WriteIndex(0x1);	// Soft Reset
	LCD_delay(100);	
			
	/*	Read LCD ID	
	LCD_WriteIndex(0xBF);
	DeviceCode = LCD_RAM;
	*/
		
	LCD_WriteIndex(0x0011);

	LCD_WriteIndex(0x0013);
	LCD_delay(300);

	LCD_WriteIndex(0xD0);
	LCD_WriteData(0x07);	//VCI1=VCI=2.8V
	LCD_WriteData(0x41);	//DDVDH=2*VCI1,VGH=6*VCI1,VGL=-4*VCI1
	LCD_WriteData(0x1D);	//VREG1OUT=2.5*1.85=4.625V,VREG1OUT ¨Q (DDVDH - 0.25)V.

	LCD_WriteIndex(0xD1);
	LCD_WriteData(0x00);	//SELVCM=0
	LCD_WriteData(0x1C);	//VCOMH=0.805*VREG1OUT,0x00(0.685)~0x3f(1.0)  18
	LCD_WriteData(0x1F);	//VCOM=1.22*VREG1OUT,0x00(0.7)~0x1f(1.32)  1A

	LCD_WriteIndex(0xD2);
	LCD_WriteData(0x01);	//AP=1
	LCD_WriteData(0x11);	//fDCDC2=Fosc/32,fDCDC1=Fosc/2

	LCD_WriteIndex(0xC0);
	LCD_WriteData(0x00);	//REV=1,SM=0,GS=0
	LCD_WriteData(0x3B);	//NL=8*(NL5:0]+1)=480lines
	LCD_WriteData(0x00);	//SCN[6:0]=0
	LCD_WriteData(0x02);	//NDL=0,PTS[2:0]=0
	LCD_WriteData(0x11);	//PTG=1,ISC[3:0]=1
	 
	LCD_WriteIndex(0xC1);	//Display_Timing_Setting for Normal Mode
	LCD_WriteData(0x10);	//BC0=1:Line inversion
	LCD_WriteData(0x0B);	//RTN0[4:0]=Setting prohibited
	LCD_WriteData(0x88);	//FP0[3:0],BP0[3:0]

	LCD_WriteIndex(0xC5);
	LCD_WriteData(0x01);	//FRA[2:0]=100(Hz)
	//LCD_WriteData(0x0);	//FRA[2:0]=125(Hz)

	LCD_WriteIndex(0XC8);	//Gamma Setting 12Parameter

	LCD_WriteData(0XC8);   
	LCD_WriteData(0X00); 
	LCD_WriteData(0X30); 
	LCD_WriteData(0X36); 
	LCD_WriteData(0X45); 
	LCD_WriteData(0X04); 
	LCD_WriteData(0X16); 
	LCD_WriteData(0X37); 
	LCD_WriteData(0X75); 
	LCD_WriteData(0X77); 
	LCD_WriteData(0X54); 
	LCD_WriteData(0X0F); 


	LCD_WriteIndex(0xE4);
	LCD_WriteData(0xA0);

	LCD_WriteIndex(0xF0);
	LCD_WriteData(0x01);

	LCD_WriteIndex(0xF3);
	LCD_WriteData(0x40);
	LCD_WriteData(0x0A);

	LCD_WriteIndex(0xF7);
	LCD_WriteData(0x80);

	LCD_WriteIndex(0x36);
	LCD_WriteData(0x0a);	//B7=0,B6=0,B5=0,B4=0,B3=1(BGR),B2=0,B1=1,B0=0

	LCD_WriteIndex(0x3a);	//Set_pixel_format
	LCD_WriteData(0x55);	//65k/262k

	//Windows set 
	LCD_WriteIndex(0x2A);	//Set_column_address
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);	//0x00(319)
	LCD_WriteData(0x01);
	LCD_WriteData(0x3F);	//0x3F(319)
	LCD_WriteIndex(0x2B);	//Set_page_address
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0xDF);

	LCD_delay(120);

	LCD_WriteIndex(0x29);	//display_on
	
	//LCD_WriteIndex(0x21);	//display_on

	LCD_WriteIndex(0x2C);	//Write_memory_start
}

void LCD_Clear(uint16_t Color)
{
	uint32_t index=0;

	LCD_SetCursor(0,0);
	
	LCD_REG = 0x2C;
	
	for( index = 0; index < 320*480; index++ )
	{
		if (!_useInvertColor)
		LCD_RAM = 0xFFFF-Color;
		else
		LCD_RAM = Color;
	}

}

void LCD_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
	uint16_t i = 0, j;
	
	if (color == Transparent) return;
	
	/* Start X */
	LCD_WriteIndex(0x2A);
	LCD_RAM = (x>>8) & 1;
	LCD_RAM = x & 0xFF;
	
	/* End X */
	LCD_RAM = (x+width>>8) & 1;
	LCD_RAM = x+width & 0xFF;
	
	/* Start Y */
	LCD_WriteIndex(0x2B);
	LCD_RAM = (y>>8) & 1;
	LCD_RAM = y & 0xFF;
	
	/* End Y */
	LCD_RAM = (y+height>>8) & 1;
	LCD_RAM = y+height & 0xFF;
	
	LCD_WriteIndex(0x2C);
	
	for (j = 0; j <= height; j++)
	{
		for (i = 0; i <= width; i++)
		{
			if (!_useInvertColor)
			LCD_RAM = 0xFFFF-color;
			else
			LCD_RAM = color;
		}
	}
	
	LCD_WriteIndex(0x2A);//Set_column_address
	LCD_WriteData(0x00);
	LCD_WriteData(0x00); //0x00(319)
	LCD_WriteData(0x01);
	LCD_WriteData(0x3F); //0x3F(319)
	LCD_WriteIndex(0x2B);//Set_page_address
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0xDF);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//										Pixel operations 
////////////////////////////////////////////////////////////////////////////////////////////////////
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos >= 320 || Ypos >= 480 )
	{
		return;
	}
	LCD_WriteReg(0x2A, Xpos>>8); LCD_RAM = Xpos & 0xFF;
	LCD_WriteReg(0x2B, Ypos>>8); LCD_RAM = Ypos & 0xFF;
	
	LCD_REG = 0x2C;
	if (!_useInvertColor)
	LCD_RAM = 0xFFFF-point;
	else
	LCD_RAM = point;
}

uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
{
	if( Xpos >= 320 || Ypos >= 480 )
	{
		return 0;
	}
	LCD_WriteReg(0x2A, Xpos>>8); LCD_RAM = Xpos & 0xFF;
	LCD_WriteReg(0x2B, Ypos>>8); LCD_RAM = Ypos & 0xFF;
	
	LCD_REG = 0x2E;
	if (!_useInvertColor)
	return 0xFFFF-LCD_RAM;
	else
	return LCD_RAM;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//										Hardware Scroll
////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t LCD_HardwareScroll(int val, int VERTICAL_SCROLL_AREA)
{
	int16_t newScrollPos;
			
	newScrollPos= _scrollPos + val;
			
	if(newScrollPos < 0)
	{
		newScrollPos+=VERTICAL_SCROLL_AREA;
	}
	else if(newScrollPos > VERTICAL_SCROLL_AREA-1)
	{
		newScrollPos-=VERTICAL_SCROLL_AREA;
	}
	
	// write to the register

	LCD_WriteIndex(0x33);
	LCD_WriteData(0); LCD_WriteData(0); // Top Fixed Area
	LCD_WriteData((VERTICAL_SCROLL_AREA >> 8) & 1); LCD_WriteData((VERTICAL_SCROLL_AREA+1) & 0xFF); // Vertical Scroll Area
	LCD_WriteData(0); LCD_WriteData(0); // Bottom Fixed Area
	
	LCD_WriteIndex(0x37);
	LCD_WriteData((newScrollPos>>8) & 0x1);
	LCD_WriteData(newScrollPos & 0xFF);

	// set new position

	_scrollPos=newScrollPos;
							
	return _scrollPos;
}

uint16_t LCD_HardwareScroll_Position() 
{
	return _scrollPos;
}

void LCD_HardwareScroll_Position_Reset(int VERTICAL_SCROLL_AREA)
{
	_scrollPos = 0;
	LCD_WriteIndex(0x33);
	LCD_WriteData(0); LCD_WriteData(0); // Top Fixed Area
	LCD_WriteData((VERTICAL_SCROLL_AREA >> 8) & 1); LCD_WriteData((VERTICAL_SCROLL_AREA+1) & 0xFF); // Vertical Scroll Area
	LCD_WriteData(0); LCD_WriteData(0); // Bottom Fixed Area
			
	LCD_WriteIndex(0x37);
	LCD_WriteData(0);
	LCD_WriteData(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//										Transfer operations
////////////////////////////////////////////////////////////////////////////////////////////////////
void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos )
{		
	LCD_WriteReg(0x2A, Xpos>>8); LCD_RAM = Xpos & 0xFF;
	LCD_WriteReg(0x2B, Ypos>>8); LCD_RAM = Ypos & 0xFF;
}

void LCD_WriteIndex(uint16_t index)
{
	//GPIO_ResetBits(GPIOD , GPIO_Pin_7);		 //CS=0;
	LCD_REG	= index;
	//GPIO_SetBits(GPIOD , GPIO_Pin_7);		 //CS=1;
}

void LCD_WriteData(uint16_t data)
{
	//GPIO_ResetBits(GPIOD , GPIO_Pin_7);		 //CS=0;
	LCD_RAM = data;
	//GPIO_SetBits(GPIOD , GPIO_Pin_7);		 //CS=1;
}

uint16_t LCD_ReadData(void)
{
	//uint32_t tmp;
	//GPIO_ResetBits(GPIOD , GPIO_Pin_7);		 //CS=0;
	//tmp = LCD_RAM;
	//GPIO_SetBits(GPIOD , GPIO_Pin_7);		 //CS=1;
	return LCD_RAM;
	//return tmp;
}

void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{ 
	/* Write 16-bit Index, then Write Reg */  
	LCD_WriteIndex(LCD_Reg);         
	/* Write 16-bit Reg */
	LCD_WriteData(LCD_RegValue);  
}

uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
	/* Write 16-bit Index (then Read Reg) */
	LCD_WriteIndex(LCD_Reg);

	/* Read 16-bit Reg */
	return LCD_ReadData();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//										Transfer operations
////////////////////////////////////////////////////////////////////////////////////////////////////
void LCD_Enter_Sleep_Mode(void)
{
	LCD_WriteIndex(0x10);
}

void LCD_Exit_Sleep_Mode(void)
{
	LCD_WriteIndex(0x11);
}

void LCD_Enter_Normal_Mode(void)
{
	LCD_WriteIndex(0x11);
}

void LCD_Enter_Invert_Mode(void)
{
	LCD_WriteIndex(0x21);
}

void LCD_Exit_Invert_Mode(void)
{
	LCD_WriteIndex(0x20);
}

void LCD_ON(void)
{
	LCD_WriteIndex(0x29);
}

void LCD_OFF(void)
{
	LCD_WriteIndex(0x28);
}

void LCD_Hardware_InvertMode_On(void)
{
	_useInvertColor = true;
}

void LCD_Hardware_InvertMode_Off(void)
{
	_useInvertColor = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//										Led
////////////////////////////////////////////////////////////////////////////////////////////////////
void LCD_Led_On(void)
{
	GPIO_SetBits(GPIOB , GPIO_Pin_12);	
}

void LCD_Led_Off(void)
{
	GPIO_ResetBits(GPIOB , GPIO_Pin_12);	
}

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
