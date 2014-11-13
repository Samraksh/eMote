#Ananth Muralidharan (ananth.muralidharan@samraksh.com)
#Copyright: The Samraksh Company
#Date: Nov 10, 2014
#Function: Compute RAM usage (split between heap, stack, .bss, .data)


$BaseDir = "D:\AnanthAtSamraksh\";
$MFVer= "MF\MicroFrameworkPK_v4_3\";
$GCC="GCC4.7"
$Solution="EmoteDotNow"

$MFDir = $BaseDir + $MFVer;
$ScriptsRoot = $BaseDir + $MFVer + "Samraksh\PSScripts";

$binDir = $MFDir + "BuildOutput\THUMB2\"+$GCC +"\le\FLASH\debug\" + $Solution +"\bin\";
$tinyclr_map_File = $binDir + "tinyclr.map";
$OutFile = $binDir + "\RAM_Sizes.txt";
$hexSum = 0;

$BSS = Get-Content $tinyclr_map_File | Select-String "\.bss" -CaseSensitive;

Try{
    foreach ($line in $BSS) {
        $fields = $line -split '\s+'
        #'{0:x}' -f $fields[3] | Out-Host
        $hexSum += '{0:x}' -f $fields[3]
    }
}
catch{
    "Exception"
}

'{0:x}' -f $hexSum | Out-Host
$hexSum | Out-Host
#Write-Host $BSS