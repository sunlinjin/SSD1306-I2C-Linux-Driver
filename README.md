# SSD1306-I2C-Linux-Driver

使用SSD1306驱动芯片以及I2C总线接口的OLED模块的Linux用户态驱动程序。本程序适用于Linux操作系统。已经在Raspberry Pi（Raspbian）、Intel Galileo Gen1（Yocto Linux）、NanoPi 2 Fire（Debian）等单板机上测试通过。

## 兼容性说明

请根据实际情况，修改`SSD1306.h`中，关于I2C设备文件和I2C从设备地址的宏定义。

    #define SSD1306_I2C_DEVFILE "/dev/i2c-0" // Galileo Gen1
    #define SSD1306_I2C_ADDR 0x3c // ==(SSD1306_Address)

以及，根据所使用平台不同（例如使用STM32、8051等平台），请修改`SSD1306.c`中，函数`SSD1306_sendCommand()`和`SSD1306_sendData()`的具体实现。

## 权利声明

原始文件附带的权利声明：

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

本版本由mikukonai出于个人使用目的改写。
