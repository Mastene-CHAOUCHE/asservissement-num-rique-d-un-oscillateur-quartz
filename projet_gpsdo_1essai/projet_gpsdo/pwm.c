#define F_CPU 16000000UL
#include <avr/io.h>
#include "VirtualSerial.h"
int main(void)
{    SetupHardware();
    ////ajout pour débloquer les 3 leds d'après D4 D3 D2////
    
    
    /* 1. PD7 en sortie */
    DDRD |= (1 << PD7);

    /* 2. Configuration Timer 4 pour PWM 10 bits sur PD7
     * On enlève les diviseurs lents pour avoir un signal stable.
     */
    
    // On règle le plafond à 1023 (Mode 10 bits)
    // Pour le Timer 4, on utilise le registre TC4H pour les bits de poids fort
    TC4H = (1023 >> 8);
    OCR4C = (1023 & 0xFF);

    // On règle la valeur initiale à 50% (512)
    TC4H = (512 >> 8);
    OCR4D = (512 & 0xFF);

    // Configuration des registres de contrôle
    // COM4D1 : Active le PWM sur PD7 (OC4D)
    // PWM4D : Mode PWM
    TCCR4C = (1 << COM4D1) | (1 << PWM4D);
    
    // TCCR4B : On choisit CS40 (Pas de diviseur) 
    // F_PWM = 64MHz / 1024 = 62.5 kHz
    TCCR4B = (1 << CS40);
    
    
    ////ajout définition des ports/////
   
    while(1)
    {
        //Ta boucle de contrôle viendra ici
        //pour allumer et éteindre les 4 leds
       
    }

    return 0;
}
