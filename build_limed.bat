@echo off
@cls
::path=C:\Programok\cygwin64\bin;%PATH%
path=C:\Programok\cygwin\bin;%PATH%
path=C:\Programok\gcc-arm-none-eabi-5_4-2016q3-20160926-win32\bin;%PATH%
set WORK_ROOT=%CD%

@call _git_config.bat

bash -i -c "make -r -C $WORK_ROOT %*"
