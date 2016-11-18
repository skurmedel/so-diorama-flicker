DEVICE=attiny85
CLOCK=1000000 # 1mHz

SOURCES=src/flicker.c src/main.c
OBJECTS=$(SOURCES:.c=.o)
CC=avr-gcc
CFLAGS=-Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -g

main.elf: $(OBJECTS)
	$(CC) $(CFLAGS) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.ex
	avr-objcopy -j .text -j .data - O ihex main.elf main.hex 
	avr-size --format=avr --mcu=$(DEVICE) main.elf

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@

all: main.hex

disasm: main.elf
	avr-objdump -d main.elf