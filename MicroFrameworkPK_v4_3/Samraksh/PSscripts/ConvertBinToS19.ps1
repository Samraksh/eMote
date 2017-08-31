#Mukundan Sridharan (mukundan.sridharan@samraksh.com)
#Copyright: The Samraksh Company


param (
	[Parameter(HelpMessage="Specify app bin file to convert")][string]$BinFile,
    [Parameter(HelpMessage="Specify app platform to be used: stm32|adapt|sf2|max3263")][string]$Platform = "max3263"
)



if ($psboundparameters.count -lt 1){
    write-host "Supplied: $($psboundparameters.count) parameters. Give atleast one parameter, the file to convert"
    exit;

}

$SPOCLIENT = $PSScriptRoot+ "\..\..";
$REPODIR = $SPOCLIENT + "\..\..";

$MFDir = $SPOCLIENT;
$ScriptsRoot = $PSScriptRoot;
$GCCVER = "6.3.1";

#$Binfile="TestSuite.dat";
$ToolDir=$MFDir + "\tools\bin";
#$ObjDir = $MFDir + "\BuildOutput\public\Debug\Client\dat";
$ToolChainBin = $REPODIR + "\TestSystem\Compilers\GCC" + $GCCVER + "\bin\" ;
$ObjTool = "$ToolChainBin\arm-none-eabi-objcopy.exe";
$Tool = "$ToolDir\binToSrec.exe";


#Output files
$OTemp = "$BinFile.srec";
#Remove .dat from input file and replace with .S19
$OutFile = $BinFile + ".s19" ;


#Find a suitable baseaddress for the platform
$BaseAddress;
if($Platform -eq "stm32") {
    $BaseAddress="080a0000";
}
elseif ($Platform -eq "adapt") {
    $BaseAddress="80700000";
}
elseif ($Platform -eq "sf2") {
    $BaseAddress="60064800";
}
elseif ($Platform -eq "max3263") {
    $BaseAddress="00162000";
}
write-host "Using App base address $BaseAddress, assuming its for $Platform ...";
write-host "Converting file $BinFile ...";

#chdir $ObjDir;
$cmd = "$Tool -b $BaseAddress -i $BinFile -o $OTemp";
write-host "Executing: $cmd";
Invoke-Expression $cmd;
$cmd = "$ObjTool $OTemp $OutFile";
write-host "Executing: $cmd";
Invoke-Expression $cmd;

write-host "Done.";