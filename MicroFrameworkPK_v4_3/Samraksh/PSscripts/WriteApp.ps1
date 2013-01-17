#Mukundan Sridharan (mukundan.sridharan@samraksh.com)
#Copyright: The Samraksh Company
#Function: Erase Bank 1 of flash and Program the app at location specified in ConvertBinToS19 script



#Clear-variable putty*
$putty_path="C:\Program Files (x86)\Putty"
$putty_exe="plink.exe"
#        output_id - OutputDataReceived event SourceIdentifer
$putty_output_id = "Putty.OutputDataReceived"
#        error_id - ErrorDataReceived event SourceIdentifer
$putty_error_id = "Putty.ErrorDataReceived"
#        info_id - InfoReceived event SourceIdentifer
$putty_info_id = "Putty.InfoReceived"
#$putty_resultrecord_id = "Putty.ResultRecordReceived"


# Start-Putty
#   starts an instance of Putty to connect to OpenOCD
Function Start-Putty {
    echo "Starting Putty..."
    #chdir $openocd_path_bin;
    Clear-Variable process
    $process = New-Object System.Diagnostics.Process
    $process.StartInfo.Filename = "$putty_path\$putty_exe"
    $process.StartInfo.Arguments = "telnet://localhost:4444"
    $process.StartInfo.UseShellExecute = $FALSE
    $process.StartInfo.WindowStyle = "Hidden"
    # streams
    $process.StartInfo.RedirectStandardInput = $TRUE
    $process.StartInfo.RedirectStandardOutput = $TRUE
    $process.StartInfo.RedirectStandardError = $TRUE
    # start
    $process.Start() | Out-DBG "Putty.Process.Start()"
    # StandardInput stream
    $input = $process.StandardInput
    # StandardOutput stream
    #Register-ObjectEvent $process OutputDataReceived $gdb_output_id { $EventArgs.Data | Out-DBG "GDB.StandardOutput"; if($EventArgs.Data -like "[\^]*") { New-Event $gdb_resultrecord_id -MessageData $EventArgs.Data | Out-DBG "GDB.RegisterEngineEvent.ResultRecordReceived"; } elseif($data -like "[\*\+=]*") { New-Event $gdb_asyncrecord_id -MessageData $EventArgs.Data | Out-DBG "GDB.NewEvent.AsyncRecordReceived"; } } | Out-DBG "GDB.RegisterObjectEvent.StandardOutput"
    Register-ObjectEvent $process OutputDataReceived $putty_output_id { $EventArgs.Data | Out-DBG "Putty.StandardOutput"; if($EventArgs.Data -like ">*") { New-Event $putty_info_id -MessageData $EventArgs.Data | Out-DBG "Putty.NewEvent.InfoReceived"; }  } | Out-DBG "Putty.RegisterObjectEvent.StandardOutput"
    $process.BeginOutputReadLine()
    # StandardError stream
    Register-ObjectEvent $process ErrorDataReceived $putty_error_id { $EventArgs.Data | Out-DBG "Putty.StandardError"; } | Out-DBG "Putty.RegisterObjectEvent.StandardError"
    $process.BeginErrorReadLine()
    # Export handles up one level
    Set-Variable putty_process $process -Scope Global
    Set-Variable putty_input $input -Scope Global
    #Wait-Info-Putty
    echo "Putty was started."
}



Function Wait-Info-Putty {
    param($timeout=-1)
	$resultevent = Wait-Event $putty_info_id -Timeout $timeout;
	Remove-Event $putty_info_id;
}

Function Kill-Putty {
    echo "Killing Putty..."
    Unregister-Event $putty_output_id
    Unregister-Event $putty_error_id
    $putty_process.Kill()
    echo "Putty was killed."
}

Function Write-Putty {
    param($line)
    $putty_input.WriteLine($line)
    Wait-Info-Putty;
}

#Load file to flash
Function Load-OpenOCD {
     param ($file)
    # unlock flash
    #echo "OpenOCD:: soft_reset_halt"
    Write-Putty "soft_reset_halt" #Emote
    Write-Putty "soft_reset_halt" #Emote
    # load
    echo "Loading file $file..."
    Write-Putty "flash write_image $file"
}

##########################       Main Starts here  ##################

$debug = $true
$BaseDir = "G:\\Main\\MF-Dev\\";
$MFVer= "MF4.3beta\\MicroFrameworkPK_v4_3\\";
$MFDir = $BaseDir + $MFVer;
$ScriptsRoot = $BaseDir + $MFVer + "Samraksh\PSScripts";

# Working sets
#$axf_path = "D:\\Main\\Emote-dev\\Dev\\MicroFrameworkPK_v4_0\\BuildOutput\\THUMB2\\GCC4.2\\le\\FLASH\\debug\\STM32F10x\\bin\\"
#$axf_file = "TinyCLR.axf"
#$axf_full = $axf_path + $axf_file

$app_path = $MFDir + "BuildOutput\\public\\Debug\\Client\\dat"
#$app_file =  "CSharpTestSuite.dat"
$app_file =  "eMotePing.dat"
#$app_file =  "RealTimeExample.dat"
$app_full = $app_path + $app_file



#Convert App to S19 format and to the right address
chdir $ScriptsRoot 
$appExe = "$app_path\\$app_file"
$app_full = $appExe + ".S19"
$cmd = ".\ConvertBinToS19.ps1 $appExe $app_full" 
echo "Converting app to S19 format\n\n $cmd"
Invoke-Expression $cmd

Start-Putty

# Load APP directly through OpenOCD (STM32), since no debugging is possible

 Load-OpenOCD $app_full

# Load APP directly through OpenOCD (STM32), since no debugging is possible
#Load-RAM-OpenOCD $app_full


sleep(10)
Kill-Putty
