@set HOME=%CD%
::start "%CD% gdb" C:\Programok\gcc-arm-none-eabi-5_4-2016q3-20160926-win32\bin\arm-none-eabi-gdb.exe -s=./bin/demo1.elf -e=./bin/demo1.s19 -x=.targetinit
start "%CD% gdb" C:\KBApps\gcc-arm-none-eabi-5_4-2016q3-20160926-win32\bin\arm-none-eabi-gdb.exe -se=./bin/demo1.elf -x=.targetinit
