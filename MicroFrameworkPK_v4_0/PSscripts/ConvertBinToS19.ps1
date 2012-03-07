#Mukundan Sridharan (mukundan.sridharan@samraksh.com)
#Copyright: The Samraksh Company

#$STM32=true;
$SoC8200=true;

#$Binfile="TestSuite.dat";
$ToolDir="D:\Main\Emote-dev\Dev\MicroFrameworkPK_v4_0\tools\bin";
$ObjDir = "D:\Main\Emote-dev\Dev\MicroFrameworkPK_v4_0\BuildOutput\public\Debug\Client\dat";
$ToolChainBin = "D:\Main\Emote-dev\ToolChains\codesourcery-nick\bin";
$Binfile=$Args[0];
$Tool = "$ToolDir\binToSrec.exe";
$OTemp = "$BinFile.temp";
$OutFile = $Args[1];
$ObjTool = "$ToolChainBin\arm-none-eabi-objcopy.exe";

$BaseAddress;
#if($STM32) {
#    $BaseAddress="080ce000";
    #}
#else if ($SoC8200) {
    $BaseAddress="80700000";
#}


write-host "Converting file $BinFile ...";

chdir $ObjDir;
$cmd = "$Tool -b $BaseAddress -i $BinFile -o $OTemp";
write-host "Executing: $cmd";
Invoke-Expression $cmd;
$cmd = "$ObjTool $OTemp $OutFile";
write-host "Executing: $cmd";
Invoke-Expression $cmd;

write-host "Done.";