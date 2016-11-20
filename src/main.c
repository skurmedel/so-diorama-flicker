#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>

struct input_pin
{
    uint8_t     n;
    enum 
    {
        PULL_UP_OFF = 0,
        PULL_UP_ON  = 1      
    } pull_up;
};

struct output_pin
{
    uint8_t     n;
};

/*
    The pins used for different functions. They correspond to PORTB numbers. 

    The defaults are set according to the general schematic.
*/
static const struct input_pin  PIN_IN_FLICKER_MODE_SWITCH    = { 2, PULL_UP_ON };
static const struct input_pin  PIN_IN_FLICKER_ONOFF_SWITCH   = { 3, PULL_UP_ON };
static const struct input_pin  PIN_IN_DIMMING_POT            = { 4, PULL_UP_OFF };
static const struct output_pin PIN_OUT_FLICKER               = { 0 };
static const struct output_pin PIN_OUT_DIMMED                = { 1 };


/*
    Called on reset to set up the hardware timers and interrupts.
*/
void init_timers(void)
{
    /* Activate timer synchronization. */
    GTCCR |= _BV(TSM); 

    /*
        - Set 0C0A & 0C0B on PWM (make PWM output active on those pins.)
        - Waveform generation mode to "fast PWM" with BOTTOM update and TOV on MAX.
    */
    TCCR0A &= 0xAF;
    /*
        Waveform TOV flag set on TOP instead.
    */
    TCCR0B |= 0x08;
    /*
        Use clkIO with no prescaling.
    */
    TCCR0B |= 0x01;
    /*
        Turn on Timer/Counter0 interrupts on overflow.
    */
    TIMSK |= 0x02;

    /* 
        On the ATtiny85, these pins won't get PWMed unless they are configured
        as outputs in DDRB, so we do just that.
    */
    DDRB |=  (_BV(PIN_OUT_DIMMED.n) | _BV(PIN_OUT_FLICKER.n));

    /*
        Set TOP to 128 which gives us ~50% duty cycle.
    */
    OCR0A = 0x80;

    GTCCR &= ~_BV(TSM);
}

/*
    Called on reset to set up the IO pins and interrupts.
*/
void init_io(void)
{    
    /*
        Set pull ups for inputs.
    */
    PORTB |= (_BV(PIN_IN_FLICKER_ONOFF_SWITCH.n) * PIN_IN_FLICKER_ONOFF_SWITCH.pull_up)
          |  (_BV(PIN_IN_FLICKER_MODE_SWITCH.n) * PIN_IN_FLICKER_MODE_SWITCH.pull_up);
}

int main(void)
{
    /* 
        Put init stuff inside here, and don't touch the interrupt enable flags.
    */
    ATOMIC_BLOCK(ATOMIC_FORCEON) 
    {
        init_io();
        init_timers();
    }

    while (1)
    {
        sleep_mode();
    }

    return 0;
}