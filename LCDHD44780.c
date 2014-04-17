/*##############################################################*/
/* Author: Marcus Nasarek                                       */
/* File:   LCDH44780.c                                          */
/* Compile with:                                                */
/*      gcc -Wall -c LCDHD44780.c -o LCDHD44780.o               */
/* License:                                                     */
/*                                                              */
/*  This program is free software; you can redistribute it      */ 
/*  and/or modify it under the terms of the GNU General         */  
/*  Public License as published by the Free Software            */
/*  Foundation; either version 3 of the License, or             */
/*  (at your option) any later version.                         */
/*                                                              */
/*  This program is distributed in the hope that it will        */
/*  be useful, but WITHOUT ANY WARRANTY; without even the       */
/*  implied warranty of MERCHANTABILITY or                      */
/*  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       */
/*  Public License for more details.                            */
/*                                                              */
/*  You should have received a copy of the GNU General          */
/*  Public License along with this program; if not,             */
/*  see <http://www.gnu.org/licenses/>.                         */
/*                                                              */
/*##############################################################*/

#include "LCDHD44780.h"

void init_gpio(void)
{
  // Set all pins to output
  bcm2835_gpio_fsel(LCD_RS, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LCD_D4, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LCD_D5, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LCD_D6, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LCD_D7, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LCD_E,  BCM2835_GPIO_FSEL_OUTP);
  
  bcm2835_delayMicroseconds(C_DELAY);
  
  // Set all pins to LOW
  bcm2835_gpio_clr(LCD_RS);
  bcm2835_gpio_clr(LCD_D4);
  bcm2835_gpio_clr(LCD_D5);
  bcm2835_gpio_clr(LCD_D5);
  bcm2835_gpio_clr(LCD_D7);
  bcm2835_gpio_clr(LCD_E);
  
}

void init_lcd(void)
{
  init_gpio();
  /* 
  The busy flag (BF) is kept in the busy state
  until the initialization ends (BF = 1). The 
  busy state lasts for 10 ms after VCC rises to 4.5 V
  */
  bcm2835_delay(GPIO_RISING);

  /*
  --- symbols:
  I/D = 1: Increment
  I/D = 0: Decrement
  S = 1: Accompanies display shift
  S/C = 1: Display shift
  S/C = 0: Cursor move
  R/L = 1: Shift to the right
  R/L = 0: Shift to the left
  DL = 1: 8 bits, DL = 0: 4 bits
  N = 1: 2 lines, N = 0: 1 line
  F = 1: 5
  ×
  10 dots, F = 0: 5
  ×
  8 dots
  BF = 1: Internally operating
  BF = 0: Instructions acceptable
  */
  
  /* 
  --- Display init after turn on: 
  1. Display clear
  2. Function set:
      DL = 1; 8-bit interface data
      N = 0; 1-line display
      F = 0; 5×8 dot character font
  3. Display on/off control:
      D = 0; Display off
      C = 0; Cursor off
      B = 0; Blinking off
  4. Entry mode set:
      I/D = 1; Increment by 1
      S = 0; No shift
  */ 
  
  // begin of "warming up"
  // see fig 24 on page 46 of datasheet
  
  // Function set - 
  // Sets interface data length
  // (DL), number of display lines
  // (N), and character font (F).
  // RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 
  // 0  0   0   0   1   DL   N   F   -   -  
  // DL = 0 --> 4 bit data length (1 = 8 bit)
  // N = 1 --> 2 line display (0 = 1 line)
  // F = 0 --> 5x8 dot character font
  // 0x33 = 
  // 0  0   0   0   1   1    0   0   1   1
  // --> ?
  write_to_lcd(0x33, LCD_CMD);
  bcm2835_delayMicroseconds(100);
  
  // Function set - 
  // Sets interface data length
  // (DL), number of display lines
  // (N), and character font (F).
  // RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 
  // 0  0   0   0   1   DL   N   F   -   -  
  // DL = 0 --> 4 bit data length (1 = 8 bit)
  // N = 1 --> 2 line display (0 = 1 line)
  // F = 0 --> 5x8 dot character font
  // 0x32 = 
  // 0  0   0   0   1   1    0   0   1   0  
  // --> ?
  write_to_lcd(0x32, LCD_CMD);
  bcm2835_delayMicroseconds(R_EXEC_TIME);
  
  // end of "warming up"
  
  lcd_function_set(LCD_FUNCTION_SET | TWO_LINES);

  lcd_control(LCD_CONTROL | DISPLAY_ON);

  lcd_entry_mode(LCD_ENTRY_MODE | ADDR_INCREMENT);
  
  clear_lcd();
  
}

void lcd_function_set(uint8_t control_flags)
{
  // Function set - 
  // Sets interface data length
  // (DL), number of display lines
  // (N), and character font (F).
  // RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 
  // 0  0   0   0   1   DL   N   F   -   -  
  // DL = 0 --> 4 bit data length (1 = 8 bit)
  // N = 1 --> 2 line display (0 = 1 line)
  // F = 0 --> 5x8 dot character font
  // F = 1 --> 5x10 dot character font
  // 0x28 = 
  // 0  0   0   0   1   0    1  0   0   0
  write_to_lcd(control_flags, LCD_CMD);
  bcm2835_delayMicroseconds(R_EXEC_TIME);  
}

void lcd_control(uint8_t control_flags)
{
  // Display on/off control - 
  // Sets entire display (D) on/off,
  // cursor on/off (C), and
  // blinking of cursor position
  // character (B).
  // RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 
  // 0  0   0   0   0   0   1   D   C   B
  // D = 1 --> display on
  // C = 0 --> cursor off
  // B = 0 --> blinking off
  write_to_lcd(control_flags, LCD_CMD);
  bcm2835_delayMicroseconds(R_EXEC_TIME);
}

void lcd_entry_mode(uint8_t control_flags)
{
  // Entry mode set -
  // Sets cursor move direction
  // and specifies display shift.
  // These operations are
  // performed during data write
  // and read.  
  // Execution time 37us
  // RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 
  // 0  0   0   0   0   0   0   1   I/D S
  // I/D = increment/decrement address pointer
  // I/D = 1 --> increment
  // S = 0 --> no shift 
  write_to_lcd(control_flags, LCD_CMD);
  bcm2835_delayMicroseconds(R_EXEC_TIME);  
}

void clear_lcd(void) {
  // Clear display - 
  // Clears entire display and
  // sets DDRAM address 0 in
  // address counter:
  // RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 
  // 0  0   0   0   0   0   0   0   0   1
  write_to_lcd(0x01, LCD_CMD);  
  bcm2835_delayMicroseconds(R_EXEC_TIME);  
}

void display_off(void)
{
  lcd_control(LCD_CONTROL);
}

void show_cursor(bool on)
{
  uint8_t cursor_flag;
  cursor_flag = ( on ? CURSOR_ON : 0x00);
  lcd_control(LCD_CONTROL|DISPLAY_ON|cursor_flag);
}

void cursor_blinking(bool on)
{
  uint8_t flag;
  flag = ( on ? BLINKING_ON : 0x00);
  lcd_control(LCD_CONTROL|DISPLAY_ON|flag);  
}

void set_cursor(uint8_t addr)
{
  // Set DDRAM address
  // RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 
  // 0  0   1   ADD ADD ADD ADD ADD ADD ADD  
  write_to_lcd(addr, LCD_CMD);
  bcm2835_delayMicroseconds(R_EXEC_TIME);
  // fprintf(stdout,"cursor on addr: 0x%0x\n", addr);  
}

void write_to_lcd(uint8_t byte, uint8_t mode)
{
  switch (mode) {
    case LCD_CMD:
      bcm2835_gpio_clr(LCD_RS);
      // fprintf(stdout, "Set RS to cmd mode\n");
      break;
    case LCD_DATA:
      bcm2835_gpio_set(LCD_RS);
      //bcm2835_delay(5);
      // fprintf(stdout, "Set RS to data mode for '%c'\n", byte);
      break;
    default:
      // fprintf(stderr, "mode for setting GPIO pin unknown: %i\n", mode);
      exit(EXIT_FAILURE);
  }

  // write high bits
  bcm2835_gpio_clr(LCD_D4);
  bcm2835_gpio_clr(LCD_D5);
  bcm2835_gpio_clr(LCD_D6);
  bcm2835_gpio_clr(LCD_D7);

  if ((byte & 0x10) == 0x10) {
    bcm2835_gpio_set(LCD_D4);
  }
  if ((byte & 0x20) == 0x20) {
    bcm2835_gpio_set(LCD_D5);
  }
  if ((byte & 0x40) == 0x40) {
    bcm2835_gpio_set(LCD_D6);
  }
  if ((byte & 0x80) == 0x80) {
    bcm2835_gpio_set(LCD_D7);
  }

  // Toogle ENABLE pin in us 
  bcm2835_delayMicroseconds(E_DELAY);
  bcm2835_gpio_set(LCD_E);  
  bcm2835_delayMicroseconds(E_DATA_PULSE);
  bcm2835_gpio_clr(LCD_E); 
  (LCD_DATA) ? bcm2835_delayMicroseconds(E_DATA_PULSE) :bcm2835_delayMicroseconds(E_CMD_PULSE); 

  // write low bits
  bcm2835_gpio_clr(LCD_D4);
  bcm2835_gpio_clr(LCD_D5);
  bcm2835_gpio_clr(LCD_D6);
  bcm2835_gpio_clr(LCD_D7);

  if ((byte & 0x01) == 0x01) {
    bcm2835_gpio_set(LCD_D4);
  }
  if ((byte & 0x02) == 0x02) {
    bcm2835_gpio_set(LCD_D5);
  }
  if ((byte & 0x04) == 0x04) {
    bcm2835_gpio_set(LCD_D6);
  }
  if ((byte & 0x08) == 0x08) {
    bcm2835_gpio_set(LCD_D7);
  }

  // Toogle ENABLE pin in us 
  // bcm2835_gpio_fsel(LCD_E, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_delayMicroseconds(E_DELAY);
  bcm2835_gpio_set(LCD_E);  
  bcm2835_delayMicroseconds(E_DATA_PULSE);
  bcm2835_gpio_clr(LCD_E); 
  (LCD_DATA) ? bcm2835_delayMicroseconds(E_DATA_PULSE) :bcm2835_delayMicroseconds(E_CMD_PULSE);
  
}

void write_string(char *str)
{
  uint16_t len = (strlen(str) > 16 ? 16 : strlen(str));
  char message[16];
  uint16_t i;

  memset(message, 0x20, 16);
  memcpy(message, str, len);

  for (i=0; i<LCD_WIDTH;i++) {
    write_to_lcd(message[i], LCD_DATA);
  }
}

void gpio_reset(void)
{
  // bcm2835_gpio_fsel(LCD_RS, BCM2835_GPIO_PUD_OFF);
  // bcm2835_gpio_fsel(LCD_E, BCM2835_GPIO_PUD_OFF);
  // bcm2835_gpio_fsel(LCD_D4, BCM2835_GPIO_PUD_OFF);
  // bcm2835_gpio_fsel(LCD_D5, BCM2835_GPIO_PUD_OFF);
  // bcm2835_gpio_fsel(LCD_D6, BCM2835_GPIO_PUD_OFF);
  // bcm2835_gpio_fsel(LCD_D7, BCM2835_GPIO_PUD_OFF);
  
  // bcm2835_gpio_fsel(LCD_RS, BCM2835_GPIO_FSEL_INPT);
  // bcm2835_gpio_fsel(LCD_E, BCM2835_GPIO_FSEL_INPT);
  // bcm2835_gpio_fsel(LCD_D4, BCM2835_GPIO_FSEL_INPT);
  // bcm2835_gpio_fsel(LCD_D5, BCM2835_GPIO_FSEL_INPT);
  // bcm2835_gpio_fsel(LCD_D6, BCM2835_GPIO_FSEL_INPT);
  // bcm2835_gpio_fsel(LCD_D7, BCM2835_GPIO_FSEL_INPT);

  bcm2835_gpio_clr(LCD_RS);
  bcm2835_gpio_clr(LCD_E);
  bcm2835_gpio_clr(LCD_D4);
  bcm2835_gpio_clr(LCD_D5);
  bcm2835_gpio_clr(LCD_D6);
  bcm2835_gpio_clr(LCD_D7);
}
