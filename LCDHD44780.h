/*##############################################################*/
/* Author: Marcus Nasarek                                       */
/* File:   LCDH44780.h                                          */
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

// Thanks to Matt Hawkins (http://www.raspberrypi-spy.co.uk)
// The wiring for the LCD is as follows:
// 1 : GND
// 2 : 5V
// 3 : Contrast (0-5V)*
// 4 : RS (Register Select)
// 5 : R/W (Read Write)       - GROUND THIS PIN
// 6 : Enable or Strobe
// 7 : Data Bit 0             - NOT USED
// 8 : Data Bit 1             - NOT USED
// 9 : Data Bit 2             - NOT USED
// 10: Data Bit 3             - NOT USED
// 11: Data Bit 4
// 12: Data Bit 5
// 13: Data Bit 6
// 14: Data Bit 7
// 15: LCD Backlight +5V**
// 16: LCD Backlight GND

#ifndef _LCDHD44780_H_
#define _LCDHD44780_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <bcm2835.h>

// Define GPIO pin to LCD pin mapping
#define LCD_RS RPI_V2_GPIO_P1_26
#define LCD_RW RPI_V2_GPIO_P1_07
#define LCD_E  RPI_V2_GPIO_P1_24
#define LCD_D4 RPI_V2_GPIO_P1_22 
#define LCD_D5 RPI_V2_GPIO_P1_18
#define LCD_D6 RPI_V2_GPIO_P1_16
#define LCD_D7 RPI_V2_GPIO_P1_12

// Define some device constants
// Maximum characters per line
#define LCD_WIDTH 16    
#define LCD_DATA HIGH 
#define LCD_CMD LOW

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
#define LCD_FUNCTION_SET 0x20
#define DATA_LENGTH      0x10
#define TWO_LINES        0x08
#define BIG_FONT_SET     0x04



// Display control flags
// Sets entire display (D) on/off,
// cursor on/off (C), and
// blinking of cursor position
// character (B).
// RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 
// 0  0   0   0   0   0   1   D   C   B
// D = 1 --> display on
// C = 0 --> cursor off
// B = 0 --> blinking off
#define LCD_CONTROL 0x08 // display control
#define DISPLAY_ON  0x04 
#define DISPLAY_OFF 0x00
#define CURSOR_ON	0x02
#define BLINKING_ON 0x01

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
#define LCD_ENTRY_MODE 0x04
#define ADDR_INCREMENT 0x02
#define LINE_SHIFT	   0x01

// Cursor or display shift -
// Moves the cursor and shifts
// display without chaning
// DDRAM contents. 
// Execution time 37us
// RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 
// 0  0   0   0   0   1   S/C R/L -   -
// S/C = cursor/display shift
// S/C = 0 --> cursor shift
// R/L = shift direction
// R/L = 1 --> shift to right  
#define CURSOR_DISPLAY_SHIFT 0x10
#define CURSOR_SHIFT         0x00
#define DISPLAY_SHIFT        0x08
#define SHIFT_RIGHT          0x04
#define SHIFT_LEFT           0x00


// LCD DDRAM address for the 1st line
// bits DB0..DB6 --> 0x00..0x4F
// DB7 = 1 --> offset = 0x80
// RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 
// 0  0   1   ADD ADD ADD ADD ADD ADD ADD
// 0x80 =
// 0  0   1   0   0   0   0   0   0   0  
#define LCD_LINE_1 0x80 
// LCD DDRAM address for the 2nd line
// 2 lines with equal # of addresses
// DB7 = 1 --> offset = 0x80 + (0x00..0x4F)/2 = 0xC0
#define LCD_LINE_2 0xC0 

// Timing constants in us
#define E_DELAY 1         // us
#define E_CMD_PULSE 5000  // us
#define E_DATA_PULSE 5000 // us
#define C_DELAY 1         // us
#define R_EXEC_TIME 50    // us
#define GPIO_RISING 10    // ms

#define MAX_WAIT 100

void init_gpio(void);

void init_lcd(void);

void lcd_function_set(uint8_t control_flags);

void lcd_control(uint8_t control_flags);

void lcd_entry_mode(uint8_t control_flags);

void lcd_shift_control(uint8_t control_flags);

void clear_lcd(void);

void set_cursor(uint8_t addr);

uint8_t busy_wait(void);

void display_off(void);

void show_cursor(bool on);

void cursor_blinking(bool on);

void shift_cursor(uint8_t direction);

void shift_display(uint8_t direction);

void return_home(void);

void write_to_lcd(uint8_t byte, uint8_t mode);

void write_string(char *str);

uint8_t read_from_lcd(uint8_t mode);

void gpio_reset(void);

#endif
