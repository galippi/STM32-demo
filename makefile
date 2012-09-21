all: blinker02.gcc.thumb.bin

CFLAGS_DEBUG = -gdwarf-2 -I./ST_lib
CFLAGS_OPTIM = -O1
#CFLAGS_OPTIM = -O3

ARMGNU := arm-none-eabi
CFLAGS := -mthumb
#CFLAGS += -mcpu=cortex-m0
#CFLAGS += -march=armv7-m

CFLAGS += -Wall $(CFLAGS_DEBUG) $(CFLAGS_OPTIM)

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

gpio.o : gpio.c gpio.h
	$(ARMGNU)-gcc $(CFLAGS) -c gpio.c -o gpio.o

adc.o : adc.c adc.h
	$(ARMGNU)-gcc $(CFLAGS) -c adc.c -o adc.o

adc_app.o : adc_app.c adc_app.h adc.h util.h
	$(ARMGNU)-gcc $(CFLAGS) -c adc_app.c -o adc_app.o

util.o : util.c util.h
	$(ARMGNU)-gcc $(CFLAGS) -c util.c -o util.o

blinker02.gcc.thumb.o : blinker02.c gpio.h gpio_app.h adc_app.h adc.h
	$(ARMGNU)-gcc $(CFLAGS) -c blinker02.c -o blinker02.gcc.thumb.o

blinker02.gcc.thumb2.o : blinker02.c gpio.h
	$(ARMGNU)-gcc $(CFLAGS) -mthumb -mcpu=cortex-m0 -march=armv7-m -c blinker02.c -o blinker02.gcc.thumb2.o

blinker02.gcc.thumb.bin : memmap vectors.o blinker02.gcc.thumb.o gpio.o adc.o adc_app.o util.o
	$(ARMGNU)-ld -L./lib -o blinker02.gcc.thumb.elf -T memmap vectors.o blinker02.gcc.thumb.o gpio.o adc.o adc_app.o util.o -lgcc
	$(ARMGNU)-objdump -D blinker02.gcc.thumb.elf > blinker02.gcc.thumb.list
	$(ARMGNU)-objcopy blinker02.gcc.thumb.elf blinker02.gcc.thumb.bin -O binary

blinker02.gcc.thumb2.bin : memmap vectors.o blinker02.gcc.thumb2.o
	$(ARMGNU)-ld -o blinker02.gcc.thumb2.elf -T memmap vectors.o blinker02.gcc.thumb2.o gpio.o 
	$(ARMGNU)-objdump -D blinker02.gcc.thumb2.elf > blinker02.gcc.thumb2.list
	$(ARMGNU)-objcopy blinker02.gcc.thumb2.elf blinker02.gcc.thumb2.bin -O binary

clean:
	rm -f vectors.o blinker02.gcc.thumb.o blinker02.gcc.thumb2.o blinker02.gcc.thumb.bin blinker02.gcc.thumb2.bin
	rm -f gpio.o
	rm -f blinker02.gcc.thumb.elf blinker02.gcc.thumb.list
