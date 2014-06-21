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

#include "LCD.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// GUI Image
////////////////////////////////////////////////////////////////////////////////////////////////////
void GUI_DrawImage(const unsigned int* image, int x, int y, int w, int h)
{
	int xi, yi;
	for (yi = y; yi < y+h; yi++)
	{
		for (xi = x; xi < x+w; xi++)
		{
			LCD_SetPoint(xi, yi, image[(yi-y)*w+(xi-x)]);
		}
	}
}

void GUI_DrawInvertImage(const unsigned int* image, int x, int y, int w, int h)
{
	LCD_Hardware_InvertMode_On();
	GUI_DrawImage(image, x, y, w, h);
	LCD_Hardware_InvertMode_Off();
}

void GUI_DrawImageWithReplace(const unsigned int* image, int x, int y, int w, int h, int color1, int color2)
{
	int xi, yi;
	for (yi = y; yi < y+h; yi++)
	{
		for (xi = x; xi < x+w; xi++)
		{
			LCD_SetPoint(xi, yi, ((image[(yi-y)*w+(xi-x)] == color1) ? color2 : image[(yi-y)*w+(xi-x)]));
		}
	}	
}

void GUI_DrawImage_SD(char* path, int x, int y, int w, int h)
{
	int n = 120, xi=0, yi=0, i;
	char buff[120];
	if (f_open(&fsrc, path, FA_READ) != FR_OK)
	{
		MessageBox_Show("#> SD - Error Open Image;\n", "SD Error", true, false);
		return;
	}	
	
	while (n == 120)
	{
		f_read (&fsrc, buff, 120, (UINT*)&n);
		for (i = 0; i < n; i+=2)
		{
			LCD_SetPoint(x+xi,y+yi,(((uint16_t)buff[i+1]<<8)|buff[i]));
			xi++;
			if (xi >= w) 
			{
				xi = 0;
				yi++;
			}
			if (yi > 480 || xi > 320) break;
		}
	}
		
	f_close(&fsrc);
}

void GUI_DrawInvertImage_SD(char* path, int x, int y, int w, int h)
{
	LCD_Hardware_InvertMode_On();
	GUI_DrawImage_SD(path, x, y, w, h);
	LCD_Hardware_InvertMode_Off();
}

void GUI_DrawImageWithReplace_SD(char* path, int x, int y, int w, int h, int color1, int color2)
{
	int n = 120, xi=0, yi=0, i;
	char buff[120];
	if (f_open(&fsrc, path, FA_READ) != FR_OK)
	{
		MessageBox_Show("#> SD - Error Open Image;\n", "SD Error", true, false);
		return;
	}	
	
	while (n == 120)
	{
		f_read (&fsrc, buff, 120, (UINT*)&n);
		for (i = 0; i < n; i+=2)
		{
			int clr = (((uint16_t)buff[i+1]<<8)|buff[i]);
			LCD_SetPoint(x+xi,y+yi,(clr==color1) ? color2 : clr);
			xi++;
			if (xi >= w) 
			{
				xi = 0;
				yi++;
			}
			if (yi > 480 || xi > 320) break;
		}
	}
		
	f_close(&fsrc);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// GUI Clear
////////////////////////////////////////////////////////////////////////////////////////////////////
void GUI_SmartClear(int clr)
{
	int iter;
	
	for (iter = 100; iter < 200; iter += 10) LCD_Clear(RGB(iter,iter,iter));
	for (iter = 200; iter > 0; iter -= 10) LCD_Clear(RGB(iter,iter,iter));
	/*
	for (iter = -40; iter <= 480; iter++)
	{
		if (iter > 0)
		{
			GUI_Line(0, iter-1, 320, iter-1, clr);
		}
		if (iter < 480-40)
		{
			GUI_Line(0, iter+40, 320, iter+40, RGB(90+iter/3,90+iter/3,90+iter/3));
		}
		delay_ms(1S);
	}
	*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// GUI Text
////////////////////////////////////////////////////////////////////////////////////////////////////
void GUI_PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	uint8_t y, x, w, h;
	GetCharSize(&w, &h, ASCI);

	if (GetFont() == 0)
	{
		uint8_t* ascii = GetASCIICode(ASCI);
		for( x=0; x<w; x++ )
		{
				for( y=0; y<h; y++ ) 
				{
						if( (ascii[x] >> 7 - y) & 1 == 1)
						{
								LCD_SetPoint( Xpos + x, Ypos + (h-y), charColor );
						}
						else
						{
							if (bkColor != 0x1234)
								LCD_SetPoint( Xpos + x, Ypos + (h-y), bkColor );
						}
				}
		}
	}
	else
	{
			uint8_t* ascii = GetASCIICode(ASCI);
			for( y=0; y<h; y++ ) 
			{
					for( x=0; x<w; x++ )
					{
						if( (ascii[y] >> 7 - x) & 1 == 1)
						{
								LCD_SetPoint( Xpos + x, Ypos + y, charColor );
						}
						else
						{
							if (bkColor != 0x1234)
								LCD_SetPoint( Xpos + x, Ypos + y, bkColor );
						}
					}
			}
	}
}

void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
	uint8_t TempChar;
	uint8_t w,h;
	
	do
	{
			TempChar = *str++;  
			GUI_PutChar( Xpos, Ypos, TempChar, Color, bkColor );    
			GetCharSize(&w,&h,TempChar);
			if(Xpos < MAX_X - w - 1)
			{
				Xpos += w+1;
			} 
			else if (Ypos < MAX_Y - h)
			{										
				Xpos = 0;
				Ypos += h;
			}   
			else
			{
				Xpos = 0;
				Ypos = 0;
			}    
	}
	while ( *str != 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// GUI Geometry
////////////////////////////////////////////////////////////////////////////////////////////////////
void GUI_Line( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
  short dx,dy;
  short temp;

  if( x0 > x1 )
  {
    temp = x1;
    x1 = x0;
    x0 = temp;   
  }
  if( y0 > y1 )
  {
    temp = y1;
    y1 = y0;
    y0 = temp;   
  }

  dx = x1-x0;
  dy = y1-y0;

  if( dx == 0 )
  {
    do
    { 
      LCD_SetPoint(x0, y0, color);
      y0++;
    }while( y1 >= y0 ); 
    return; 
  }
  if( dy == 0 )
  {
    do
    {
      LCD_SetPoint(x0, y0, color);
      x0++;
    }
    while( x1 >= x0 ); 
		return;
  }

	/* Bresenham's line algorithm  */
  if( dx > dy )
  {
    temp = 2 * dy - dx;
    while( x0 != x1 )
    {
	    LCD_SetPoint(x0,y0,color);
	    x0++;
	    if( temp > 0 )
	    {
	      y0++;
	      temp += 2 * dy - 2 * dx; 
	 	  }
      else         
      {
			  temp += 2 * dy;
			}       
    }
    LCD_SetPoint(x0,y0,color);
  }  
  else
  {
    temp = 2 * dx - dy;
    while( y0 != y1 )
    {
	 	  LCD_SetPoint(x0,y0,color);     
      y0++;                 
      if( temp > 0 )           
      {
        x0++;               
        temp+=2*dy-2*dx; 
      }
      else
			{
        temp += 2 * dy;
			}
    } 
    LCD_SetPoint(x0,y0,color);
	}
} 

void GUI_Circle(int x, int y, int r,int color)
{
        int x1,y1,yk = 0;
        int sigma,delta,f;

        x1 = 0;
        y1 = r;
        delta = 2*(1-r);

        do
        {
                LCD_SetPoint(x+x1,y+y1,color);
                LCD_SetPoint(x-x1,y+y1,color);
                LCD_SetPoint(x+x1,y-y1,color);
                LCD_SetPoint(x-x1,y-y1,color);

                f = 0;
                if (y1 < yk)
                        break;
                if (delta < 0)
                {
                        sigma = 2*(delta+y1)-1;
                        if (sigma <= 0)
                        {
                                x1++;
                                delta += 2*x1+1;
                                f = 1;
                        }
                }
                else
                if (delta > 0)
                {
                        sigma = 2*(delta-x1)-1;
                        if (sigma > 0)
                        {
                                y1--;
                                delta += 1-2*y1;
                                f = 1;
                        }
                }
                if (!f)
                {
                        x1++;
                        y1--;
                        delta += 2*(x1-y1-1);
                }
        }
        while(1);
}

void GUI_Rect(int x, int y, int w, int h, int color)
{
	GUI_Line(x, y, x+w-1, y, color);
	GUI_Line(x, y+h-1, x+w-1, y+h-1, color);
	
	GUI_Line(x, y, x, y+h-1, color);
	GUI_Line(x+w-1, y, x+w-1, y+h-1, color);
}

void GUI_RectEllipce(int x, int y, int w, int h, int color)
{
        int x1,y1,yk = 0;
        int sigma,delta,f;
		int r = 3;

        x1 = 0;
        y1 = r;
        delta = 2*(1-r);

        do
        {
                LCD_SetPoint(x+w-r+x1,y+r-y1,color);
				LCD_SetPoint(x+w-r+x1,y+h-r+y1,color);
				LCD_SetPoint(x+r-x1,y+h-r+y1,color);
				LCD_SetPoint(x+r-x1,y+r-y1,color);
				
                f = 0;
                if (y1 < yk)
                        break;
                if (delta < 0)
                {
                        sigma = 2*(delta+y1)-1;
                        if (sigma <= 0)
                        {
                                x1++;
                                delta += 2*x1+1;
                                f = 1;
                        }
                }
                else
                if (delta > 0)
                {
                        sigma = 2*(delta-x1)-1;
                        if (sigma > 0)
                        {
                                y1--;
                                delta += 1-2*y1;
                                f = 1;
                        }
                }
                if (!f)
                {
                        x1++;
                        y1--;
                        delta += 2*(x1-y1-1);
                }
        }
        while(1);
	GUI_Line(x+r, y, x+w-r, y, color);
	GUI_Line(x+r, y+h, x+w-r, y+h, color);
	
	GUI_Line(x-1, y+r, x-1, y+h-r, color);
	GUI_Line(x+w+1, y+r, x+w+1, y+h-r, color);
}

void GUI_FillRectEllipce(int x, int y, int w, int h, int color)
{
        int x1,y1,yk = 0;
        int sigma,delta,f;
		int r = 3;

        x1 = 0;
        y1 = r;
        delta = 2*(1-r);

        do
        {
				GUI_Line(x+w-r+x1,y+r-y1,x+w-r+x1,y+h-r+y1,color);
				GUI_Line(x+r-x1,y+h-r+y1,x+r-x1,y+r-y1,color);
				
                f = 0;
                if (y1 < yk)
                        break;
                if (delta < 0)
                {
                        sigma = 2*(delta+y1)-1;
                        if (sigma <= 0)
                        {
                                x1++;
                                delta += 2*x1+1;
                                f = 1;
                        }
                }
                else
                if (delta > 0)
                {
                        sigma = 2*(delta-x1)-1;
                        if (sigma > 0)
                        {
                                y1--;
                                delta += 1-2*y1;
                                f = 1;
                        }
                }
                if (!f)
                {
                        x1++;
                        y1--;
                        delta += 2*(x1-y1-1);
                }
        }
        while(1);
		
		
	LCD_FillRect(x+r, y, w-r-r, h, color);
}

void GUI_FillCircle(int x, int y, int r,int color)
{
	// TODO
}

/******************* (C) Copyright (C) 2014  Contemplator (Ivan Zaikov) *****END OF FILE****/
