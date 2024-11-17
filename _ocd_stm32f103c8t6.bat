@cls
@set OCD_PATH=C:\Programok\OpenOCD-0.12.0-3
@set START=start
::start %OCD_PATH%\bin-x64\openocd.exe -f scripts\board\st_nucleo_f103rb.cfg
::start %OCD_PATH%\bin-x64\openocd.exe -f %OCD_PATH%\scripts\board\st_nucleo_f103rb.cfg
::start %OCD_PATH%\bin-x64\openocd.exe -f %OCD_PATH%\scripts\board\st_nucleo_f103c8.cfg
%START% %OCD_PATH%\bin\openocd.exe -f %CD%\OpenOCD\st_nucleo_g0.cfg
::%OCD_PATH%\bin-x64\openocd.exe -f %CD%\OpenOCD\st_nucleo_f103c8.cfg
