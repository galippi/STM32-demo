##################################################################
CFLAGS_DEBUG = -gdwarf-2 -I./ST_lib

WARNINGS = -Wall -Wextra
WARNINGS += -Wwrite-strings -Wcast-qual -Wpointer-arith -Wsign-compare
WARNINGS += -Wundef
WARNINGS += -Wmissing-declarations

CFLAGS_OPTIM = -O1
#CFLAGS_OPTIM = -O3

ARMGNU := arm-none-eabi

CC = $(ARMGNU)-gcc
LL = $(ARMGNU)-ld

CFLAGS := -mthumb
#CFLAGS += -mcpu=cortex-m0
#CFLAGS += -march=armv7-m

#LDFLAGS_STRIP_DEBUG_INFO = -s
LDFLAGS  = $(LDFLAGS_STRIP_DEBUG_INFO)

LDLIBS :=
LDLIBS += -lgcc

TARGET_ELF = $(TARGET_DIR)$(TARGET).elf
TARGET_ELF = $(TARGET_DIR)$(TARGET).bin
all : $(TARGET_BIN)

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

$(TARGET_ELF) : $(OBJECTS)
	$(LL) $(LDFLAGS) $(addprefix $(TARGET_DIR),$(OBJECTS)) $(LDLIBS) -o $@

#%.o: %.c $(MAKEFILE)
%.o: %.c $(DUMMY_DIR_FILE)
	@echo Building $@
	$(CC) $(CFLAGS) -c -o $(TARGET_DIR)$@ $<
	-@rm -f $(@:.o=.d)
	$(CC) -M $(CFLAGS) -c -o $(TARGET_DIR)$(@:.o=.d) $<

#%.o: %.cpp $(MAKEFILE)
%.o: %.cpp $(DUMMY_DIR_FILE)
	@echo Building $@
	$(CC) $(CPPFLAGS) -c -o $(TARGET_DIR)$@ $<
	-@rm -f $(@:.o=.d)
	$(CC) -M $(CPPFLAGS) -c -o $(TARGET_DIR)$(@:.o=.d) $<
#	$(CC) --version

$(DUMMY_DIR_FILE):
	-mkdir $(TARGET_DIR)
	echo Dummy file >$@

targetdir:
	-mkdir $(sort $(dir $(OBJECTS)))

##################################################################
# cleaning rule
##################################################################

clean:
	rm -f $(addprefix $(TARGET_DIR), *.o *.d *~ $(TARGET))
	rm -f $(OBJECTS)
	rm -f $(OBJECTS:.o=.d)
	rm -f version.h
	rm -r $(TARGET_DIR)

dep_test:
	@echo DEPFILES=$(DEPFILES)
	@echo wildcard=$(wildcard $(DEPFILES))

include version.mk
$(TARGET).o: version.h

include $(wildcard $(DEPFILES))
