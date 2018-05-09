#Mukundan Sridharan (mukundan.sridharan@samraksh.com)
#Copyright: The Samraksh Company

param (
    [Parameter(HelpMessage="Specify app bin file for which hmac needs to be calculated")][string]$BinFile,
    [Parameter(HelpMessage="Specify app platform to be used: stm32|adapt|sf2")][string]$key = ""
)

#$message = 'Message'
if ($BinFile -eq ""){
    echo "Pass a file as first argument to compute its signature ";
    exit
}

Function ConvertToHex {

    [cmdletbinding()]

    param(
        [parameter(Mandatory=$true)]
        [Byte[]]
        $Bytes
    )

    $HexString = [System.Text.StringBuilder]::new($Bytes.Length * 2)

    ForEach($byte in $Bytes){
        $HexString.AppendFormat("0x{0:x2}, ", $byte) | Out-Null
    }

    $HexString.ToString()
}

#######Main starts here #####

echo "Computing signature for file $BinFile"
$exec= Get-Content $BinFile -Encoding byte
#echo $exec

[byte[]]$defaultKey = 0xC6, 0x29, 0x73, 0xE3, 0xC8, 0xD4, 0xFC, 0xB6,
        0x89, 0x36, 0x46, 0xF9, 0x58, 0xE5, 0xF5, 0xE5,
        0x25, 0xC2, 0xE4, 0x1E, 0xCC, 0xA8, 0xC3, 0xEF,
        0xA2, 0x8D, 0x24, 0xDE, 0xFD, 0x19, 0xDA, 0x08

if ($key -eq ""){
    $key = $defaultKey
}
$hmacsha = New-Object System.Security.Cryptography.HMACSHA256
$hmacsha.key = [Text.Encoding]::ASCII.GetBytes($key)
$sig = $hmacsha.ComputeHash($exec)

#$signature = [Convert]::ToBase64String($signature)

echo "Decimal Sig: $sig"

$hexS=ConvertToHex($sig)

echo "Hex Sig: $hexS"

# Do we get the expected signature?
#echo ($signature -eq 'qnR8UCqJggD55PohusaBNviGoOJ67HC6Btry4qXLVZc=')
