@cls
@set START=start "%CD%"
@set PATH=%PATH%;D:\Gabor\Java\IotTerminal\IotTerminal\lib\RXTXcomm
@set JAVA=C:\Program Files (x86)\Java\jdk1.7.0_11
::@java -jar D:\Gabor\Java\IotTerminal\IotTerminal\bin\iotterminal.jar
%START% "%JAVA%\bin\java" -jar D:\Gabor\Java\IotTerminal\IotTerminal\bin\iotterminal.jar
