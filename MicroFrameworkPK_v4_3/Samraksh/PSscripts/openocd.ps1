# openOCD configuration
#        exe - name of the executable
$openocd_exe = "openocd-x64-0.5.0-rc1"
#        interface_cfg - specify the interface config file
$openocd_interface_cfg = "olimex-arm-usb-tiny-h.cfg"
#$openocd_interface_cfg = "olimex-jtag-tiny.cfg"
#        board_cfg - specify the board config file
$openocd_board_cfg = "stm32xl.cfg"
#        path_root - root path
$openocd_path_root = "D:\Main\Emote-dev\Tools\openocd-x64-0.5.0-rc1"
#        path_bin - path to binaries
$openocd_path_bin = "$openocd_path_root\bin\"
#        path_tcl - path to tcl (tool command language) files, these are openocd's .cfg files
$openocd_path_tcl = "$openocd_path_root\tcl\"
#        output_id - OutputDataReceived event SourceIdentifer
$openocd_output_id = "OpenOCD.OutputDataReceived"
#        error_id - ErrorDataReceived event SourceIdentifer
$openocd_error_id = "OpenOCD.ErrorDataReceived"
#        info_id - InfoReceived event SourceIdentifer
$openocd_info_id = "OpenOCD.InfoReceived"

#Clear-variable putty*
$putty_path="C:\Program Files (x86)\WinSCP\PuTTY"
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
    Write-Sam "Starting Putty..."
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
    Write-Sam "Putty was started."
}


# Start-OpenOCD
#   starts an instance of OpenOCD
Function Start-OpenOCD {
    Write-Sam "Starting OpenOCD..."
    
    chdir $openocd_path_bin;
    $process = New-Object System.Diagnostics.Process
    $process.StartInfo.Filename = "$openocd_path_bin\$openocd_exe"
    $process.StartInfo.Arguments = "-f $openocd_path_bin\config\$openocd_interface_cfg -f $openocd_path_bin\config\$openocd_board_cfg"
    $process.StartInfo.UseShellExecute = $FALSE
    $process.StartInfo.WindowStyle = "Hidden"
    # streams
    $process.StartInfo.RedirectStandardInput = $TRUE
    $process.StartInfo.RedirectStandardOutput = $TRUE
    $process.StartInfo.RedirectStandardError = $TRUE
    # start
    $process.Start() | Out-DBG "OpenOCD.Process.Start()"
    # StandardInput stream
    $input = $process.StandardInput
    # StandardOutput stream
    Register-ObjectEvent $process OutputDataReceived $openocd_output_id { $EventArgs.Data | Out-DBG "OpenOCD.StandardOutput" } | Out-DBG "OpenOCD.RegisterObjectEvent.StandardOutput"
    $process.BeginOutputReadLine()
    # StandardError stream
    Register-ObjectEvent $process ErrorDataReceived $openocd_error_id { $EventArgs.Data | Out-DBG "OpenOCD.StandardError"; if($EventArgs.Data -like "Open On-Chip Debugger 0.5.0*") { New-Event $openocd_info_id -MessageData $EventArgs.Data | Out-DBG "OpenOCD.NewEvent.InfoReceived"; } } | Out-DBG "OpenOCD.RegisterObjectEvent.StandardError"
    $process.BeginErrorReadLine()
    # Export handles up one level
    Set-Variable openocd_process $process -Scope Global
    Set-Variable openocd_input $input -Scope Global
    Wait-Info-OpenOCD
    Write-Sam "OpenOCD was started."
    sleep 2
    Start-Putty
}




# Kill-OpenOCD
#   kills OpenOCD and unregisters associated events
Function Kill-OpenOCD {
    Write-Sam "Killing OpenOCD..."
    Unregister-Event $openocd_output_id
    Unregister-Event $openocd_error_id
    $openocd_process.Kill()
    Write-Sam "OpenOCD was killed."
}


Function Kill-Putty {
    Write-Sam "Killing Putty..."
    Unregister-Event $putty_output_id
    Unregister-Event $putty_error_id
    $putty_process.Kill()
    Write-Sam "Putty was killed."
}

Function Wait-Info-OpenOCD {
    param($timeout=-1)
	try {
		$resultevent = Wait-Event $openocd_info_id -Timeout $timeout
		Remove-Event $openocd_info_id
	} catch {
		Throw "The Wait-Event $openocd_info_id failed."
	}
}

Function Wait-Info-Putty {
    param($timeout=-1)
	#try {
		$resultevent = Wait-Event $putty_info_id -Timeout $timeout;
		Remove-Event $putty_info_id;
	#} catch {
	#	Throw "The Wait-Event $putty_info_id failed."
	#}
}

Function Wait-ResultRecord-Putty {
    param($timeout=-1)
    $resultevent = Wait-Event $putty_resultrecord_id -Timeout $timeout
    #Process-ResultRecord-GDB $resultevent.MessageData
    Remove-Event $putty_resultrecord_id
}


# Write-OpenOCD
#   writes to the OpenOCD process and awaits a resultrecord
#   @param - the line to write
#   @param - the timeout of the wait event; @default - 10
Function Write-Putty {
    param($line)
    $putty_input.WriteLine($line)
    Wait-Info-Putty;
}


#Load file to flash
Function Load-OpenOCD {
    # unlock flash
    param ($file)
    # specify executable and symbol file(s)
    #Write-GDB "-file-exec-and-symbols $file"
    
    #Write-GDB "stm32x unlock 0"
    # reset init
    #Write-Sam "OpenOCD:: reset init "
    #Write-Putty "reset init"
    Write-Sam "OpenOCD:: soft_reset_halt"
    Write-Putty "soft_reset_halt" #Emote
    Write-Sam "OpenOCD:: soft_reset_halt "
    Write-Putty "soft_reset_halt" #Emote
    # load
    Write-Sam "Loading file $file..."
    Write-Putty "flash write_image $file"
}

Function Load-RAM-OpenOCD {
    # unlock flash
    param ($file)
    # specify executable and symbol file(s)
    #Write-GDB "-file-exec-and-symbols $file"
    
    #Write-GDB "stm32x unlock 0"
    # reset init
    #Write-Sam "OpenOCD:: reset init "
    #Write-Putty "reset init"
    Write-Sam "OpenOCD:: reset init"
    Write-Putty "reset init" #Emote
    # load
    Write-Sam "Loading file to RAM $file..."
    Write-Putty "load_image $file"
}