Raspberry Pi v2 LCD Display control
===================================

This program shows small text lines on LCD displays with the HD44780 chip connected to the GPIO ports of a Raspberry Pi V2. 

![The HD44780 Display](https://dl.dropboxusercontent.com/u/40629133/HD44780/Display.jpg)

Currently the 2 lines a 16 characters display is supported. The neccessary 
hardware for testing is listed below: 

* [LCD1602A](https://www.google.de/#q=HD44780+2x16+buy)
* a breadboard
* jumper wires male/female
* a jumper kit
* a 3.3 k resistor
* a 560 ohm resistor

Thanks to Matt Hawkins (http://www.raspberrypi-spy.co.uk) for explaining the wiring.

The program is written in C and uses the bcm2835 lib from Mike McCauley at( http://www.airspayce.com/mikem/bcm2835/ )
So get a copy and compile the lib:

```
./configure 
make
sudo make install
```

Once the lib is installed you are ready to compile the test program. Clone this source and run make: 

```
git clone https://github.com/rheikvaneyck/HD44780.git
cd HD44780
make
```

To print some text on the display call the program with option -1 <text> for text on line 1 and -2 <text> for text on line 2:

```
sudo ./lcd_test -1 "Hallo Welt" -2 "Wie geht's?"
```

  
