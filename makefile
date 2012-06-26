all: blinker02.gcc.thumb.bin

CFLAGS_DEBUG = -gdwarf-2 -I./ST_lib
CFLAGS_OPTIM = -O1

ARMGNU := arm-none-eabi
CFLAGS :=

CFLAGS += $(CFLAGS_DEBUG) $(CFLAGS_OPTIM)

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker02.gcc.thumb.o : blinker02.c
	$(ARMGNU)-gcc $(CFLAGS) -mthumb -c blinker02.c -o blinker02.gcc.thumb.o

blinker02.gcc.thumb2.o : blinker02.c
	$(ARMGNU)-gcc $(CFLAGS) -mthumb -mcpu=cortex-m0 -march=armv7-m -c blinker02.c -o blinker02.gcc.thumb2.o

blinker02.gcc.thumb.bin : memmap vectors.o blinker02.gcc.thumb.o
	$(ARMGNU)-ld -o blinker02.gcc.thumb.elf -T memmap vectors.o blinker02.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker02.gcc.thumb.elf > blinker02.gcc.thumb.list
	$(ARMGNU)-objcopy blinker02.gcc.thumb.elf blinker02.gcc.thumb.bin -O binary

blinker02.gcc.thumb2.bin : memmap vectors.o blinker02.gcc.thumb2.o
	$(ARMGNU)-ld -o blinker02.gcc.thumb2.elf -T memmap vectors.o blinker02.gcc.thumb2.o
	$(ARMGNU)-objdump -D blinker02.gcc.thumb2.elf > blinker02.gcc.thumb2.list
	$(ARMGNU)-objcopy blinker02.gcc.thumb2.elf blinker02.gcc.thumb2.bin -O binary

clean:
	rm -f vectors.o blinker02.gcc.thumb.o blinker02.gcc.thumb2.o blinker02.gcc.thumb.bin blinker02.gcc.thumb2.bin
	rm -f blinker02.gcc.thumb.elf blinker02.gcc.thumb.list
