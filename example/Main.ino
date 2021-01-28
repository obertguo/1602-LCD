//Include driver code
#include "./LCD.h"

//Define pins
const int d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int rs = 12, enable = 11;

void setup(){
    LCD lcd = LCD(rs, enable, d4, d5, d6, d7);
    
    //Print a single character to the display
    lcd.printChar('a');
    
    //Print a string to the display
    lcd.printString("Hello World!");
}

void loop(){
}
