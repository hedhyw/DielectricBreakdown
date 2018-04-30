MMCU=atmega48p
CC=./main.c ./display.c ./m74hc595.c ./calibration.c
OBJ=/tmp/3Mh59ZFX.elf
HEX=/tmp/3Mh59ZFX_OUT.hex
EEPROM=/tmp/3Mh59ZFX_EEPROM.hex
LFUSE=/tmp/3Mh59ZFX_LFUSE.txt
all:
	avr-gcc -Wall -std=gnu99 -mmcu=$(MMCU) -Os $(CC) -L. -o $(OBJ)
	avr-objcopy -j .text -j .data -O ihex $(OBJ) $(HEX)
	avr-objcopy -j .eeprom --change-section-lma .eeprom=0 -O ihex $(OBJ) $(EEPROM)
	xxd -r -p $(EEPROM) /tmp/eeprom.bin
	cp $(HEX) /tmp/
upload:
	avrdude -p $(MMCU) -c usbasp -U flash:w:$(HEX) -U eeprom:w:$(EEPROM)
lfuse_read:
	avrdude -p $(MMCU) -c usbasp -U lfuse:r:$(LFUSE):h
	cat $(LFUSE)
