all:  lcd_test

lcd_test: lcd_test.o LCDHD44780.o
	gcc -Wall -O2 lcd_test.o LCDHD44780.o -lbcm2835 -o lcd_test

lcd_test.o: lcd_test.c
	gcc -Wall -c lcd_test.c -o lcd_test.o

LCDHD44780.o: LCDHD44780.h LCDHD44780.c
	gcc -Wall -c LCDHD44780.c -o LCDHD44780.o

clean:
	rm -f *.o *~ lcd_test