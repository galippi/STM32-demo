@set OCD_PATH=C:\KBApps\openocd-0.9.0
@set START=start
::start %OCD_PATH%\bin-x64\openocd.exe -f scripts\board\st_nucleo_f103rb.cfg
::start %OCD_PATH%\bin-x64\openocd.exe -f %OCD_PATH%\scripts\board\st_nucleo_f103rb.cfg
::start %OCD_PATH%\bin-x64\openocd.exe -f %OCD_PATH%\scripts\board\st_nucleo_f103c8.cfg
::start %OCD_PATH%\bin-x64\openocd.exe -f %CD%\OpenOCD\st_nucleo_f103c8.cfg
%START% %OCD_PATH%\bin-x64\openocd.exe -f %CD%\OpenOCD\st_nucleo_f103c8.cfg
