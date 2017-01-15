@set OCD_PATH=C:\Programok\openocd-0.9.0
::start %OCD_PATH%\bin-x64\openocd.exe -f scripts\board\st_nucleo_f103rb.cfg
::%OCD_PATH%\bin-x64\openocd.exe -f %OCD_PATH%\scripts\board\st_nucleo_f103rb.cfg -c "reset init program ./bin/demo1.elf verify reset exit"
%OCD_PATH%\bin-x64\openocd.exe -f %OCD_PATH%\scripts\board\st_nucleo_f103rb.cfg -c "reset init" -c "flash list" -c exit