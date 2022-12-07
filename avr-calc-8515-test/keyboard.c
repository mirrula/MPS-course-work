#include <avr/io.h>
#include <util/delay.h>

#include "keyboard.h"

unsigned char keys[4][6] = {
    {'7', '8', '9', 'm', 'v', 'O'},
    {'4', '5', '6', '*', '/', 'C'},
    {'1', '2', '3', '+', '-', '='},
    {'0', '.', 'p', 'u', 'A', '='}
};

void init_keyboard() {
    DDRC |= 0x0F; // PB 0-3 output for rows.
    DDRD &= 0x03; // PC 2-7 input for columns.
}

unsigned char scan_key() {
    while (1) {
        PORTD |= 0xFC;
        
		for(int i = 0;i<4;i++){
            PORTC = ~(1 << i);
            for(int j = 2;j < 8;j++){
                if(bit_is_clear(PIND, j)){
                    while(bit_is_clear(PIND, j)) _delay_ms(50);
                    return keys[i][j-2];
                }
            }
        }

    }
}

