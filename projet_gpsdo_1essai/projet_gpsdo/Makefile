MCU    = atmega32u4
F_CPU  = 16000000UL
CC     = avr-gcc
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall

EXEC = inputcap
DFU  = /usr/bin/dfu-programmer

# === Sources ===
SRC = main.c inputcap.c PID.c

# === Headers ===
HDR = inputcap.h PID.h

all: $(EXEC).hex

$(EXEC).elf: $(SRC) $(HDR)
	$(CC) $(CFLAGS) \
		-DF_USB=16000000UL \
		-I../lufa/VirtualSerial_example \
		-I../lufa/lufa-LUFA-140928 \
		-g3 \
		-o $@ $(SRC) \
		-L../lufa/VirtualSerial_lib -lVirtualSerial \
		-lm

$(EXEC).hex: $(EXEC).elf
	avr-objcopy -O ihex $< $@

install: $(EXEC).hex
	$(DFU) $(MCU) erase
	$(DFU) $(MCU) flash $(EXEC).hex
	$(DFU) $(MCU) reset

clean:
	rm -f *.elf *.hex
