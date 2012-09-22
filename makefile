##################################################################
CFLAGS_DEBUG = -gdwarf-2 -I./ST_lib

WARNINGS = -Wall -Wextra
WARNINGS += -Wwrite-strings -Wcast-qual -Wpointer-arith -Wsign-compare
WARNINGS += -Wundef
WARNINGS += -Wmissing-declarations

CFLAGS_OPTIM = -O1
#CFLAGS_OPTIM = -O3

ARMGNU := arm-none-eabi-

LDFLAGS = -L./lib -T memmap

AS = $(ARMGNU)as
CC = $(ARMGNU)gcc
CC_DEP = gcc
LL = $(ARMGNU)ld
OBJCOPY = $(ARMGNU)objcopy
OBJDUMP = $(ARMGNU)objdump

CFLAGS_TARGET := -mthumb
#CFLAGS += -mcpu=cortex-m0
#CFLAGS += -march=armv7-m

#LDFLAGS_STRIP_DEBUG_INFO = -s
LDFLAGS  += $(LDFLAGS_STRIP_DEBUG_INFO)

LDLIBS :=
LDLIBS += -lgcc

##################################################################
TARGET=blinker02.gcc.thumb
TARGET_DIR=bin
DUMMY_DIR_FILE = $(TARGET_DIR)/dummy

CPPFILES =
#CPPFILES+=

CFILES   = 
CFILES  += blinker02.c
CFILES  += adc.c
CFILES  += adc_app.c
CFILES  += dac.c
CFILES  += gpio.c
CFILES  += timer.c
CFILES  += util.c
#CFILES  += 

SFILES  =
#SFILES += vectors.s

TARGET_ELF = $(TARGET_DIR)/$(TARGET).elf
TARGET_BIN = $(TARGET_DIR)/$(TARGET).bin
TARGET_LIST = $(TARGET_DIR)/$(TARGET).list

all : $(TARGET_BIN) $(TARGET_LIST)

OBJECTS = $(addprefix $(TARGET_DIR)/,$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o))
DEPFILES = $(addprefix $(TARGET_DIR)/,$(CPPFILES:.cpp=.d) $(CFILES:.c=.d))

CFLAGS = $(CFLAGS_TARGET) $(WARNINGS) $(CFLAGS_DEBUG) $(CFLAGS_OPTIM)
CFLAGS_DEP = $(WARNINGS) $(CFLAGS_DEBUG) $(CFLAGS_OPTIM)

$(TARGET_ELF) : $(OBJECTS)
	$(LL) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(TARGET_BIN) : $(TARGET_ELF)
	$(OBJCOPY) $^ $@ -O binary

$(TARGET_LIST) : $(TARGET_ELF)
	$(OBJDUMP) -D $^ > $@

  #%.o: %.s $(MAKEFILE)
$(TARGET_DIR)/%.o: %.s $(DUMMY_DIR_FILE)
	@echo Building $@
	$(AS) -as $< -o $@

#%.o: %.c $(MAKEFILE)
$(TARGET_DIR)/%.o: %.c $(DUMMY_DIR_FILE)
	@echo Building $(notdir $@)
	-@rm -f $(@:.o=.d)
	$(CC_DEP) -M $(CFLAGS_DEP) -c -o $(@:.o=.d) $<
	$(CC) $(CFLAGS) -c -o $@ $<

#	$(CC) -v -x c -E -

#%.o: %.cpp $(MAKEFILE)
$(TARGET_DIR)/%.o: %.cpp $(DUMMY_DIR_FILE)
	@echo Building $@
	-@rm -f $(@:.o=.d)
	$(CC) -M $(CPPFLAGS) -c -o $(TARGET_DIR)$(@:.o=.d) $<
	$(CC) $(CPPFLAGS) -c -o $(TARGET_DIR)$@ $<
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
#$(TARGET_ELF): version.h

include $(wildcard $(DEPFILES))
