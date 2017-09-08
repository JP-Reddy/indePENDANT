@echo off

rem save current path
set pwd=%cd%

rem echo %pwd%

C:
set CYGWIN_PATH=C:\CSDTK\cygwin
chdir "%CYGWIN_PATH%\bin"

mount -bfu "%CYGWIN_PATH%\bin" "/usr/bin"
mount -bfu "%CYGWIN_PATH%\lib" "/usr/lib"
mount -bfu "%CYGWIN_PATH%" "/"

set build=%pwd%\evt.sh

rem echo %build%

rem pause

bash --login -i %build%

pause