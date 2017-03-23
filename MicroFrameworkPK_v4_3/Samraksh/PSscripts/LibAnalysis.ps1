#Mukundan Sridharan (mukundan.sridharan@samraksh.com)
#Copyright: The Samraksh Company
#Date: Nov 15, 2012
#Improved on: March 22, 2017
#Function: Analyze RAM and Code size of individual modules using the obj tools, sort them and write to a output file


#Example Use: .\LibAnalysis.ps1 EmoteDotNow Release 


param (
	[Parameter(HelpMessage="Specify solution to analyze")][string]$Solution = "EmoteDotNow",
	[Parameter(HelpMessage="Specify flavor(Release|Debug) to analyze")][string]$flavor = "Debug",
	[Parameter(HelpMessage="Sort on Ram size or Code size")][bool]$RamSort = 0,
	[Parameter(HelpMessage="Specify basedir of MF source tree")][string]$BaseDir = "D:\Main\Repos\"
)

$ToolChain=$BaseDir + "TestSystem\Compilers\GCC5.4.1\bin";
$MFVer= "MF\MicroFrameworkPK_v4_3\";
$GCC="GCC5.4"

$MFDir = $BaseDir + $MFVer;
$ScriptsRoot = $BaseDir + $MFVer + "Samraksh\PSScripts";

$libDir = $MFDir + "BuildOutput\THUMB2\"+$GCC +"\le\FLASH\"+ $flavor + "\"+$Solution +"\lib";
$OutFile = $libDir + "\0LibCodeSizes.txt";

$Tool = $ToolChain + "\arm-none-eabi-size.exe";

#function List-Libs {
    Write-Host "Getting the list of lib files..."
    $LibList = Get-ChildItem $libDir\*.lib
    #$LibList = Get-ChildItem $libDir\*watchdog_pal.lib
    #$fcount =0;
    #$Bss_Size=@()
    #$Data_Size=@()
    #$Ram_Size=@()
	$FRamList=@{}
    $FCodeList=@{}
    foreach ($lib in $LibList){
    
        $fname =  $lib -split  "\\";
        $fname = $fname[$fname.Count-1]
        #Write-Host "File:" $FList[$fcount]
        $Bss_Size+=@(0);
        $Data_Size+=@(0);
        $Ram_Size+=@(0);
        $rawOut = Invoke-Expression "$Tool -t $lib" 
        #$BSS = $rawOut | Select-String "\.bss" -CaseSensitive ;
        #$Data = $rawOut | Select-String "\.data" -CaseSensitive;
		$Totals = $rawOut | Select-String "(TOTALS)" -CaseSensitive
        
		$t_fields = -split $Totals
		$codeSize = $t_fields[0];
		[int]$ramSize = [int]$t_fields[1]+[int]$t_fields[2];
		
        #[int]$Ram_Size[$fcount] = ([int]$Bss_Size[$fcount]+[int]$Data_Size[$fcount]);
        $FRamList[$fname]=[int]$ramSize
		$FCodeList[$fname]= [math]::Round([float]$codeSize/1024,2)
        $fcount++;
        #Write-Host $rawOut
    }
    $ram_total=[int]0;
    foreach ($i in $FRamList.Values) {$ram_total+=[int]$i}
    $FRamList["Total"]= $ram_total;
	
	$code_total=[int]0;
    foreach ($i in $FCodeList.Values) {$code_total+=[float]$i}
    $FCodeList["Total"]= [math]::Round($code_total,2);
    
	
	if($RamSort) {
		Write-Host "Sort on RAM Size ..."
		Write-Host "ModuleName `t`t`t Ram Size(in Bytes) `t`t`t Code Size(in KB)"
		if(Test-Path $OutFile -PathType Leaf) {del $OutFile}
		"ModuleName `t`t`t`t  Ram Size (in bytes) `t`t`t`t Code Size (in KB)" | Add-Content $OutFile
		$FRamList.GetEnumerator() | Sort-Object Value | ForEach-Object {"{0} `t`t`t {1} `t`t`t {2}" -f $_.Name, $_.Value,$FCodeList[$_.Name]}
		$FRamList.GetEnumerator() | Sort-Object Value | ForEach-Object {"{0} `t`t`t`t`t`t`t`t`: {1} `t`t`t`t`t`t`t`t: {2}" -f $_.Name, $_.Value,$FCodeList[$_.Name]}| Add-Content $OutFile
	}
	else {
		Write-Host "Sort on Code Size ...`n`n"
		Write-Host "ModuleName `t`t`t Ram Size(in Bytes) `t`t`t Code Size(in KB)"
		if(Test-Path $OutFile -PathType Leaf) {del $OutFile}
		"ModuleName `t`t`t`t  Ram Size (in bytes) `t`t`t`t Code Size (in KB)" | Add-Content $OutFile
		$FCodeList.GetEnumerator() | Sort-Object Value | ForEach-Object {"{0} `t`t`t {1} `t`t`t {2}" -f $_.Name,$FRamList[$_.Name], $_.Value}
		$FCodeList.GetEnumerator() | Sort-Object Value | ForEach-Object {"{0} `t`t`t`t {1} `t`t`t`t {2}" -f $_.Name,$FRamList[$_.Name], $_.Value}| Add-Content $OutFile
	}
#}