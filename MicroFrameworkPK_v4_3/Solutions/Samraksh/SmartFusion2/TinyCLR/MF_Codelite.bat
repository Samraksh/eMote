@echo off
@echo Executing Pre Build commands ...
@setenv_gcc.cmd 5.4.1 D:\Main\Repos\TestSystem\Compilers\GCC5.4.1\
@echo Done
msbuild /t:build TinyClr.proj
