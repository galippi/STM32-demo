#
# ST-LINK GDB SERVER initialization
#
# This connects to a GDB Server listening
# for commands on localhost at tcp port 61234
# ST-LINK GDB SERVER port
#target remote localhost:61234
# openOCD port
target remote localhost:3333

#load ST_MB525_RAM.elf
# Initializing PC and stack pointer
# RAM_START_ADDR is at 0x20000000
load
set $pc = (0x20000050)
