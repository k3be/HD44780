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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <bcm2835.h>
#include <getopt.h>

// Define GPIO pin to LCD pin mapping
#define LCD_RS RPI_V2_GPIO_P1_26
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

int print_usage(int errcode, char *program_name);

void init_gpio(void);

void init_lcd(void);

void set_curser(uint8_t addr);

void write_to_lcd(uint8_t byte, uint8_t mode);

void write_string(char *str);

void gpio_reset(void);

