#Mukundan Sridharan (mukundan.sridharan@samraksh.com)
#Copyright: The Samraksh Company
#Function: Erase Bank 1 of flash and Program the app at location specified in ConvertBinToS19 script


# SamTest absolute root
$debug = $true
$BaseDir = "G:\Main\MF-Dev\";
$MFVer= "MF4.3beta\MicroFrameworkPK_v4_3\";
$MFDir = $BaseDir + $MFVer;
$ScriptsRoot = $BaseDir + $MFVer + "Samraksh\PSScripts";

# Working sets
#$axf_path = "D:\\Main\\Emote-dev\\Dev\\MicroFrameworkPK_v4_0\\BuildOutput\\THUMB2\\GCC4.2\\le\\FLASH\\debug\\STM32F10x\\bin\\"
#$axf_file = "TinyCLR.axf"
#$axf_full = $axf_path + $axf_file

$app_path = $MFDir + "\BuildOutput\public\Debug\Client\dat"
#$app_file =  "CSharpTestSuite.dat"
$app_file =  "eMotePing.dat"
$app_full = $app_path + $app_file

# setup environment
clear
$env:Path = "$samtest_path;$openocd_path_bin;$gdb_path_bin"
cd $ScriptsRoot

try {
	. .\SamTest.ps1
	#. .\gdb.ps1
	#. .\msbuild.ps1
	. .\openocd.ps1
	#. .\logic.ps1
} catch {
	Write-Error "Could not load powershell scripts."
    break
}

# Add custom exception types
Add-CustomExceptions

# START SCRIPT #
Write-Sam "Running Script..."

# Ensure a clean runspace
Kill-All

# Start OpenOCD
Start-OpenOCD
#sleep(5)

#Convert App to S19 format and to the right address
chdir $samtest_path
$appExe = "$app_path\\$app_file"
$app_full = $appExe + ".S19"
$cmd = "ConvertBinToS19.ps1 $appExe $app_full" 
Write-Sam "Converting app to S19 format\n\n $cmd"
Invoke-Expression $cmd

# Load APP directly through OpenOCD (STM32), since no debugging is possible
# Write-Putty "soft_reset_halt"
# Write-Putty "soft_reset_halt"
# Write-Putty "stm32x mass_erase 1"
 Load-OpenOCD $app_full

# Load APP directly through OpenOCD (STM32), since no debugging is possible
#Load-RAM-OpenOCD $app_full


sleep(10)
Kill-Putty

# kill processes
#Kill-All

# END SCRIPT #