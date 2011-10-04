@echo off

REM # provide the gcc open plug's compiler tool dir, as the original tool from OP's  just dump in a arm-elf
REM # have to rearrange the tools a bit
REM # arm-elf\bin for all the .exe
REM # arm-elf\include for all the .h
REM # arm-elf\lib for all the libs ( .a)

setenv_base.cmd GCCOP4.2 PORT %*