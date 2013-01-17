#Mukundan Sridharan (mukundan.sridharan@samraksh.com)
#Copyright: The Samraksh Company

$STM32=true;
#$SoC8200=true;
$BaseDir = "F:\Mukundan-Main2\MF-Dev\";
$MFVer= "MF4.3beta\MicroFrameworkPK_v4_3\";
$MFDir = $BaseDir + $MFVer;
$ScriptsRoot = $BaseDir + $MFVer + "Samraksh\PSScripts";

#$Binfile="TestSuite.dat";
$ToolDir=$MFDir + "\tools\bin";
$ObjDir = $MFDir + "\BuildOutput\public\Debug\Client\dat";
$ToolChainBin = $BaseDir + "ToolChains\codesourcery-nick\bin";
$Binfile=$Args[0];
$Tool = "$ToolDir\binToSrec.exe";
$OTemp = "$BinFile.temp";
$OutFile = $Args[1];
$ObjTool = "$ToolChainBin\arm-none-eabi-objcopy.exe";

#$BaseAddress;
#if($STM32) {
    $BaseAddress="080a0000";
#}
#else if ($SoC8200) {
    #$BaseAddress="80700000";
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