#include <avr/io.h>
#include <avr/interrupt.h>
#include "inputcap.h"

static volatile unsigned int  overflow_count = 0;
static volatile unsigned int  first_val      = 0;
static volatile unsigned int  second_val     = 0;
static volatile unsigned char first_done     = 0;
static volatile unsigned char capture_done   = 0;

ISR(TIMER1_OVF_vect)
{
    if (first_done && !capture_done)
        overflow_count++;
}

ISR(TIMER1_CAPT_vect)
{
    unsigned int val = ICR1;

    if (!first_done)
    {
        first_val      = val;
        overflow_count = 0;
        first_done     = 1;
    }
    else if (!capture_done)
    {
        second_val   = val;
        capture_done = 1;
    }
}

void setup_input_capture(void)
{
    TCCR1A = 0x00;
    TCCR1B = (1 << ICES1) | (1 << CS10);  /* front montant, prescaler=1 */
    TIMSK1 = (1 << ICIE1) | (1 << TOIE1); /* capture + overflow         */
    TCNT1  = 0;
}

/*
 * measure_cycles()
 *
 * Retourne : overflow_count * 65536 + (cap2 - cap1)
 *
 * Le cast (unsigned int)(cap2 - cap1) gere le wrap-around :
 *   ex: cap1=60000, cap2=5000, ovf=1
 *   delta = (unsigned int)(5000 - 60000) = 10536  (modulo 65536)
 *   total = 65536 + 10536 = 76072  correct
 */
signed long measure_cycles(void)
{
    signed long  total;
    unsigned int cap1, cap2, ovf;

    cli();
    capture_done   = 0;
    first_done     = 0;
    overflow_count = 0;
    sei();

    while (!capture_done);

    cli();
    cap1 = first_val;
    cap2 = second_val;
    ovf  = overflow_count;
    sei();

    total = (signed long)ovf * 65536L
          + (signed long)(unsigned int)(cap2 - cap1);

    return total;
}

void erreur(void)
{
    DDRB  |= (1 << PB0);
    PORTB |= (1 << PB0);
    while (1);
}
