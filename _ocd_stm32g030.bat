@cls
@set OCD_PATH=C:\Programok\OpenOCD-0.12.0-3
::@set OCD_PATH=C:\Programok\openocd-0.9.0
@set START=start
::start %OCD_PATH%\bin-x64\openocd.exe -f scripts\board\st_nucleo_f103rb.cfg
::start %OCD_PATH%\bin-x64\openocd.exe -f %OCD_PATH%\scripts\board\st_nucleo_f103rb.cfg
::start %OCD_PATH%\bin-x64\openocd.exe -f %OCD_PATH%\scripts\board\st_nucleo_f103c8.cfg
%START% %OCD_PATH%\bin\openocd.exe -f %CD%\OpenOCD\st_nucleo_g0.cfg
::%START% %OCD_PATH%\bin-x64\openocd.exe -f %CD%\OpenOCD\st_nucleo_g0.cfg
::%START% %OCD_PATH%\bin\openocd.exe -f %CD%\OpenOCD\st_nucleo_g0.cfg -d3
::%START% %OCD_PATH%\bin\openocd.exe -f %CD%\OpenOCD\st_nucleo_g0.cfg  -c "init; reset halt"
::%START% %OCD_PATH%\bin-x64\openocd.exe -f %CD%\OpenOCD\st_nucleo_f103c8.cfg

@if "%START%"=="" goto wait_to_user

sleep 2s
::start C:\Programok\putty.exe -load openocd
call _putty_OpenOCD_telnet_limed.bat

goto end

:wait_to_user
pause
goto end

:end
