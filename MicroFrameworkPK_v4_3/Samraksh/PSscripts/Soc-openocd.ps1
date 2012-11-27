#Mukundan Sridharan (mukundan.sridharan@samraksh.com)
#Copyright: The Samraksh Company

$ScriptsRoot = "D:\Main\Emote-dev\Dev\MicroFrameworkPK_v4_0\PSScripts";

$ToolDir="D:\\Main\\SoC\\Tools\\openocd-bin\\";
$ToolChain="D:\\Main\\Emote-dev\\ToolChains\\codesourcery-nick\\bin\\";
$ObjFile = "openocd.exe";
$InterfaceConfig = "interface\olimex-jtag-tiny.cfg";
#$InterfaceConfig = "config\olimex-arm-usb-tiny-h.cfg";
$BoardConfig = "board\am3517evm.cfg";


# Working sets
$axf_path = "D:\\Main\\Emote-dev\\Dev\\MicroFrameworkPK_v4_0\\BuildOutput\\ARM\\GCC4.2\\le\\RAM\\debug\\SOC8200\\bin\\"
#$axf_file = "TinyCLR.axf"
$axf_file = "NativeSample.axf"
$axf_full = $axf_path + $axf_file

write-host "Killing any runing openOCD process ...";
$OOps = Get-Process -Name *openocd*;

if($OOps -ne $null) {
    #$a = read-host "OpenOCD seems to be running as process $OOps, to Kill process enter 'Y', to exit enter 'N' ";
    #if ($a -eq 'Y') {
    #    Stop-Process $OOps.Id;
    #} else {
    #    #exit(0);
    #} 
}else {
    write-host "No OpenOCD running, continuing..";
    chdir $ToolDir;
    $cmd = "$ToolDir\$ObjFile -ArgumentList '-f $InterfaceConfig -f $BoardConfig' ";
    write-host "Executing in backgound: $cmd";
    #Start-Process $ToolDir\$ObjFile -ArgumentList '-f config\olimex-arm-usb-tiny-h.cfg -f config\stm32xl.cfg'
    Start-Process $ToolDir\$ObjFile -ArgumentList '-f interface\olimex-jtag-tiny.cfg -f board\am3517evm.cfg'
}


#$OpenOCDPs = Start-Job "$cmd";
#Invoke-Expression $cmd;


sleep 3;

#write-host "Starting Putty.."
#chdir $ScriptsRoot;
#$putty_path="C:\Program Files (x86)\WinSCP\PuTTY"
#$putty_exe="putty.exe"

#Start-Process $putty_path\$putty_exe -ArgumentList 'telnet://localhost:4444'

$gdb_full = $ToolChain + '\arm-none-eabi-gdb.exe'

Start-Process $gdb_full -ArgumentList $axf_full
