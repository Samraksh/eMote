

cd D:\Main\Repos\eMote\MicroFrameworkPK_v4_3\BuildOutput\THUMB2\GCC5.4\le\FLASH\debug\SmartFusion2\lib\RoT2

foreach($i in (ls *.obj)) {
    Write-Host $i; 
    D:\Main\Repos\TestSystem\Compilers\GCC5.4.1\bin\arm-none-eabi-objdump.exe -h  $i|Select-String rodata
}