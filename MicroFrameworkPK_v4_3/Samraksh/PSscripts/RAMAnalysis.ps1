#Mukundan Sridharan (mukundan.sridharan@samraksh.com)
#Copyright: The Samraksh Company
#Date: Nov 15, 2012
#Improved on: March 22, 2017
#Function: Analyze RAM usage of individual modules using the objdump tool, sort them and write to a output file

param (
	[Parameter(HelpMessage="Specify solution to analyze")][string]$Solution = "SmartFusion2",
	[Parameter(HelpMessage="Specify flavor(Release|Debug) to analyze")][string]$flavor = "Debug",
	[Parameter(HelpMessage="Specify basedir of MF source tree")][string]$BaseDir = "D:\Main\Repos\"
)

$ToolChain=$BaseDir + "TestSystem\Compilers\GCC5.4.1\bin";
$MFVer= "eMote\MicroFrameworkPK_v4_3\";
#$GCC="GCC5.4"
$GCC="GCC6.3"

$MFDir = $BaseDir + $MFVer;
$ScriptsRoot = $BaseDir + $MFVer + "Samraksh\PSScripts";

$libDir = $MFDir + "BuildOutput\THUMB2\"+$GCC +"\le\FLASH\"+ $flavor + "\"+$Solution +"\lib";
$OutFile = $libDir + "\RAM_Sizes.txt";

$Tool = $ToolChain + "\arm-none-eabi-objdump.exe";

#function List-Libs {
    Write-Host "Getting the list of lib files..."
    $LibList = Get-ChildItem $libDir\*.lib
    #$LibList = Get-ChildItem $libDir\*watchdog_pal.lib
    $fcount =0;
    $Bss_Size=@()
    $Data_Size=@()
    $Ram_Size=@()
    $FList=@{}
    foreach ($lib in $LibList){
    
        $fname =  $lib -split  "\\";
        $fname = $fname[$fname.Count-1]
        #Write-Host "File:" $FList[$fcount]
        $Bss_Size+=@(0);
        $Data_Size+=@(0);
        $Ram_Size+=@(0);
        $rawOut = Invoke-Expression "$Tool -h $lib" 
        $BSS = $rawOut | Select-String "\.bss" -CaseSensitive ;
        $Data = $rawOut | Select-String "\.data" -CaseSensitive;
        
        foreach ($i in $BSS){
            $fields = -split $i ;
            $fields[2] = '0x' +$fields[2];
            $Bss_Size[$fcount]+= $fields[2];
            #Write-Host "BSS iter size: " $fields[2];
            #Write-Host "BSS total size: " $Bss_Size[$fcount];
        }
        
        foreach ($i in $Data){
            $fields = -split $i ;
            $fields[2] = '0x' +$fields[2];
            $Data_Size[$fcount]+=$fields[2];
            #Write-Host "DATA iter size: " $fields[2];
            #Write-Host "DATA total size: " $Data_Size[$fcount];
        }
        [int]$Ram_Size[$fcount] = ([int]$Bss_Size[$fcount]+[int]$Data_Size[$fcount]);
        $FList[$fname]=[int]$Ram_Size[$fcount];
        $fcount++;
        #Write-Host $rawOut
    }
    $total=[int]0;
    foreach ($i in $FList.Values) {$total+=[int]$i}
    $FList["Total"]= $total;
    
    Write-Host "ModuleName `t`t`t`t: Size"
    if(Test-Path $OutFile -PathType Leaf) {del $OutFile}
    "ModuleName `t`t`t`t`t`t`t`t: Size (in bytes)" | Add-Content $OutFile
    $FList.GetEnumerator() | Sort-Object Value 
    $FList.GetEnumerator() | Sort-Object Value | ForEach-Object {"{0} `t`t`t`t`t`t`t`t: {1}" -f $_.Name,$_.Value}| Add-Content $OutFile
#}