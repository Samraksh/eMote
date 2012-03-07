# openOCD configuration
#   exe                - name of the executable
$openocd.exe           = "openocd-x64-0.5.0-rc1.exe"
#   interface_cfg      - specify the interface config file
#$openocd.interface_cfg = "config\olimex-jtag-tiny.cfg";
$openocd.interface_cfg = "bin\config\olimex-arm-usb-tiny-h.cfg";
#   board_cfg          - specify the board config file
$openocd.board_cfg     = "bin\config\stm32xl.cfg"
#   path               - root path
$openocd.path          = "D:\Main\Emote-dev\Tools\openocd-x64-0.5.0-rc1"
#   path_bin           - path to binaries
$openocd.path_bin      = $openocd.path + "bin\"
#   path_tcl           - path to tcl (tool command language) files, these are openocd's .cfg files
$openocd.path_tcl      = $openocd.path + "tcl\"
#   output_id          - OutputDataReceived event SourceIdentifer
$openocd.output_id     = "OpenOCD.OutputDataReceived"
#   error_id           - ErrorDataReceived event SourceIdentifer
$openocd.error_id      = "OpenOCD.ErrorDataReceived"

# Start-OpenOCD
#   starts OpenOCD
function Start-OpenOCD {
    Write-Host "Starting openocd..."
    $openocd.process = New-Object System.Diagnostics.Process
    $p_openocd.StartInfo.Filename = "$openocd_exe"
    #$p_openocd.StartInfo.Arguments = "-s $path_openocd_config -f $openocd_interface_cfg -f $openocd_board_cfg"
    $p_openocd.StartInfo.Arguments = "-f $openocd_interface_cfg -f $openocd_board_cfg"
    $openocd.process.StartInfo.UseShellExecute = $FALSE
    $openocd.process.StartInfo.UseShellExecute = $FALSE
    $openocd.process.StartInfo.WindowStyle = "Hidden"
    # gdb streams
    $openocd.process.StartInfo.RedirectStandardInput = $TRUE
    $openocd.process.StartInfo.RedirectStandardOutput = $TRUE
    $openocd.process.StartInfo.RedirectStandardError = $TRUE
    # start
    $openocd.process.Start()
    # open streams
    $openocd.input = $process.StandardInput
    Register-ObjectEvent -InputObject $openocd.process -EventName OutputDataReceived -SourceIdentifier $openocd.output_id -action { if(![System.String]::IsNullOrEmpty($EventArgs.Data)) { Write-Host "o-ocd: " $EventArgs.Data } }
    $openocd.process.BeginOutputReadLine()
    Register-ObjectEvent -InputObject $openocd.process -EventName ErrorDataReceived -SourceIdentifier $openocd.error_id -action { if(![System.String]::IsNullOrEmpty($EventArgs.Data)) { Write-Host "e-ocd: " $EventArgs.Data } }
    $openocd.process.BeginErrorReadLine()
    Write-Host "+openOCD started"
}

# Kill-OpenOCD
function Kill-OpenOCD {
    Write-Sam "Killing OpenOCD..."
    Unregister-Event $openocd_output_id
    Unregister-Event $openocd_error_id
    $openocd_process.Kill()
    Write-Sam "OpenOCD was killed."
}