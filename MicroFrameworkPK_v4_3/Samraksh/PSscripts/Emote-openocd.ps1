#Mukundan Sridharan (mukundan.sridharan@samraksh.com)
#Copyright: The Samraksh Company

$BaseDir = "G:\Main\MF-Dev\";
$MFVer= "MF4.3beta\MicroFrameworkPK_v4_3\";
$MFDir = $BaseDir + $MFVer;
$ScriptsRoot = $BaseDir + $MFVer + "Samraksh\PSScripts";

$ToolDir=$BaseDir + "JtagTools\openocd-x64-0.5.0-rc1\bin";
$ToolChain=$BaseDir + "ToolChains\codesourcery-nick\bin";
$ObjFile = "openocd-x64-0.5.0-rc1.exe";
#$InterfaceConfig = "config\olimex-jtag-tiny.cfg";
$InterfaceConfig = "config\olimex-arm-usb-tiny-h.cfg";
$BoardConfig = "config\stm32xl.cfg";


# Working sets
$axf_path = $MFDir + "\BuildOutput\\THUMB2\\GCC4.2\\le\\FLASH\\debug\\EmoteDotNow\\bin\\"
$axf_file = "TinyCLR.axf"
#$axf_file = "TinyBooter.axf"
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
    Start-Process $ToolDir\$ObjFile -ArgumentList '-f config\olimex-arm-usb-tiny-h.cfg -f config\stm32xl.cfg'
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
