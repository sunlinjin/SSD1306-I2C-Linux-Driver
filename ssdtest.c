/*
 * SSD1306 I2C Driver Test
 * Mikukonai 2017
 *
 */
#include "SSD1306.h"

int main()
{
	SSD1306_init();
	SSD1306_clearDisplay();
  SSD1306_setBrightness(255);

  SSD1306_setPageMode();
	SSD1306_setTextXY(0,0);
	SSD1306_putString("ABCDEFGHIJKLMNOP");
  SSD1306_setTextXY(1,0);
  SSD1306_putString("QRSTUVWXYZabcdef");
  SSD1306_setTextXY(2,0);
  SSD1306_putString("ghijklmnopqrstuv");
  SSD1306_setTextXY(3,0);
  SSD1306_putString("wxyz0123456789!\"");
  SSD1306_setTextXY(4,0);
  SSD1306_putString("#$%^&*(),.<>[]{}");

	close(ssd1306_i2c_devfd);
	return 0;
}
