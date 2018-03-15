target remote localhost:3333
monitor reset
monitor halt
load bin/demo1.elf
disconnect
target remote localhost:3333
monitor reset halt
