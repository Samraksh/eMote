Function CalculateDelayAndJitter
{
    param(
        [Parameter(Mandatory=$True, Position=0)][string]$channelValueOrig,
		[Parameter(Mandatory=$True, Position=1)][string]$timerValue
	)

    $channelValue = 'channel ' + $channelValueOrig
    $timerValueFile = "-Delay_Timer$timerValue.txt"
    $curTime = Get-Date -UFormat %H:%M:%S
    Write-Host "Calculating delay for timer $timerValue. Time is " $curTime
    $i = $a = $b = 0
    function new-array {,$args}
    $delayChannel = new-array 0
    Set-Variable -Name $delayChannel -Scope global
    

    
    while($i -lt $CSVData.Count)
    {
        if($CSVData.$channelValue[$i] -eq 1 -and $CSVData.$channelValue[$i+1] -eq 0){
            $a = $CSVData.'Time[s]'[$i+1]
        }

        if($CSVData.$channelValue[$i] -eq 0 -and $CSVData.$channelValue[$i+1] -eq 1){
            $b = $CSVData.'Time[s]'[$i+1]
        
            if($a -ne 0)
            {
                if($delayChannel[0] -ne 0){
                    $delayChannel += $b - $a
                }
                else{
                    $delayChannel[0] = $b - $a            
                }
                $a = $b = 0
            }
        }
    
        $i++
        
    }

    
    $curTime = Get-Date -UFormat %H:%M:%S
    Write-Host "Writing delay values for timer $timerValue to file. Time is " $curTime
    $j = 0
    $OutFileName = $BaseDir + $DelayValue + $FileName + $timerValueFile
    $stream = [System.IO.StreamWriter] $OutFileName
    
    while($j -lt $delayChannel.Count)
    {
        #Converting all delays and jitter to microseconds
        $stream.WriteLine($delayChannel[$j] * [Math]::Pow(10,6))
        $j++
    }

    $stream.Close()
    

    CalculateJitter $channelValueOrig $timerValue $delayChannel
}

#^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Function CalculateJitter
{
    param(
        [Parameter(Mandatory=$True, Position=0)][string]$channelValue,
		[Parameter(Mandatory=$True, Position=1)][string]$timerValue,
        [Parameter(Mandatory=$True, Position=2)]$delayChannel
	)

    $channelValue = 'channel ' + $channelValue
    $timerValueFile = "-Jitter_Timer$timerValue.txt"
    $curTime = Get-Date -UFormat %H:%M:%S
    Write-Host "Calculating jitter for timer $timerValue. Time is " $curTime
    function new-array {,$args}
    $jitterChannel = new-array 0
    
    $j = 0
    while($j+1 -le ($delayChannel.Count)-1)
    {
        #JitterChannel0 is a dynamic array created with an initial value of 0.
        #The first if...else is present in order to replace the 0.
        if($jitterChannel[0] -ne 0){
            #Below "if" is present to avoid going beyond the length of DelayChannel0.
            #Array starts from 0 and goes until "($DelayChannel0.Count)-1". 
            #Doing a "$DelayChannel0[$j+1]" when $j is equal to "($DelayChannel0.Count)-1", means that we are reading a value which is not present.
            #This can be avoided in the while loop itself.
             
            #if($DelayChannel0[$j+1] -gt 0){
                $jitterChannel += [math]::abs($delayChannel[$j+1] - $delayChannel[$j])
            #}
        }
        else{
            $jitterChannel[0] = [math]::abs($delayChannel[$j+1] - $delayChannel[$j])
        }
        $j++
    }

    $curTime = Get-Date -UFormat %H:%M:%S
    Write-Host "Writing jitter values for timer $timerValue to file. Time is " $curTime
    $j = 0
    $OutFileName = $BaseDir + $DelayValue + $FileName + $timerValueFile
    $stream = [System.IO.StreamWriter] $OutFileName
    
    while($j -lt $jitterChannel.Count)
    {
        #Converting all delays and jitter to microseconds
        $stream.WriteLine($jitterChannel[$j] * [Math]::Pow(10,6))
        $j++
    }

    $stream.Close()
    
}

#------------------------------------------------------------------------------------------
<#
Function CalculateDelayChannel1
{
    $curTime = Get-Date -UFormat %H:%M:%S
    Write-Host 'Calculating delay for timer 2. Time is ' $curTime
    $i = 0
    $a = $b = 0
    function new-array {,$args}
    $DelayChannel1 = new-array 0


    while($i -lt $CSVData.Count)
    {
        if($CSVData.'channel 1'[$i] -eq 1 -and $CSVData.'channel 1'[$i+1] -eq 0){
            $a = $CSVData.'Time[s]'[$i+1]
        }

        if($CSVData.'channel 1'[$i] -eq 0 -and $CSVData.'channel 1'[$i+1] -eq 1){
            $b = $CSVData.'Time[s]'[$i+1]
        
            if($a -ne 0)
            {
                if($DelayChannel1[0] -ne 0){
                    $DelayChannel1 += $b - $a
                }
                else{
                    $DelayChannel1[0] = $b - $a            
                }
                $a = $b = 0
            }
        }
    
        $i++
        
    }

    #Write-Host $DelayChannel1.Count
    
    $j = 0
    $OutFileName = $BaseDir + $DelayValue + $FileName + "-DelayTimer2.txt"
    $stream = [System.IO.StreamWriter] $OutFileName
    $curTime = Get-Date -UFormat %H:%M:%S
    Write-Host 'Writing delay values for timer 2 to file. Time is ' $curTime
    while($j -lt $DelayChannel1.Count)
    {
        #Write-Host $DelayChannel1[$j]
        #Converting all delays and jitter to microseconds
        $stream.WriteLine($DelayChannel1[$j] * [Math]::Pow(10,6))
        $j++
    }
    
    $stream.Close()

    CalculateJitterChannel1
}

#^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Function CalculateJitterChannel1
{
    $curTime = Get-Date -UFormat %H:%M:%S
    Write-Host 'Calculating jitter for timer 2. Time is ' $curTime
    function new-array {,$args}
    $JitterChannel1 = new-array 0

    $j = 0
    while($j+1 -le ($DelayChannel1.Count)-1)
    {
        if($JitterChannel1[0] -ne 0){
            #if($DelayChannel1[$j+1] -gt 0){
                $JitterChannel1 += [math]::abs($DelayChannel1[$j+1] - $DelayChannel1[$j])
            #}
        }
        else{
            $JitterChannel1[0] = [math]::abs($DelayChannel1[$j+1] - $DelayChannel1[$j])
        }
        $j++
    }

    #Write-Host $JitterChannel1.count
    
    $j = 0
    $OutFileName = $BaseDir + $DelayValue + $FileName + "-JitterTimer2.txt"
    # ----------------------------------
    # According to this -- blogs.technet.com/b/gbordier/archive/2009/05/05/powershell-and-writing-files-how-fast-can-you-write-to-a-file.aspx
    # using a stream to write to a file is the fastest method.
    # ----------------------------------
    $stream = [System.IO.StreamWriter] $OutFileName
    $curTime = Get-Date -UFormat %H:%M:%S
    Write-Host 'Writing jitter values for timer 2 to file. Time is ' $curTime
    while($j -lt $JitterChannel1.Count)
    {
        #Write-Host $JitterChannel1[$j]
        #Converting all delays and jitter to microseconds
        $stream.WriteLine($JitterChannel1[$j] * [Math]::Pow(10,6))
        $j++
    }

    $stream.Close()
    
}
#>

#*************************************************************************************************
<#
Algorithm: 
1. Find difference between end and start of transition. 
2. Transition end is   -- 0 to 1 transition; measure at 1.
3. Transition start is -- 1 to 0 transition; measure at 0.
4. All delays are stored in a dynamic array.
5. Difference between delays is jitter which is also stored in a dynamic array.
#>

$BaseDir = 'D:\AnanthAtSamraksh\VirtualTimerChanges\Baseline\VirtTimerBaseLine\'
$DelayValue = '250us\'
$FileName = '250us'
$CSVFile = $BaseDir + $DelayValue + $FileName + '.csv'

<#
$CSVData = Get-Content -Path $CSVFile | Select-Object -Skip 1 | ForEach-Object {
    $lineValue = $_.split(',') -replace('\s+','')
    Write-Host $lineValue
}
#>

$CSVData = Import-Csv $CSVFile | Select-Object -Skip 1

#Syntax:
#CalculateDelay 'channel number' 'timer number'
#CalculateJitter 'channel number' 'timer number'

CalculateDelayAndJitter '0' '0'
<#
CalculateDelayAndJitter '1' '1'
CalculateDelayAndJitter '2' '2'
CalculateDelayAndJitter '3' '3'
CalculateDelayAndJitter '4' '4'
CalculateDelayAndJitter '5' '5'
CalculateDelayAndJitter '6' '6'
CalculateDelayAndJitter '7' '7'
#>

#CalculateDelayChannel0
#CalculateDelayChannel1

