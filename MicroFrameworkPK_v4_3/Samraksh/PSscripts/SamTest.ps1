
Function Write-Sam {
    param($line)
    Write-Host "(SAMTEST) $line" -f blue
}

Function Out-DBG {
	param($info)
	if($debug) {
		if($info) { 
			$info = "[$info]: "
		}
		Write-Host $info -f magenta -nonewline
		$input | Out-Host
	} else {
		$input | Out-Null
	}
}

Function IsNullOrEmpty {
    param($string)
    return [System.String]::IsNullOrEmpty($string)
}

Function Set-AXF {

}

Function Set-Project {

}

function Kill-All {
	Kill-OpenOCD
    Kill-Putty
	Kill-GDB
	#Kill-MSBuild
	Remove-Event *
	Unregister-Event *
	#Kill-Logic
}

Function Add-CustomExceptions {
    # custom exception types
    $source = @"
    	public class TimeoutException : System.Exception {}
    	public class BuildFailedException : System.Exception {}
"@
    Add-Type $source
}