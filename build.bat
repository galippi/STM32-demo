@echo off
rem echo 1=%1 2=%2
rem path=C:\Programs\cygwin\bin;%PATH%
path=C:\DEVENV\CYGWIN\V1_7_9\bin;C:\Programs\Sourcery G++ Lite\bin;%PATH%
rem set TMPDIR=c:\tmp
set WORK_ROOT=%CD%

::bash -i -c "make -d -r -C $WORK_ROOT %1 %2 %3 %4"
bash -i -c "make -r -C $WORK_ROOT %1 %2 %3 %4"
