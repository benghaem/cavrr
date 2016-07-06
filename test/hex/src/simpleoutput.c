#include <avr/io.h>
#include <avr/cpufunc.h>
int main(void){

    /* Enable output */
    DDRB = 0xFF;

    /* PORTB0 11110000 */
    PORTB = 0xF0;

    while(1){
        PORTB = ~PORTB;
        _NOP();
    }
}
