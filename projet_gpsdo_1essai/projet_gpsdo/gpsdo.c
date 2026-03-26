#define F_CPU 16000000UL
#include <avr/io.h> //E/S ex PORTB
#include <util/delay.h>
#include "VirtualSerial.h"

int main(void){
    MCUCR |= (1 << JTD);
    MCUCR |= (1 << JTD);
    SetupHardware();

    DDRF |= (1 << PF1);   
    DDRF |= (1 << PF4);   
    DDRF |= (1 << PF5);   
    DDRF |= (1 << PF6); 

    while(1)
    {
        /* Allumer toutes les LEDs */
        PORTF |= (1 << PF1);
        PORTF |= (1 << PF4);
        PORTF |= (1 << PF5);
        PORTF |= (1 << PF6);
        _delay_ms(500);
        /* Eteindre toutes les LEDs */
        PORTF &= ~(1 << PF1);
        PORTF &= ~(1 << PF4);
        PORTF &= ~(1 << PF5);
        PORTF &= ~(1 << PF6);
        _delay_ms(500);
    }

    return 0;
}
