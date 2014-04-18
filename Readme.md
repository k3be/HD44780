Raspberry Pi v2 LCD Display control
===================================

This program shows small text lines on LCD displays with the HD44780 chip connected to the GPIO ports of a Raspberry Pi V2. 

![The HD44780 Display](https://dl.dropboxusercontent.com/u/40629133/HD44780/Display.jpg)

Currently the 2 lines a 16 characters display is supported. The neccessary 
hardware for testing is listed below: 

* [LCD1602A](https://www.google.de/#q=HD44780+2x16+buy)
* [a breadboard](https://www.google.de/#q=Steckbrett+buy)
* [jumper wires male/female](https://www.google.de/#q=Jumper-Cable+m-f+kit+buy)
* [a jumper kit](https://www.google.de/#q=Jumper-Cable+kit+buy)
* [a 3.3 k resistor](https://www.google.de/#q=3.3k+ohm+resistor+buy)
* [a 560 ohm resistor](https://www.google.de/#q=560+ohm+0.25+W+resistor+buy)

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

  
