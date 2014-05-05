@echo off

REM 
REM This script is used to extract a list of libraries under a .\tmp directory
REM

set PREVCD=%CD%


for %%i IN (%*) do (
  IF "%%i"=="%1" (
	IF NOT EXIST %1\tmp mkdir %1\tmp
	cd %1\tmp
  ) 
)

for %%i IN (%1\*.lib) do (
	  IF EXIST "%GNU_TOOLS_BIN%\%GNU_TARGET%-ar.exe" (
		@echo %GNU_TOOLS_BIN%\%GNU_TARGET%-ar -x %%i
        %GNU_TOOLS_BIN%\%GNU_TARGET%-ar -x %%i
      ) ELSE (
          @echo ERROR: Cannot find GCC archiver tool!
          GOTO :DONE
        )
    )


:DONE

cd %PREVCD%
set PREVCD=