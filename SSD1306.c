/*
  06/01/2016
  Author: Makerbro
  Platforms: ESP8266
  Language: C++
  File: ACROBOTIC_SSD1306.cpp
  ------------------------------------------------------------------------
  Description: 
  SSD1306 OLED Driver Library.
  ------------------------------------------------------------------------
  Please consider buying products from ACROBOTIC to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use. 
  https://acrobotic.com/
  ------------------------------------------------------------------------
  License:
  Released under the MIT license. Please check LICENSE.txt for more
  information.  All text above must be included in any redistribution. 
*/

#include "SSD1306.h"

void SSD1306_init(void)
{
	m_font_offset = 2;
	//I2C init
	ssd1306_i2c_devfd = open(SSD1306_I2C_DEVFILE, O_RDWR);
	if(ioctl(ssd1306_i2c_devfd, I2C_SLAVE, SSD1306_I2C_ADDR)<0 )
	{
		printf("SSD1306 ioctl error : %s\r\n", strerror(errno));
	}

	SSD1306_sendCommand(0xAE);            //display off
	SSD1306_sendCommand(0xA6);            //Set Normal Display (default)
	SSD1306_sendCommand(0xAE);            //DISPLAYOFF
	SSD1306_sendCommand(0xD5);            //SETDISPLAYCLOCKDIV
	SSD1306_sendCommand(0x80);            // the suggested ratio 0x80
	SSD1306_sendCommand(0xA8);            //SSD1306_SETMULTIPLEX
	SSD1306_sendCommand(0x3F);
	SSD1306_sendCommand(0xD3);            //SETDISPLAYOFFSET
	SSD1306_sendCommand(0x0);             //no offset
	SSD1306_sendCommand(0x40|0x0);        //SETSTARTLINE
	SSD1306_sendCommand(0x8D);            //CHARGEPUMP
	SSD1306_sendCommand(0x14);
	SSD1306_sendCommand(0x20);            //MEMORYMODE
	SSD1306_sendCommand(0x00);            //0x0 act like ks0108
	SSD1306_sendCommand(0xA1);            //SEGREMAP   Mirror screen horizontally (A0)
	SSD1306_sendCommand(0xC8);            //COMSCANDEC Rotate screen vertically (C0)
	SSD1306_sendCommand(0xDA);            //0xDA
	SSD1306_sendCommand(0x12);            //COMSCANDEC
	SSD1306_sendCommand(0x81);            //SETCONTRAST
	SSD1306_sendCommand(0xCF);            //
	SSD1306_sendCommand(0xd9);            //SETPRECHARGE 
	SSD1306_sendCommand(0xF1); 
	SSD1306_sendCommand(0xDB);            //SETVCOMDETECT                
	SSD1306_sendCommand(0x40);
	SSD1306_sendCommand(0xA4);            //DISPLAYALLON_RESUME        
	SSD1306_sendCommand(0xA6);            //NORMALDISPLAY             
	SSD1306_clearDisplay();
	SSD1306_sendCommand(0x2E);            //Stop scroll
	SSD1306_sendCommand(0x20);            //Set Memory Addressing Mode
	SSD1306_sendCommand(0x00);            //Set Memory Addressing Mode ab Horizontal addressing mode
	SSD1306_setFont(font8x8);
}

void SSD1306_setFont(const uint8_t* font)
{
  m_font = font;
  m_font_width = pgm_read_byte(&m_font[0]);
}

void SSD1306_sendCommand(unsigned char command)
{
	uint8_t val[2];
  val[0] = SSD1306_Command_Mode;
  val[1] = command;
	if( write(ssd1306_i2c_devfd, &val, 2) < 0 )
	{
		printf("I2C write error\n");
	}
}

void SSD1306_setBrightness(unsigned char Brightness)
{
   SSD1306_sendCommand(SSD1306_Set_Brightness_Cmd);
   SSD1306_sendCommand(Brightness);
}

void SSD1306_setHorizontalMode()
{
    addressingMode = HORIZONTAL_MODE;
    SSD1306_sendCommand(0x20);                      //set addressing mode
    SSD1306_sendCommand(0x00);                      //set horizontal addressing mode
}

void SSD1306_setPageMode()
{
    addressingMode = PAGE_MODE;
    SSD1306_sendCommand(0x20);                      //set addressing mode
    SSD1306_sendCommand(0x02);                      //set page addressing mode
}

void SSD1306_setTextXY(unsigned char row, unsigned char col)
{
    SSD1306_sendCommand(0xB0 + row);                          //set page address
    SSD1306_sendCommand(0x00 + (m_font_width*col & 0x0F));    //set column lower addr
    SSD1306_sendCommand(0x10 + ((m_font_width*col>>4)&0x0F)); //set column higher addr
}

void SSD1306_clearDisplay()
{
  unsigned char i,j;
  SSD1306_sendCommand(SSD1306_Display_Off_Cmd);     //display off
  for(j=0;j<8;j++)
  {    
    SSD1306_setTextXY(j,0);    
    {
      for(i=0;i<16;i++)  //clear all columns
      {
        SSD1306_putChar(' ');    
      }
    }
  }
  SSD1306_sendCommand(SSD1306_Display_On_Cmd);     //display on
  SSD1306_setTextXY(0,0);    
}

void SSD1306_sendData(unsigned char Data)
{
  uint8_t val[2];
  val[0] = SSD1306_Data_Mode;
  val[1] = Data;
  if( write(ssd1306_i2c_devfd, &val, 2) < 0 )
  {
    printf("I2C write error\n");
  }
}

int SSD1306_putChar(unsigned char ch)
{
    if (!m_font) return 0;
    //Ignore non-printable ASCII characters. This can be modified for
    //multilingual font.  
    if(ch < 32 || ch > 127) 
    {
        ch = ' ';
    }
    unsigned char i = 0;
    for(i=0;i<m_font_width;i++)
    {
       // Font array starts at 0, ASCII starts at 32
       SSD1306_sendData(pgm_read_byte(&m_font[(ch-32)*m_font_width+m_font_offset+i])); 
    }
}

void SSD1306_putString(const char *string)
{
    unsigned char i=0;
    while(string[i])
    {
        SSD1306_putChar(string[i]);     
        i++;
    }
}

unsigned char SSD1306_putNumber(long long_num)
{
  unsigned char char_buffer[10]="";
  unsigned char i = 0;
  unsigned char f = 0;

  if (long_num < 0) 
  {
    f=1;
    SSD1306_putChar('-');
    long_num = -long_num;
  } 
  else if (long_num == 0) 
  {
    f=1;
    SSD1306_putChar('0');
    return f;
  } 

  while (long_num > 0) 
  {
    char_buffer[i++] = long_num % 10;
    long_num /= 10;
  }

  f=f+i;
  for(; i > 0; i--)
  {
    SSD1306_putChar('0'+ char_buffer[i - 1]);
  }
  return f;

}

unsigned char SSD1306_putFloat(float floatNumber,unsigned char decimal)
{
  unsigned int temp=0;
  float decy=0.0;
  float rounding = 0.5;
  unsigned char f=0;
  if(floatNumber<0.0)
  {
    SSD1306_putString("-");
    floatNumber = -floatNumber;
    f +=1;
  }
  unsigned char i=0;
  for (i=0; i<decimal; ++i)
  {
    rounding /= 10.0;
  }
    floatNumber += rounding;
  
  temp = floatNumber;
  f += SSD1306_putNumber(temp);
  if(decimal>0)
  {
    SSD1306_putChar('.');
    f +=1;
 }
  decy = floatNumber-temp;//decimal part, 
  for(i=0;i<decimal;i++)//4 
  {
    decy *=10;// for the next decimal
    temp = decy;//get the decimal
    SSD1306_putNumber(temp);
    decy -= temp;
  }
  f +=decimal;
  return f;
}

void SSD1306_drawBitmap(unsigned char *bitmaparray,int bytes)
{
  char localAddressMode = addressingMode;
  if(addressingMode != HORIZONTAL_MODE)
  {
      //Bitmap is drawn in horizontal mode     
      SSD1306_setHorizontalMode();
  }
  int i=0;
  for(i=0;i<bytes;i++)
  {
      SSD1306_sendData(pgm_read_byte(&bitmaparray[i]));
  }

  if(localAddressMode == PAGE_MODE)
  {
     //If pageMode was used earlier, restore it.
     SSD1306_setPageMode(); 
  }
  
}

void SSD1306_setHorizontalScrollProperties(int direction,unsigned char startPage, unsigned char endPage, unsigned char scrollSpeed)
{
   if(Scroll_Right == direction)
   {
        //Scroll right
        SSD1306_sendCommand(0x26);
   }
   else
   {
        //Scroll left  
        SSD1306_sendCommand(0x27);

   }
    SSD1306_sendCommand(0x00);
    SSD1306_sendCommand(startPage);
    SSD1306_sendCommand(scrollSpeed);
    SSD1306_sendCommand(endPage);
    SSD1306_sendCommand(0x00);
    SSD1306_sendCommand(0xFF);
}

void SSD1306_activateScroll()
{
    SSD1306_sendCommand(SSD1306_Activate_Scroll_Cmd);
}

void SSD1306_deactivateScroll()
{
    SSD1306_sendCommand(SSD1306_Dectivate_Scroll_Cmd);
}

void SSD1306_setNormalDisplay()
{
    SSD1306_sendCommand(SSD1306_Normal_Display_Cmd);
}

void SSD1306_setInverseDisplay()
{
    SSD1306_sendCommand(SSD1306_Inverse_Display_Cmd);
}
