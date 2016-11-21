#ifndef FLICKER_H
#define FLICKER_H
/*
    Emulates a light flickering at it's "end of life". The output is suitable 
    for using as a TOP-value in an Atmel PWM module.
*/

#include <stdint.h>

struct flicker_state
{
    /*
        Internal values, don't touch them unless you have to.
    */
    /*
        PRNG seed.
    */
    uint8_t     seed;
    /*
        Value outputted last call.
    */
    uint8_t     prev;
    /*
        Target value, we move towards this over several invocations.
    */
    uint8_t     target;
    /*
        Current cycle counter.
    */
    uint8_t     cycle;
};

/*
    Create a new flicker state.
*/
struct flicker_state flicker_state_create(uint8_t seed);

/*
    Tries to mimic the look of an incandescent light bulb flickering.

    Returns
        A value from 0 to 255 representing the brightness of the light.
*/
uint8_t flicker_incandescent(struct flicker_state *);
/*
    The annoying flickering of a flourescent light tube.

    Returns
        A value from 0 to 255 representing the brightness of the light.
*/
uint8_t flicker_fluorescent(struct flicker_state *); 

#endif 