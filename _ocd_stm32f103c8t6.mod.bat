::@set OCD_PATH=C:\Programok\openocd-0.9.0
@set OCD_PATH=C:\KBApps\openocd-0.10.0
::start %OCD_PATH%\bin-x64\openocd.exe -f scripts\board\st_nucleo_f103rb.cfg
::start %OCD_PATH%\bin-x64\openocd.exe -f %OCD_PATH%\scripts\board\st_nucleo_f103rb.cfg
::start %OCD_PATH%\bin\openocd.exe -f %OCD_PATH%\scripts\board\st_nucleo_f103c8.cfg
::%OCD_PATH%\bin-x64\openocd.exe -f %CD%\OpenOCD\st_nucleo_f103c8.cfg
::@set OCD_SCRIPTS=%OCD_PATH%\share\openocd\scripts
@set OCD_SCRIPTS=%OCD_PATH%\scripts
start %OCD_PATH%\bin\openocd.exe -f OpenOCD\st_nucleo_f103c8.cfg
