#include "./LCD.h"
#include "Arduino.h"

const uint8_t CLEAR = 0x01; 

const uint8_t FUNCTION_SET = 0x28; //4 bit mode, 2 line display, 5x8 dot format 
const uint8_t FOUR_BIT_MODE = 0x20; 

const uint8_t DISPLAY_ON = 0x0c;
const uint8_t DISPLAY_OFF = 0x08;

const uint8_t ENTRY_MODE_SET = 0x06;

//Initialize pins
LCD::LCD(uint8_t rs, uint8_t enable, 
        uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) : 
        _rs(rs), _enable(enable), _dataPins{d4, d5, d6, d7}
{

    //Set all pins as outputs
    pinMode(_rs, OUTPUT);
    pinMode(_enable, OUTPUT);

    for(int i = 0; i < 4; i++){
        pinMode(_dataPins[i], OUTPUT);
    }

    /*
        SEE INITIALIZATION PROCESS HERE
        https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
        PG 46
    */

    //Wait for LCD to power on - wait for at least 40ms after Vcc rises to 2.7V 
    delayMicroseconds(40000);

    //Make sure the rs and enable pins are off - just in case if they were on previously
    digitalWrite(_rs, LOW); 
    digitalWrite(_enable, LOW);

    //1st try
    execute(0x3, false);  //Function set - LCD interface is in 8 bit mode
    delayMicroseconds(4500); //Wait at least 4.1ms
    
    //2nd try
    execute(0x3, false);  //Function set - LCD interface is in 8 bit mode
    delayMicroseconds(4500); //Wait at least 4.1ms

    //3rd try
    execute(0x3, false);  //Function set - LCD interface is in 8 bit mode
    delayMicroseconds(200); //Wait at least 100 microseconds 

    //All set, ready to set to 4 bit mode
    execute(FOUR_BIT_MODE, false);
    
    //Alright, in 4 bit mode! 

    //Complete the final initialization process
    //Specify display lines and character font 
    execute(FUNCTION_SET, false);
    displayOff();
    clear();
    execute(ENTRY_MODE_SET, false);
    displayOn();
}

void LCD::clear(){
    execute(CLEAR, false);
    delayMicroseconds(2000);
    //Takes a while to clear since theoretically, the LCD writes a bunch of space characters to DDRAM
}

void LCD::printChar(uint8_t c){
    //Make sure RS is enabled so that we can write to DDRAM
    execute(c, true);
}

void LCD::printString(const char str[]){
    //Strings are simply character arrays, so just iterate the print char function until we reach a null reference
    int i = 0;
    while(str[i]){
        printChar((uint8_t)str[i]);
        i++;
    }
}

void LCD::displayOn(){
    execute(DISPLAY_ON, false);
}

void LCD::displayOff(){
    execute(DISPLAY_OFF, false);
}

//Params - 8 bit instruction to write to data pins d4-d7 and a boolean mode that represents whether to write HIGH or LOW to RS  
inline void LCD::execute(uint8_t instruction, bool mode){
    digitalWrite(_rs, mode);
    
    write(instruction >> 4); //get first 4 bits of instruction 
    write(instruction); //last 4 bits are already in place, no need to bitshift
}

//Write instructions to data pins (4 bit mode)
void LCD::write(uint8_t instruction){
    for(int i = 0; i < 4; i++){
        //determine what what value to write by bitshifting and masking 
        digitalWrite(_dataPins[i], (instruction >> i) & 0x01); 
    }
    //Write HIGH to enable pin so that the LCD processes the instructions
    enable();
}

//Handle writing and timings for enable pins
void LCD::enable(){
    //Total enable cycle time must be > 1000 ns
    digitalWrite(_enable, HIGH);
    delayMicroseconds(1); //Must be HIGH at least 450ns according to datasheet
    digitalWrite(_enable, LOW);
    delayMicroseconds(50); //at least 37-40 µs is needed to execute 
}