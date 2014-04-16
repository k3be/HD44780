LCD Display control
===================

This program shows small text lines on LCD displays with the HD44780 chip. 

![The HD44780 Display](https://dl.dropboxusercontent.com/u/40629133/HD44780/Display.jpg)

Currently the 2 lines a 16 character display is supported. The neccessary 
hardware for testing is listed below: 

* LCD1602A
* a breadboard
* jumper wires male/female
* a jumper kit
* a 3.3 k resistor
* a 560 ohm resistor

Thanks for explaining the wiring to Matt Hawkins (http://www.raspberrypi-spy.co.uk).

The program is written in C and uses the bcm2835 lib from Mike McCauley at http://www.airspayce.com/mikem/bcm2835/ 

* bcm2835 lib 

So get a copy and compile the lib:
