#include "flicker.h"

struct flicker_state flicker_state_create(uint8_t seed)
{
    return (struct flicker_state)
    {
        seed,
        0xFF,
        0x40,
        0
    };
}

inline uint8_t advance_lsfr(struct flicker_state *s)
{
    uint8_t bits = ((s->seed >> 0) ^ (s->seed >> 2) ^ (s->seed >> 3) ^ (s->seed >> 4)) & 1;
    return (s->seed = (s->seed >> 1) | (bits << 7));
}

uint8_t flicker_incandescent(struct flicker_state *s)
{
    if (s->prev < s->target)
    {
        s->prev += 1;
    }
    else if (s->prev > s->target)
    {
        s->prev -= 1;
    }

    s->cycle += 1;
    if (s->cycle > 239 - 1)
    {
        uint8_t k = advance_lsfr(s);
        s->target = k ^ s->prev;
        if (k % 11 == 0) 
            s->target = k >> 5;
        else
            s->target = 255;
        s->cycle = 0;
    }

    return s->prev;
}

uint8_t flicker_fluorescent(struct flicker_state *s)
{
    if (s->prev < s->target)
    {
        s->prev += 1;
    }
    else if (s->prev > s->target)
    {
        s->prev -= 1;
    }

    s->cycle += 1;
    if (s->cycle > 239 - 1)
    {
        s->target = advance_lsfr(s) ^ s->prev; 
        s->cycle = 0;
    }

    return s->prev;
} 
