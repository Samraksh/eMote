# SamTest absolute root
$debug = $true
$drive = "D:\Main\Emote-dev\Dev"
$samtest_path = "$drive\MicroFrameworkPK_v4_0\PSScripts"
$powershell_path = $samtest_path

# Working sets
$axf_path = "D:\\Main\\Emote-dev\\Dev\\MicroFrameworkPK_v4_0\\BuildOutput\\THUMB2\\GCC4.2\\le\\FLASH\\debug\\STM32F10x\\bin\\"
$axf_file = "TinyCLR.axf"
$axf_full = $axf_path + $axf_file
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
sleep(5)
# Start GDB
Start-GDB
#sleep(5)
# Connect
Connect-GDB
#sleep(5)
# Reconnect to ensure that execution is stopped
Connect-GDB
sleep(5)
# Load
Load-GDB
# Set BP at ApplicationEntryPoint
$BP_ApplicationEntryPoint = Insert-BP-GDB "ApplicationEntryPoint"
#$BP_Timer = Insert-BP-GDB "Timer_Driver::Initialize"
# Run until BP
Continue-GDB
# Wait until we confirm that we have reached BP
$async = Wait-AsyncRecordKey-GDB "*stopped"
sleep(2)
Write-Sam "test time"
# TEST
#$compiler = "$env:windir/Microsoft.NET/Framework/v2.0.50727/csc"
#&$compiler /target:library /lib:C:\SamTest\powershell /out:Tester.dll /r:SaleaeDeviceSdkDotNet.dll *.cs
$saleae = [Reflection.Assembly]::LoadFrom("C:\SamTest\powershell\SaleaeDeviceSdkDotNet.dll")
$tester = [Reflection.Assembly]::LoadFrom("C:\SamTest\powershell\Tester.dll")
$face = new-object Tester.TestInstance
Register-ObjectEvent $face Connected -SourceIdentifier TestInstance.Connected | Out-DBG
Register-ObjectEvent $face Parsed -SourceIdentifier TestInstance.Parsed | Out-DBG
Register-ObjectEvent $face Stopped -SourceIdentifier TestInstance.Stopped | Out-DBG
Write-Sam "Parsing..."
$face.Parse("")
Wait-Event TestInstance.Parsed
Write-Sam "Testing..."
$face.Connect()
Wait-Event TestInstance.Connected
Continue-GDB
$face.Start()
sleep(10)
$face.Stop()

# kill processes
#Kill-All

# END SCRIPT #