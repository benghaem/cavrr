#include <avr/io.h>
#include <avr/cpufunc.h>

void delay(int cycles){
    for (;cycles > 0; cycles--){
        _NOP();
    }
}

int main(void){

    /* Enable output */
    DDRB = 0xFF;

    /* PORTB0 11110000 */
    PORTB = 0xF0;

    while(1){
        PORTB = ~PORTB;
        delay(10);
    }
}
