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

/* Includes ------------------------------------------------------------------*/
#include "TouchPanel.h"

   extern FIL fsrc;      // file objects
   extern FRESULT res;         // FatFs function common result code

/* Private variables ---------------------------------------------------------*/
Matrix matrix ;
Coordinate  display ;


Coordinate ScreenSample[3];

Coordinate DisplaySample[3] = {
                                { 30,  150 },
								{ 290, 300},
                                { 160, 450}
	                          };

#define THRESHOLD 2

static void ADS7843_SPI_Init(void) 
{ 
  SPI_InitTypeDef SPI_InitStruct;	

  RCC_APB1PeriphClockCmd(Open_RCC_SPI,ENABLE);	

  SPI_I2S_DeInit(Open_SPI);
  /* Open_SPI Config -------------------------------------------------------------*/ 
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master; 
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; 
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; 
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; 
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; 
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; 
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; 
  SPI_InitStruct.SPI_CRCPolynomial = 7; 

  SPI_Init(Open_SPI, &SPI_InitStruct);

  SPI_Cmd(Open_SPI, ENABLE); 
} 

bool LoadCalibrate(void)
{
	int n, it, cur=0, state=0;
	char c[50], buff[120];
		
	if (f_open(&fsrc, "SETTINGS.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) != FR_OK)
	{
		MessageBox_Show("#> SD - Error Open Settings File;\n", "SD Error", true, false);
		return false;
	}	
	
	f_read (&fsrc, buff, 120, (UINT*)&n);
	
	
	matrix.An = 0;
	matrix.Bn = 0;
	matrix.Cn = 0;
	matrix.Dn = 0;
	matrix.En = 0;
	matrix.Fn = 0;
	matrix.Divider = 0;
	
	
	/* Parse */
	for (it = 0; it < n && buff[it] != 0; it++)
	{
		if (buff[it] == '\n')
		{
			c[cur] = 0;
			cur = 0;
			
			switch(state)
			{
				case 0: matrix.An = atof(c); 
				break;
				
				case 1: matrix.Bn = atof(c); 
				break;
				
				case 2: matrix.Cn = atof(c); 
				break;
				
				case 3: matrix.Dn = atof(c); 
				break;
				
				case 4: matrix.En = atof(c); 
				break;
				
				case 5: matrix.Fn = atof(c); 
				break;
				
				case 6: matrix.Divider = atof(c); 
				break;				
			}
			state++;
		}
		else
		{
			c[cur] = buff[it];
			cur++;
		}
	}
		
	f_close(&fsrc);
	
	
	if (	matrix.An == 0 || matrix.Bn == 0 ||	matrix.Cn == 0 ||	matrix.Dn == 0 ||
				matrix.En == 0 || matrix.Fn == 0 || matrix.Divider == 0  )
	{
		return false;
	}
	
	return true;
}

void SaveCalibrate(void)
{	
	if (f_open(&fsrc, "SETTINGS.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) != FR_OK)
	{
		while(1);
		//cprintf("#> SD - Error Open Settings File;\n");
	}	
			
	f_printf(&fsrc, "%d.%d\n", (int)matrix.An, (int)((matrix.An-(int)matrix.An)*10000));
	f_printf(&fsrc, "%d.%d\n", (int)matrix.Bn, (int)((matrix.Bn-(int)matrix.Bn)*10000));
	f_printf(&fsrc, "%d.%d\n", (int)matrix.Cn, (int)((matrix.Cn-(int)matrix.Cn)*10000));
	f_printf(&fsrc, "%d.%d\n", (int)matrix.Dn, (int)((matrix.Dn-(int)matrix.Dn)*10000));
	f_printf(&fsrc, "%d.%d\n", (int)matrix.En, (int)((matrix.En-(int)matrix.En)*10000));
	f_printf(&fsrc, "%d.%d\n", (int)matrix.Fn, (int)((matrix.Fn-(int)matrix.Fn)*10000));
	f_printf(&fsrc, "%d.%d\n", (int)matrix.Divider, (int)((matrix.Divider-(int)matrix.Divider)*10000));
		
	f_close(&fsrc);
}

/*******************************************************************************
* Function Name  : TP_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TP_Init(void) 
{ 

	GPIO_InitTypeDef GPIO_InitStructure;
	//EXTI_InitTypeDef   EXTI_InitStructure;
	//NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(Open_SPI_SCK_GPIO_CLK | Open_SPI_MISO_GPIO_CLK | Open_SPI_MOSI_GPIO_CLK,ENABLE);
	
	RCC_AHB1PeriphClockCmd(Open_TP_CS_CLK | Open_TP_IRQ_CLK,ENABLE);
	Open_SPI_CLK_INIT(Open_RCC_SPI,ENABLE);

	GPIO_PinAFConfig(Open_SPI_SCK_GPIO_PORT,  Open_SPI_SCK_SOURCE,  Open_GPIO_AF_SPI);
	GPIO_PinAFConfig(Open_SPI_MISO_GPIO_PORT, Open_SPI_MISO_SOURCE, Open_GPIO_AF_SPI);
	GPIO_PinAFConfig(Open_SPI_MOSI_GPIO_PORT, Open_SPI_MOSI_SOURCE, Open_GPIO_AF_SPI);

	GPIO_InitStructure.GPIO_Pin = Open_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;  
	GPIO_Init(Open_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Open_SPI_MISO_PIN;
	GPIO_Init(Open_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Open_SPI_MOSI_PIN;
	GPIO_Init(Open_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);


   /* TP_CS  */
   GPIO_InitStructure.GPIO_Pin = Open_TP_CS_PIN;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(Open_TP_CS_PORT, &GPIO_InitStructure);

  
    /*TP_IRQ */
	GPIO_InitStructure.GPIO_Pin = Open_TP_IRQ_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(Open_TP_IRQ_PORT, &GPIO_InitStructure);	
	
	TP_CS(1); 
  ADS7843_SPI_Init(); 
	
	if (LoadCalibrate() == false)
	{
			TouchPanel_Calibrate();
	}
	
	/* EXTI */
	
	/* Connect EXTI Line5 to PC5 pin */
  //SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);
	
	/* Configure EXTI Line5 */
   //EXTI_InitStructure.EXTI_Line = EXTI_Line5;
   //EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
   //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
   //EXTI_InitStructure.EXTI_LineCmd = ENABLE;
   //EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
	//NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  // NVIC_Init(&NVIC_InitStructure);
} 

/*******************************************************************************
* Function Name  : DelayUS
* Description    : 
* Input          : - cnt:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

static void DelayUS(vu32 cnt)
{
  uint16_t i;
  for(i = 0;i<cnt;i++)
  {
     uint8_t us = 12;
     while (us--)
     {
       ;   
     }
  }
}


/*******************************************************************************
* Function Name  : WR_CMD
* Description    : 
* Input          : - cmd: 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void WR_CMD (uint8_t cmd)  
{ 
  /* Wait for SPI3 Tx buffer empty */ 
  while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_TXE) == RESET); 
  /* Send SPI3 data */ 
  SPI_I2S_SendData(Open_SPI,cmd); 
  /* Wait for SPI3 data reception */ 
  while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_RXNE) == RESET); 
  /* Read Open_SPI received data */ 
  SPI_I2S_ReceiveData(Open_SPI); 
} 



/*******************************************************************************
* Function Name  : RD_AD
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
static int RD_AD(void)  
{ 
  unsigned short buf,temp; 
  /* Wait for Open_SPI Tx buffer empty */ 
  while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_TXE) == RESET); 
  /* Send Open_SPI data */ 
  SPI_I2S_SendData(Open_SPI,0x0000); 
  /* Wait for SPI3 data reception */ 
  while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_RXNE) == RESET); 
  /* Read Open_SPI received data */ 
  temp=SPI_I2S_ReceiveData(Open_SPI); 
  buf=temp<<8; 
  DelayUS(1); 
  while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_TXE) == RESET); 
  /* Send Open_SPI data */ 
  SPI_I2S_SendData(Open_SPI,0x0000); 
  /* Wait for Open_SPI data reception */ 
  while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_RXNE) == RESET); 
  /* Read Open_SPI received data */ 
  temp=SPI_I2S_ReceiveData(Open_SPI); 
  buf |= temp;
  buf>>=3; 
  buf&=0xfff; 
  return buf; 
} 


/*******************************************************************************
* Function Name  : Read_X
* Description    : Read ADS7843 ADC X 
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
int Read_X(void)  
{  
  int i; 
  TP_CS(0); 
  DelayUS(1); 
  WR_CMD(CHX); 
  DelayUS(1); 
  i=RD_AD(); 
  TP_CS(1); 
  return i;    
} 

/*******************************************************************************
* Function Name  : Read_Y
* Description    : Read ADS7843 ADC Y
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
int Read_Y(void)  
{  
  int i; 
  TP_CS(0); 
  DelayUS(1); 
  WR_CMD(CHY); 
  DelayUS(1); 
  i=RD_AD(); 
  TP_CS(1); 
  return i;     
} 

/*******************************************************************************
* Function Name  : Read_Bat
* Description    : Read ADS7843 ADC Bat
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
int Read_Bat(void)  
{  
  int i; 
  TP_CS(0); 
  DelayUS(1); 
  WR_CMD(CHB); 
  DelayUS(1); 
  i=RD_AD(); 
  TP_CS(1); 
  return i;     
} 

void TP_SwitchOff(void)
{
	TP_CS(0); 
  DelayUS(1); 
  WR_CMD(0x80); 
  DelayUS(1); 
	TP_CS(1); 
}

/*******************************************************************************
* Function Name  : TP_GetAdXY
* Description    : Read ADS7843
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
void TP_GetAdXY(int *x,int *y)  
{ 
  int adx,ady; 
  adx=Read_X(); 
  DelayUS(1); 
  ady=Read_Y(); 
  *x=adx; 
  *y=ady; 
} 

/*******************************************************************************
* Function Name  : TP_DrawPoint
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos)
{
  LCD_SetPoint(Xpos,Ypos,Blue);     /* Center point */
  LCD_SetPoint(Xpos+1,Ypos,Blue);
  LCD_SetPoint(Xpos,Ypos+1,Blue);
  LCD_SetPoint(Xpos+1,Ypos+1,Blue);	
}	

/*******************************************************************************
* Function Name  : DrawCross
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DrawCross(uint16_t Xpos,uint16_t Ypos)
{
  GUI_Line(Xpos-15,Ypos,Xpos-2,Ypos,0xffff);
  GUI_Line(Xpos+2,Ypos,Xpos+15,Ypos,0xffff);
  GUI_Line(Xpos,Ypos-15,Xpos,Ypos-2,0xffff);
  GUI_Line(Xpos,Ypos+2,Xpos,Ypos+15,0xffff);
  
  GUI_Line(Xpos-15,Ypos+15,Xpos-7,Ypos+15,RGB(184,158,131));
  GUI_Line(Xpos-15,Ypos+7,Xpos-15,Ypos+15,RGB(184,158,131));

  GUI_Line(Xpos-15,Ypos-15,Xpos-7,Ypos-15,RGB(184,158,131));
  GUI_Line(Xpos-15,Ypos-7,Xpos-15,Ypos-15,RGB(184,158,131));

  GUI_Line(Xpos+7,Ypos+15,Xpos+15,Ypos+15,RGB(184,158,131));
  GUI_Line(Xpos+15,Ypos+7,Xpos+15,Ypos+15,RGB(184,158,131));

  GUI_Line(Xpos+7,Ypos-15,Xpos+15,Ypos-15,RGB(184,158,131));
  GUI_Line(Xpos+15,Ypos-15,Xpos+15,Ypos-7,RGB(184,158,131));	  	
}	
	
/*******************************************************************************
* Function Name  : Read_Ads7846
* Description    : Get TouchPanel X Y
* Input          : None
* Output         : None
* Return         : Coordinate *
* Attention		 : None
*******************************************************************************/
Coordinate *Read_Ads7846(void)
{
  static Coordinate  screen;
  int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
  uint8_t count=0;
  int buffer[2][9]={{0},{0}};
  
  do
  {		   
    TP_GetAdXY(TP_X,TP_Y);  
	buffer[0][count]=TP_X[0];  
	buffer[1][count]=TP_Y[0];
	count++;  
  }
  while(!TP_INT_IN&& count<9);  /* TP_INT_IN  */
  if(count==9)   /* Average X Y  */ 
  {
	/* Average X  */
  temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
	temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
	temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;

	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];

	m0=m0>0?m0:(-m0);
  m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);

	if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;

	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.x=(temp[0]+temp[2])/2;
	  else 
	    screen.x=(temp[0]+temp[1])/2;	
	}
	else if(m2<m1) 
	  screen.x=(temp[0]+temp[2])/2;
	else 
	  screen.x=(temp[1]+temp[2])/2;

	/* Average Y  */
  temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
	temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
	temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];
	m0=m0>0?m0:(-m0);
	m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);
	if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) return 0;

	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.y=(temp[0]+temp[2])/2;
	  else 
	    screen.y=(temp[0]+temp[1])/2;	
    }
	else if(m2<m1) 
	   screen.y=(temp[0]+temp[2])/2;
	else
	   screen.y=(temp[1]+temp[2])/2;

	return &screen;
  }  
  return 0; 
}
	 

/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    : Calculate K A B C D E F
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,
                          Coordinate * screenPtr,
                          Matrix * matrixPtr)
{

  FunctionalState retTHRESHOLD = ENABLE ;
  /* K£½(X0£­X2) (Y1£­Y2)£­(X1£­X2) (Y0£­Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = DISABLE;
  }
  else
  {
    /* A£½((XD0£­XD2) (Y1£­Y2)£­(XD1£­XD2) (Y0£­Y2))£¯K	*/
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	/* B£½((X0£­X2) (XD1£­XD2)£­(XD0£­XD2) (X1£­X2))£¯K	*/
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* C£½(Y0(X2XD1£­X1XD2)+Y1(X0XD2£­X2XD0)+Y2(X1XD0£­X0XD1))£¯K */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* D£½((YD0£­YD2) (Y1£­Y2)£­(YD1£­YD2) (Y0£­Y2))£¯K	*/
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* E£½((X0£­X2) (YD1£­YD2)£­(YD0£­YD2) (X1£­X2))£¯K	*/
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* F£½(Y0(X2YD1£­X1YD2)+Y1(X0YD2£­X2YD0)+Y2(X1YD0£­X0YD1))£¯K */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }
  return( retTHRESHOLD ) ;
}

/*******************************************************************************
* Function Name  : getDisplayPoint
* Description    : Touch panel X Y to display X Y
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
FunctionalState getDisplayPoint(Coordinate * displayPtr,
                     Coordinate * screenPtr,
                     Matrix * matrixPtr )
{
  FunctionalState retTHRESHOLD =ENABLE ;

  if( matrixPtr->Divider != 0 )
  { DelayUS(2000);
    /* XD = AX+BY+C */        
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;
	/* YD = DX+EY+F */        
    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;
  }
  else
  {
    retTHRESHOLD = DISABLE;
  }
  return(retTHRESHOLD);
} 

/*******************************************************************************
* Function Name  : delay_ms
* Description    : Delay Time
* Input          : - cnt: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void delay_ms(uint16_t ms)    
{ 
	uint16_t i,j; 
	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 0xffff; j++ );
	}
} 

/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TouchPanel_Calibrate(void)
{
  uint8_t i;
  Coordinate * Ptr=0;

  for(i=0;i<3;i++)
  {
		Ptr=0;
   LCD_Clear(Black);
   GUI_Text(44,10,(uint8_t*)"Touch crosshair to calibrate",White,Black);
   delay_ms(300);
   DrawCross(DisplaySample[i].x,DisplaySample[i].y);
   do
   {
		 if (Read_Ads7846() != (void*)0)
		 {
       Ptr=Read_Ads7846();
		 }
		 else
		 {
			 if (Ptr != (void*)0) 
			 {
				 break;
			 }
		 }
   }
   while(1);
	 
   ScreenSample[i].x= Ptr->x; ScreenSample[i].y= Ptr->y;
  }
  setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix );
  LCD_Clear(Black);
	
	SaveCalibrate();
} 

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/

