@echo off

REM 
REM This script is used to extract a list of libraries under a .\tmp directory
REM

set PREVCD=%CD%
	
goto COMMENT

cd %1
	
for %%i IN (%*) do (
  IF "%%i"=="%1" (
	@echo 
  ) ELSE (
	copy %%i ..\staticLib\
  )
)

REM GOTO :DONE

:COMMENT
	
for %%i IN (%*) do (
  IF "%%i"=="%1" (
	IF NOT EXIST %1\tmp mkdir %1\tmp
	cd %1\tmp
  )
)

for %%i IN (../*.lib) do (
      IF EXIST "%GNU_TOOLS_BIN%\%GNU_TARGET%-ar.exe" (
		@echo %GNU_TOOLS_BIN%\%GNU_TARGET%-ar -x %1\%%i
        %GNU_TOOLS_BIN%\%GNU_TARGET%-ar -x %1\%%i
      ) ELSE (
          @echo ERROR: Cannot find GCC archiver tool!
          GOTO :DONE
        )
    )


:DONE

cd %PREVCD%
set PREVCD=