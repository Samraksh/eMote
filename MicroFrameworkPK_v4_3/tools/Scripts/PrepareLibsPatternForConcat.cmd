@echo on

REM 
REM This script takes lib path and a pattern name. Searches for that pattern and extracts it to said directory name supplied as 3rd param
REM optional fourth parameter will make it exclude the search pattern.
REM

set PREVCD=%CD%

IF NOT EXIST %1\%3 mkdir %1\%3
cd %1\%3

IF "%4"=="1" (
	:: FOR /F %%a in ('dir /B "%%1\*.lib" | findstr /V /I "%%2"') do SET OUTPUT=%%a
	echo "Excluding %%2"
	dir /B %1\*.lib | findstr /V /I %2 > temp.txt
) ELSE (
	:: FOR /F %%a in ('dir /B "%%1\*.lib" | findstr /I "%%2"') do SET OUTPUT=%%a
	echo "Matching %%2"
	dir /B %1\*.lib | findstr /I %2 > temp.txt
)
:: set /p OUTPUT=<temp.txt


:: for %%i IN (%OUTPUT%) do (
for /f "tokens=*" %%i IN (temp.txt) do (
	:: echo "File %%i"
	IF EXIST "%GNU_TOOLS_BIN%\%GNU_TARGET%-ar.exe" (
        @echo %GNU_TOOLS_BIN%\%GNU_TARGET%-ar -x ..\%%i
        %GNU_TOOLS_BIN%\%GNU_TARGET%-ar -x ..\%%i
	)
) 

:DONE

:: del temp.txt

cd %PREVCD%
::set PREVCD=