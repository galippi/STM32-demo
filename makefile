##################################################################
CFLAGS_DEBUG = -gdwarf-2
SUBDIRS := . u32_to_hexstring hal_STM32C5 hal_STM32C5/ST_lib
SUBDIRS += hal_STM32C5/ST_lib/ll
SUBDIRS += queue
SUBDIRS_LINKER := hal_STM32C5/lib

WARNINGS = -Wall -Wextra
WARNINGS += -Wwrite-strings -Wcast-qual -Wpointer-arith -Wsign-compare
WARNINGS += -Wundef
WARNINGS += -Wmissing-declarations
WARNINGS += -Wmissing-prototypes -Wstrict-prototypes
WARNINGS += -Wno-unused-function
WARNINGS += -Wno-enum-compare
WARNINGS += -Wno-expansion-to-defined
# stop at warnings
WARNINGS += -Werror

CFLAGS_OPTIM = -O1
#CFLAGS_OPTIM = -O3

ARMGNU := arm-none-eabi-

MEMMAP_FILE = memmap
LDFLAGS = -L./lib -T $(MEMMAP_FILE) -Map $(TARGET_MAP)

AS = $(ARMGNU)as
CC = $(ARMGNU)gcc
LL = $(ARMGNU)ld
OBJCOPY = $(ARMGNU)objcopy
OBJDUMP = $(ARMGNU)objdump

CFLAGS_TARGET := -mthumb
#CFLAGS_TARGET += -mcpu=cortex-m0
#CFLAGS_TARGET += -mcpu=cortex-m0plus
#CFLAGS_TARGET += -mcpu=cortex-m3
#CFLAGS_TARGET += -mcpu=cortex-m4
#CFLAGS_TARGET += -march=armv7-m
CFLAGS_TARGET += -march=armv8-m.main -mfpu=fpv5-sp-d16 -mfloat-abi=hard
#CFLAGS_TARGET += --short-enums

#LDFLAGS_STRIP_DEBUG_INFO = -s
LDFLAGS  += $(LDFLAGS_STRIP_DEBUG_INFO) $(addprefix -L, $(SUBDIRS_LINKER))

LDLIBS :=
LDLIBS += -lc
#LDLIBS += -lgcc

##################################################################
TARGET=demo1
TARGET_DIR=bin
DUMMY_DIR_FILE = $(TARGET_DIR)/dummy

VPATH := $(TARGET_DIR) $(SUBDIRS)

CPPFILES =
#CPPFILES+=

CFILES   = reset.c
CFILES  += main.c
CFILES  += debug.c
CFILES  += util.c
CFILES  += FaultHandler.c
CFILES  += ram_init.c
CFILES  += vector.c
CFILES  += u32_to_hexstring.c
CFILES  += SysClock.c
CFILES  += queue.c
CFILES  += gpio.c
CFILES  += scheduler_preemptive.c
CFILES  += tasks.c
CFILES  += uart.c
CFILES  += uart_app.c

SFILES  =
#SFILES += vectors.s

TARGET_ELF = $(TARGET_DIR)/$(TARGET).elf
TARGET_S19 = $(TARGET_DIR)/$(TARGET).s19
TARGET_BIN = $(TARGET_DIR)/$(TARGET).bin
TARGET_LIST = $(TARGET_DIR)/$(TARGET).list
TARGET_MAP = $(TARGET_DIR)/$(TARGET).map

all : $(TARGET_S19) $(TARGET_BIN) $(TARGET_LIST)

OBJECTS = $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
DEPFILES = $(addprefix $(TARGET_DIR)/,$(CPPFILES:.cpp=.d) $(CFILES:.c=.d))
INCDIRS = $(addprefix -I./,$(SUBDIRS))

CFLAGS = $(CFLAGS_TARGET) $(WARNINGS) $(CFLAGS_DEBUG) $(INCDIRS) $(CFLAGS_OPTIM)
CFLAGS_DEP = $(WARNINGS) $(CFLAGS_DEBUG) $(INCDIRS) $(CFLAGS_OPTIM)

$(TARGET_ELF) : $(OBJECTS) $(MEMMAP_FILE)
	$(LL) $(LDFLAGS) -o $@ $(addprefix $(TARGET_DIR)/,$(OBJECTS)) $(LDLIBS)

$(TARGET_BIN) : $(TARGET_ELF)
	$(OBJCOPY) $^ $@ -O binary

$(TARGET_S19) : $(TARGET_ELF)
	$(OBJCOPY) $^ $@ -O srec
	#python memmap.py $(TARGET_DIR)/$(TARGET).map

$(TARGET_LIST) : $(TARGET_ELF)
	$(OBJDUMP) -D $^ > $@

  #%.o: %.s $(MAKEFILE)
%.o: %.s $(DUMMY_DIR_FILE)
	@echo Building $@
	$(AS) -as $< -o $(TARGET_DIR)/$@

#%.o: %.c $(MAKEFILE)
%.o: %.c $(DUMMY_DIR_FILE)
	@echo Building $(notdir $@)
	-@rm -f $(TARGET_DIR)/$(@:.o=.d)
	$(CC) $(CFLAGS) -c -o $(TARGET_DIR)/$@ -MMD -MF$(TARGET_DIR)/$(@:.o=.d) -MT"$@" $<


#	$(CC) $(CFLAGS) -c -o $(TARGET_DIR)/$@ -MMD -MF$(TARGET_DIR)/$(@:.o=.dtmp) $<
#	@cat $(TARGET_DIR)/$(@:.o=.dtmp)| sed 's|^bin/||' > $(TARGET_DIR)/$(@:.o=.d)
#	-@rm -f $(TARGET_DIR)/$(@:.o=.dtmp)

#	$(CC) -MD $(CFLAGS) -c -o $@ $<
#	$(CC) -v -x c -E -

#%.o: %.cpp $(MAKEFILE)
%.o: %.cpp $(DUMMY_DIR_FILE)
	@echo Building $@
	-@rm -f $(@:.o=.d)
	$(CC) -M $(CPPFLAGS) -c -o $(TARGET_DIR)/$(TARGET_DIR)$(@:.o=.d) $<
	$(CC) $(CPPFLAGS) -c -o $(TARGET_DIR)/$(TARGET_DIR)$@ $<
#	$(CC) --version

$(DUMMY_DIR_FILE):
	make tool_check
	-mkdir $(TARGET_DIR)
	echo Dummy file >$@

targetdir:
	-mkdir $(sort $(dir $(OBJECTS)))

tool_check_:
	@echo $@ is skipped

tool_check:
	$(AS) --version | grep "2.36.1.20210621"
	$(CC) --version | grep "10.3.1 20210824"
	$(LL) --version | grep "2.36.1.20210621"
	$(OBJCOPY) --version | grep "2.36.1.20210621"
	$(OBJDUMP) --version | grep "2.36.1.20210621"
	@echo $@ is ok!

tool_version_list:
	$(AS) --version
	$(CC) --version
	$(LL) --version
	$(OBJCOPY) --version
	$(OBJDUMP) --version

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
