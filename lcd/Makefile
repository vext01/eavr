# EAVR Makefile
# $Id$

PROG=ealcd

# tools
AVRDUDE=avrdude -F -V
OBJCOPY=avr-objcopy
CC=avr-gcc
RM=rm -f

# parameters
MCU=atmega328
F_CPU=16000000UL
BIN_FORMAT=ihex
PORT=/dev/cuaU0
#BAUD=57600
BAUD=115200
PROTOCOL=arduino
PART=ATMEGA328P
CFLAGS=-Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)
SUDO=sudo

.c.o: ealcd_params.h
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: all
all: ${PROG}.o main.o ${PROG}.hex

${PROG}.hex: ealcd.o main.o
	${CC} ${CFLAGS} -o ${PROG} ${PROG}.o main.o
	$(OBJCOPY) -O $(BIN_FORMAT) -R .eeprom ${PROG} ${PROG}.hex

.PHONY: clean
clean:
	$(RM) ${PROG}.hex ${PROG}.o main.o ${PROG}

.PHONY: upload
upload: ${PROG}.hex
	${SUDO} $(AVRDUDE) -c $(PROTOCOL) -p $(PART) -P $(PORT) \
		-b $(BAUD) -U flash:w:${PROG}.hex

