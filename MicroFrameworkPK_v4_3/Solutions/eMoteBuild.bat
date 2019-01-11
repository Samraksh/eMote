@ECHO off

TITLE Emote Dev Command Prompt

ECHO Welcome to Samraksh Emote Platform
ECHO Usage: eMoteBuild.bat Solution Project (build or clean)
ECHO Example: eMoteBuild.bat EmoteDotNow TinyCLR build

set cur_path=%cd%
set repo_path=%cur_path%\..\..
echo Repo path is %repo_path%

set env_cmd=%cur_path%\..\setenv_gcc.cmd
echo Envscript cmd is %env_cmd%

:: CMD /K "%env_cmd% 5.4.1 %repo_path%\..\TestSystem\Compilers\GCC5.4.1\ && cd %1 && cd %2 && msbuild %2.proj /t:%3 /maxcpucount && echo All done && cd ..\.. "

:: CMD /K "%env_cmd% 5.4.1 %repo_path%\..\TestSystem\Compilers\GCC5.4.1\ && cd %1 && cd %2 && msbuild %2.proj /t:%3 && echo All done && cd ..\.. "

CMD /K "%env_cmd% 6.3.1 %repo_path%\..\TestSystem\Compilers\GCC6.3.1\ && cd %1 && cd %2 && msbuild %2.proj /t:%3 && echo All done && cd ..\.. "

 
