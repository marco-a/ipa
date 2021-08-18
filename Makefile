CC = avr-gcc
CFLAGS = -std=gnu99 -Wall -DF_CPU=16000000UL -mmcu=atmega16a -Os -I"./src/lib/" -I"./src/"


all:
	rm -f PROGRAM.elf
	rm -f PROGRAM.hex

	$(CC) $(CFLAGS) -o PROGRAM.elf src/main.c src/lib/LCD/_lcd.c src/lib/common/common.c src/lib/SigGen/SigGen.c src/lib/Timer/Timer.c src/lib/ExtADC/ExtADC.c src/lib/FreqCounter/FreqCounter.c src/lib/Measure/Measure.c src/lib/IntADC/IntADC.c src/lib/TWI/TWI.c src/lib/LCD/LCD.c src/lib/Watchdog/Watchdog.c src/measurements.c -lprintf_flt -lm -Wl,-u,vfprintf -g
	avr-objcopy -O ihex PROGRAM.elf PROGRAM.hex
	