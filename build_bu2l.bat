@echo off
path=C:\KBApps\DevEnv\Cygwin\V2_3_1\bin;%PATH%
path=C:\KBApps\gcc-arm-none-eabi-5_4-2016q3-20160926-win32\bin;%PATH%
set WORK_ROOT=%CD%

bash -i -c "make -r -C $WORK_ROOT %*"
