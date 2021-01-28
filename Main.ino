#include "./LCD.h"

//Define pins
const int d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int rs = 12, enable = 11;

void setup(){
    LCD lcd = LCD(rs, enable, d4, d5, d6, d7);

    lcd.printChar('a');
    lcd.printString("This is cool!!!");
}

void loop(){


}