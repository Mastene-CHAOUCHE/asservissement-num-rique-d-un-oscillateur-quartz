#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "inputcap.h"
#include "VirtualSerial.h"

/* --- CONFIGURATION --- */
#define F_CPU_HZ   16000000L
#define TIMER_MAX  65536L

extern USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface;
extern FILE USBSerialStream;


// SECTION PWM (TIMER 4) - CONFIGURATION PD7================================

void init_pwm_timer4(void)
{
    /* 1. Configuration de PD7 (OC4D) en sortie */
    DDRD |= (1 << PD7);

    /* 2. Configuration du plafond (TOP) à 1023 pour un PWM 10 bits */
    // On utilise TC4H pour les bits de poids fort (obligatoire sur Timer 4)
    TC4H = (1023 >> 8);
    OCR4C = (1023 & 0xFF);

    /* 3. Réglage du rapport cyclique initial à 50% (512), pour mettre un duty cycle de 100% il faut 1023 (1024 bruit)*/
	TC4H = (512 >> 8);    
	OCR4D = (512 & 0xFF);
	
    /* 4. Registres de contrôle */
    // COM4D1 : Active le PWM sur la broche D (PD7)
    // PWM4D  : Active le mode Modulateur de Largeur d'Impulsion
    TCCR4C = (1 << COM4D1) | (1 << PWM4D);
    
    // TCCR4B : Sélection de l'horloge (CS40 = Pas de diviseur)
    // Fréquence PWM = 64MHz (PLL) / 1024 = 62.5 kHz
    TCCR4B = (1 << CS40);
}


// =================================  MAIN  =====================
int main(void)
{
    /* Initialisation Matérielle LUFA */
    SetupHardware();
    CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

    /* --- INITIALISATION PWM --- */
    init_pwm_timer4();

    /* --- INITIALISATION INPUT CAPTURE --- */
    /* PD4 (ICP1) en entree */
    DDRD &= ~(1 << PD4);
    setup_input_capture();

    /* Activation des interruptions */
    GlobalInterruptEnable();

    signed long raw_cycles;
    signed long freq_hz;
    signed long freq_khz;
    signed long freq_frac;

    while (1)
    {
        /* 1. Mesure de la fréquence (Bloquant) */
        raw_cycles = measure_cycles();

        if (raw_cycles <= 0)
        {
            fprintf(&USBSerialStream,
                    "ERR: cycles negatifs (%ld) - verifier signal ICP1\r\n",
                    raw_cycles);
        }
        else
        {
            /* Calcul de la fréquence */
            freq_hz   = F_CPU_HZ / raw_cycles;
            freq_khz  = freq_hz / 1000L;
            freq_frac = freq_hz % 1000L;

            /* Affichage vers le terminal série */
            fprintf(&USBSerialStream,
                    "Cycles: %ld  |  Freq: %ld.%03ld kHz\r\n",
                    raw_cycles,
                    freq_khz,
                    freq_frac);
        }

        /* Tâches de maintenance USB */
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
        USB_USBTask();
    }

    return 0;
}
