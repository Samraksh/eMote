@echo off

REM 
REM This script takes lib path and a pattern name. Searches for that pattern and extracts it to said directory name supplied as 3rd param
REM optional fourth parameter will make it exclude the search pattern.
REM

set PREVCD=%CD%

PrepareLibsPatternForConcat.cmd %1 "RoT Inter" Kernel 1