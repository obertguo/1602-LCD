#ifndef LCD_h
#define LCD_h

#include <inttypes.h>

//This library will use 4 bit mode, using pins d4-d7, and will initialize with a 16x2 line display 
//Only pins D4-D7 will accept data
class LCD{
    public:
        LCD(uint8_t rs, uint8_t enable, 
            uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

        void clear();

        void printChar(uint8_t c);

        void printString(const char str[]);

        void displayOn();

        void displayOff();

    private:
        //Store register select, enable, and d4-d7 data pins
        const uint8_t _rs;
        const uint8_t _enable;
        const uint8_t _dataPins[4];

        //Execute instructions
        //Params - 8 bit instruction to write to data pins d4-d7 and a boolean mode that represents whether to write HIGH or LOW to RS  
        inline void execute(uint8_t instruction, bool mode);

        //Write instructions to data pins (4 bit mode)
        void write(uint8_t instruction);

        //Handle writing and timings for enable pins
        void enable();

};

#endif