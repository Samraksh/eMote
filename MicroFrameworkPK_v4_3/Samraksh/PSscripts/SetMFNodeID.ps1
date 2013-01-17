#Mukundan Sridharan (mukundan.sridharan@samraksh.com)
#Copyright: The Samraksh Company
#Function: Set MF_NODE_ID value


$New_ID= 612;

Function UpdateBinaryFile($file, $pos, $value) {
    $b1 = [byte] ($value % 256);
    $b2 = [byte] ($value / 256);
    echo "New bytes are: '$b1 $b2' ";
    
    $writer = [System.IO.File]::Open($outfile,[System.IO.FileMode]::Open);
    if($writer.CanWrite -And $writer.CanRead){
        $writer.Seek($pos, [System.IO.SeekOrigin]::Begin);
        
        $o1= $writer.ReadByte();
        $o2= $writer.ReadByte();
        echo "Replacing '$o1 $o2' with '$b1 $b2' ";
        $writer.Seek($pos, [System.IO.SeekOrigin]::Begin);
        $writer.WriteByte($b1);
        $writer.WriteByte($b2);
        $writer.Seek($pos, [System.IO.SeekOrigin]::Begin);
        $o1= $writer.ReadByte();
        $o2= $writer.ReadByte();
        echo "Values read back are : '$o1 $o2' ";
        
    $writer.Dispose();
    }else {
        echo "Problem opening binary file";
    }
}

$New_ID= 612 % 65535;

######Host specific paths################
$BaseDir = "G:\Main\MF-Dev\";
$MFVer= "MF4.3beta\MicroFrameworkPK_v4_3\";
$GCC = "GCC4.2";
$ToolChain=$BaseDir + "ToolChains\codesourcery-nick\bin";
##################################

$MFDir = $BaseDir + $MFVer;
$ScriptsRoot = $BaseDir + $MFVer + "Samraksh\PSScripts";
$Project="TinyCLR";
# Working sets
$axf_path = $MFDir + "\BuildOutput\THUMB2\" + $GCC +"\le\FLASH\\debug\EmoteDotNow\bin\";
#$axf_full = $axf_path + $Project + ".bin\" + "ER_FLASH"
$axf_full = $axf_path + $Project + ".axf"
$outfile = $axf_path + $Project + "_" + $New_ID + ".axf"


$Tool = $ToolChain + "\arm-none-eabi-objdump.exe";

Copy-Item $axf_full $outfile;
echo "Created a new outfile: $outfile ";

$rawOut = Invoke-Expression "$Tool -h $outfile" 
$ram_rw = $rawOut | Select-String "ER_RAM_RW" -CaseSensitive
$ram_rw = -split $ram_rw 
$flash0ffset = '0x' + $ram_rw[5];
$ram_start_address = '0x' + $ram_rw[3];

        
$rawOut = Invoke-Expression "$Tool -t $outfile" 
$mf_id = $rawOut | Select-String "MF_NODE_ID" -CaseSensitive
$mf_id = -split $mf_id
$mf_id_offset = '0x' + $mf_id[0] 
$mf_id_offset = $mf_id_offset - $ram_start_address;

$file_offset = [int]$flash0ffset + [int]$mf_id_offset;
echo "File offset of MF_NODE_ID is:  $file_offset"

#$hexFile = gc -en byte $axf_full ; 
#$hexFile = gc -en byte $axf_full | % { '{0:X2}' -f $_ }; 

#$o1 = $hexFile[$file_offset];
#$o2 = $hexFile[$file_offset+1];
UpdateBinaryFile $outfile $file_offset $New_ID;


