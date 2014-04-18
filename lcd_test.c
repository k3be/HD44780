/*##############################################################*/
/* Author: Marcus Nasarek                                       */
/* File:   lcd_test.c                                           */
/* Compile with:                                                */
/*      gcc -Wall -c LCDHD44780.c -o LCDHD44780.o               */
/*      gcc -Wall -c lcd_test.c -o lcd_test.o                   */
/*      gcc -Wall lcd_test.o LCDHD44780.o -lbcm2835 -o lcd_test */
/* Help:                                                        */
/*      lcd_test -h                                             */
/* Usage:                                                       */
/* sudo ./lcd_test [options]                                    */
/*  Invoking lcd_test to set the Text of a HD44780 LCD 2x16     */ 
/*  character display.                                          */
/*                                                              */
/*  The following are the options, which must be a single       */
/*  letter preceded by a '-' and followed by another character. */
/*  -1 <text> sets the text of line 1 max 16 characters long.   */
/*  -2 <text> sets the text of line 2 max 16 characters long.   */
/*  -c clears both lines of the display.                        */
/*                                                              */
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

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include "LCDHD44780.h"

int print_usage(int errcode, char *program_name)
{
  fprintf(stderr,"%s \n", program_name);
  fprintf(stderr,"Usage: \n");
  fprintf(stderr,"  %s [options]\n", program_name);
  fprintf(stderr,"\n");
  fprintf(stderr,"  Invoking %s to set the Text of a HD44780 LCD 2x16 character display.\n", program_name);
  fprintf(stderr,"\n");
  fprintf(stderr,"  The following are the options, which must be a single letter\n");
  fprintf(stderr,"    preceded by a '-' and followed by another character.\n");
  fprintf(stderr,"    -1 <text> sets the text of line 1. Text is max 16 characters long.\n");
  fprintf(stderr,"    -2 <text> sets the text of line 2. Text is max 16 characters long.\n");
  fprintf(stderr,"    -c clears both lines of the display.\n");
  fprintf(stderr,"    -p leave the text on the display. Cannot be used with -c\n");
  fprintf(stderr,"\n");

    return errcode;
}

int main(int argc, char **argv) 
{
  int opt;
  char line_1[LCD_WIDTH];
  char line_2[LCD_WIDTH];
  bool clear = false;
  bool keep = false;
  bool line_1_set = false;
  bool line_2_set = false;
  uint8_t data = 0x00;


  while((opt = getopt(argc, argv, "1:2:cph")) != -1) {
    switch (opt) {
      case '1':
        strncpy(line_1, optarg, 16);
        line_1_set = true;
        break;
      case '2':
        strncpy(line_2, optarg, 16);
        line_2_set = true;
        break;
      case 'c':
        clear = true;
        break;
      case 'p':
        clear = false;
        keep = true;
        break;
      case 'h':
        print_usage(EXIT_SUCCESS, argv[0]);
        break;
      default:
        exit(print_usage(EXIT_FAILURE, argv[0]));
    }
  }

  if (!bcm2835_init()) return 1;

  init_lcd();

  if (clear) 
    exit(EXIT_SUCCESS);

  if (!(line_1_set || line_2_set)) {
    strcpy(line_1, "Hallo");
    strcpy(line_2, "Ciao Bella!");

    set_cursor(LCD_LINE_1);
    write_string(line_1);  

    delay(2000);

    set_cursor(0x86);
    show_cursor(true);

    delay(2000);

    write_string("Welt!");

    delay(1000);

    shift_display(DISPLAY_SHIFT, SHIFT_RIGHT);

    delay(1000);

    shift_display(DISPLAY_SHIFT, SHIFT_RIGHT);

    delay(1000);

    shift_display(DISPLAY_SHIFT, SHIFT_RIGHT);

    delay(1000);

    shift_display(CURSOR_SHIFT, SHIFT_LEFT);
    write_string("?");

    delay(2000);

    shift_display(DISPLAY_SHIFT, SHIFT_LEFT);
    shift_display(DISPLAY_SHIFT, SHIFT_LEFT);
    shift_display(DISPLAY_SHIFT, SHIFT_LEFT);

    delay(2000);

    set_cursor(LCD_LINE_2);
    
    cursor_blinking(true);

    delay(2000);

    write_string(line_2);   
  } 
  else 
  {
    if (line_1_set) {
      set_cursor(LCD_LINE_1);
      write_string(line_1);        
    }
    if (line_2_set) {
      set_cursor(LCD_LINE_2);
      write_string(line_2);      
    }
  }

  delay(2000);

  if (!keep) {
    clear_lcd();

    display_off();
    
    gpio_reset();    
  }

  bcm2835_close();

  exit(EXIT_SUCCESS);
}