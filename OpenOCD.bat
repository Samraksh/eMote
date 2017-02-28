@ECHO off
TITLE Emote Dev Command Prompt
ECHO Welcome to Samraksh Emote Platform



CMD /K " cd C:\Microsemi\SoftConsole_v4.0\openocd\bin\ && openocd --command "set DEVICE M2S090" --file board/microsemi-cortex-m3.cfg"
