/*
  06/01/2016
  Author: Makerbro
  Platforms: ESP8266
  Language: C++
  File: ACROBOTIC_SSD1306.h
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

#ifndef SSD1306_H
#define SSD1306_H

#define pgm_read_byte(addr) (*(unsigned char *)(addr))
#define OLEDFONT(name) static const uint8_t name[]

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>

#include "font8x8.h"
#include "font5x7.h"

#define SSD1306_Max_X                 127    //128 Pixels
#define SSD1306_Max_Y                 63     //64  Pixels

#define PAGE_MODE                     01
#define HORIZONTAL_MODE               02

#define SSD1306_Address               0x3C
#define SSD1306_Command_Mode          0x00//0x80
#define SSD1306_Data_Mode             0x40
#define SSD1306_Display_Off_Cmd       0xAE
#define SSD1306_Display_On_Cmd        0xAF
#define SSD1306_Normal_Display_Cmd    0xA6
#define SSD1306_Inverse_Display_Cmd   0xA7
#define SSD1306_Activate_Scroll_Cmd   0x2F
#define SSD1306_Dectivate_Scroll_Cmd  0x2E
#define SSD1306_Set_Brightness_Cmd    0x81

#define Scroll_Left                   0x00
#define Scroll_Right                  0x01

#define Scroll_2Frames                0x7
#define Scroll_3Frames                0x4
#define Scroll_4Frames                0x5
#define Scroll_5Frames                0x0
#define Scroll_25Frames               0x6
#define Scroll_64Frames               0x1
#define Scroll_128Frames              0x2
#define Scroll_256Frames              0x3

///////////////////////////////////////////
// modify this for platform-compatibility
//  I2C device args
///////////////////////////////////////////
//#define SSD1306_I2C_DEVFILE "/dev/i2c-1" // Raspberry Pi (Pin 3 5)
#define SSD1306_I2C_DEVFILE "/dev/i2c-0" // Galileo Gen1
#define SSD1306_I2C_ADDR 0x3c // ==(SSD1306_Address)
static int ssd1306_i2c_devfd = 0;

const uint8_t* m_font;      // Current font.
static uint8_t m_font_offset = 2;  // Font bytes for meta data.
static uint8_t m_font_width;       // Font witdth.
static uint8_t m_col;              // Cursor column.
static uint8_t m_row;              // Cursor row (RAM). 
static char addressingMode;

void SSD1306_init(void);

void SSD1306_setNormalDisplay();
void SSD1306_setInverseDisplay();

// modified
void SSD1306_sendCommand(unsigned char command);
void SSD1306_sendData(unsigned char Data);

void SSD1306_setPageMode();
void SSD1306_setHorizontalMode();

void SSD1306_setTextXY(unsigned char Row, unsigned char Column);
void SSD1306_clearDisplay();
void SSD1306_setBrightness(unsigned char Brightness);
int SSD1306_putChar(unsigned char c);
void SSD1306_putString(const char *string);
unsigned char SSD1306_putNumber(long n);
unsigned char SSD1306_putFloat(float floatNumber,unsigned char decimal);
void SSD1306_drawBitmap(unsigned char *bitmaparray,int bytes);

void SSD1306_setHorizontalScrollProperties(
    int direction,
    unsigned char startPage, 
    unsigned char endPage, 
    unsigned char scrollSpeed);
void SSD1306_activateScroll();
void SSD1306_deactivateScroll();

void SSD1306_setFont(const uint8_t* font);

#endif
