Diorama LED Flicker
-------------------

This is an electronics project featuring an ATtiny85 and some support circuitry.
It provides a simple way to have multiple dimmed lights, with one light 
flickering in a "broken" fashion. The fluorescent pattern tries to look like a 
worn out fluorescent tube, and the incandescent pattern tries to look like a 
glitchy tungsten light. 

It is intended to be used in dioramas and/or trainsets.

## High level details
All the logic is performed by the ATtiny85 and the output is a PWM signal done 
by the built-in PWM module. 

The ATtiny85 has two inputs, a "mode" switch, a dimming input and a pattern 
switch.

### Dimming input
The dimming input can be left tied to 5V, or a potentiometer or resistor can be 
placed in series to dim the output. You could also control it with another MCU.

It uses 8 bits of resolution, so there's 255 possible values. Tying the dimming 
input to 5V effectively disables dimming. It will still read the value, but the
outputs will not be clamped.

### Outputs
The output consists of two signals. One PWM signal with a duty cycle directly
determined by the dimming input. The other is the flickering pattern, with a duty
cycle clamped to the dimming input.

This means, that with a 2.5V value on the dimming input, both outputs have a 
maximum duty cycle of 50%.

## Programming details
The flickering patterns are generated with a linear shift register as the 
pseudo-random source, with only 8 bits resolution. So the period is pretty 
short. Attempts have been made to make the period less noticeable.

## Building
KiCad is used for the schematics. There's no board layout as of now.

For the code, the main target is avr-gcc. If you have Arduino installed, you can
use the toolchain bundled with it.

As long as the avr-gcc binaries is in the shell path, and you have a suitable 
version of make (mingw32-make will work), the following should suffice:

```make```

The following targets are supported:

- `all`
- `clean`
- `disasm`
- `dude`

## License
MIT all the way, baby

