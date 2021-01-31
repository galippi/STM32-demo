@set OCD_PATH=C:\Programok\openocd-0.9.0
::start %OCD_PATH%\bin-x64\openocd.exe -f scripts\board\st_nucleo_f103rb.cfg
::start %OCD_PATH%\bin-x64\openocd.exe -f %OCD_PATH%\scripts\board\st_nucleo_f103rb.cfg
::start %OCD_PATH%\bin-x64\openocd.exe -f %OCD_PATH%\scripts\board\st_nucleo_f103c8.cfg
%OCD_PATH%\bin-x64\openocd.exe -f %CD%\OpenOCD\st_nucleo_f103c8.cfg -c "program ./bin/demo1.s19" -c "reset run" -c "exit"
::%OCD_PATH%\bin-x64\openocd.exe -f %CD%\OpenOCD\st_nucleo_f103c8.cfg
