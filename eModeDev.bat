@ECHO off

TITLE Emote Dev Command Prompt

ECHO Welcome to Samraksh Emote Platform
set repo_path=%cd%
echo Repo path is %repo_path%
cd MicroFrameworkPK_v4_3 

setenv_gcc.cmd 5.4.1 %repo_path%\..\TestSystem\Compilers\GCC5.4.1\
  
cd Solutions\EmoteDotNow\ 


 
