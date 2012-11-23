# SamTest absolute root
$debug = $true
$drive = "D:\Main\Emote-dev\Dev"
$samtest_path = "$drive\MicroFrameworkPK_v4_0\PSScripts"
$powershell_path = $samtest_path

# Working sets
$axf_path = "D:\\Main\\Emote-dev\\Dev\\MicroFrameworkPK_v4_0\\BuildOutput\\THUMB2\\GCC4.2\\le\\FLASH\\debug\\STM32F10x\\bin\\"
$axf_file = "TinyCLR.axf"
$axf_full = $axf_path + $axf_file

$app_path = "D:\\Main\\Emote-dev\\Dev\\MicroFrameworkPK_v4_0\\BuildOutput\\public\\Debug\\Client\\dat"
$app_file =  "CSharpTestSuite.dat"
#$app_full = $app_path + $app_file

$framework_root = "$drive\MicroFrameworkPK_v4_0"
$project = "nativesample.proj"

# setup environment
clear
$env:Path = "$samtest_path;$openocd_path_bin;$gdb_path_bin"
cd $samtest_path

try {
	. .\SamTest.ps1
	. .\gdb.ps1
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

# Start MSBuild
#Start-MSBuild
# Clean project
#SetEnv-MSBuild
#Clean-MSBuild
#Build-MSBuild
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

# Load APP directly through OpenOCD, since no debugging is possible
Load-OpenOCD $app_full
sleep(10)
Kill-Putty

# Start GDB
Start-GDB
#sleep(5)
# Connect
Connect-GDB
#sleep(5)
# Reconnect to ensure that execution is stopped
#Connect-GDB
sleep(5)

# Load CLR
Load-GDB $axf_full


# Set BP at ApplicationEntryPoint
$BP_ApplicationEntryPoint = Insert-BP-GDB "ApplicationEntryPoint"
#$BP_Timer = Insert-BP-GDB "Timer_Driver::Initialize"
# Run until BP
#Continue-GDB
# Wait until we confirm that we have reached BP
#$async = Wait-AsyncRecordKey-GDB "*stopped"
#sleep(2)
#Write-Sam "test time"
# TEST


#Continue-GDB
#$face.Start()
#sleep(10)
#$face.Stop()

# kill processes
#Kill-All

# END SCRIPT #