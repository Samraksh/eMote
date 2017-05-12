@echo off

dir /b /s %1\bin\Release\le\*.pe > tempPE.tmp

C:\SamGit\MF\MicroFrameworkPK_v4_3\Framework\IDE\Targets\v4.2\Tools\MetaDataProcessor.exe -create_database tempPE.tmp output.dat
del tempPE.txt

echo Done