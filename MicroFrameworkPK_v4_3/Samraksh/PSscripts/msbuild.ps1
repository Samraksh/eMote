# msbuild configuration
#   exe        - name of the executable
$msbuild_exe = "cmd.exe"
#   output_id  - OutputDataReceived event SourceIdentifer
$msbuild_output_id = "MSBuild.OutputDataReceived"
#   error_id   - ErrorDataReceived event SourceIdentifer
$msbuild_error_id  = "MSBuild.ErrorDataReceived"
#   buildcompelte_id - msbuild build complete event SourceIdentifer
$msbuild_buildcomplete_id = "MSBuild.BuildCompleted"

# Start-MSBuild
#   starts an instance of MSBuild
Function Start-MSBuild {
    Write-Sam "Starting MSBuild..."
    $process = New-Object System.Diagnostics.Process
    $process.StartInfo.Filename = $msbuild_exe
    $process.StartInfo.UseShellExecute = $FALSE
    $process.StartInfo.WindowStyle = "Hidden"
    # streams
    $process.StartInfo.RedirectStandardInput = $TRUE
    $process.StartInfo.RedirectStandardOutput = $TRUE
    $process.StartInfo.RedirectStandardError = $TRUE
    # start
    $process.Start() | Out-DBG "MSBuild.Process.Start()"
    # StandardInput stream
    $input = $process.StandardInput
    # StandardOutput stream
    Register-ObjectEvent $process OutputDataReceived $msbuild_output_id { $EventArgs.Data | Out-DBG "MSBuild.StandardOutput"; if($EventArgs.Data -match "build (su|f)") { New-Event $msbuild_buildcomplete_id -MessageData $EventArgs.Data | Out-DBG "MSBuild.NewEvent.BuildComplete"; } } | Out-DBG "MSBuild.RegisterObjectEvent.StandardOutput"
    $process.BeginOutputReadLine()
    # StandardError stream
    Register-ObjectEvent $process ErrorDataReceived $msbuild_error_id { $EventArgs.Data | Out-DBG "MSBuild.StandardError"; } | Out-DBG "MSBuild.RegisterObjectEvent.StandardError"
    $process.BeginErrorReadLine()
    # Export handles up one level
    Set-Variable msbuild_process $process -Scope Global
    Set-Variable msbuild_input $input -Scope Global
    Write-Sam "MSBuild was started."
}

# Kill-MSBuild
#   kills MSBuild and unregisters associated events
Function Kill-MSBuild {
    Write-Sam "Killing MSBuild..."
    Unregister-Event $msbuild_output_id
    Unregister-Event $msbuild_error_id
    $msbuild_process.Kill()
    Write-Sam "MSBuild was killed."
}

Function Write-MSBuild {
    param($line)
    $msbuild_input.WriteLine($line)
}

Function SetEnv-MSBuild {
	Write-Sam "Setting environment..."
	Write-MSBuild "$drive"
    Write-MSBuild "cd $drive\MicroFrameworkPK_v4_0"
    Write-MSBuild "setenv_gcc.cmd $gdb_path_root"
    Write-MSBuild "cd Solutions\STM32F10x\NativeSample"
	Write-Sam "Environment set."
}

Function Clean-MSBuild {
    Trap [TimeoutException] {
        Write-Error "Clean timed out."
        break
    }
    Trap [BuildFailedException] {
        Write-Error "Clean failed."
        break
    }
    Write-Sam "Cleaning project..."
    Write-MSBuild "msbuild $project /target:clean"
    Wait-BuildCompleted-MSBuild 10
    Write-Sam "Project cleaned."
}

Function Build-MSBuild {
    Trap [TimeoutException] {
        Write-Error "Build timed out."
        break
    }
    Trap [BuildFailedException] {
        Write-Error "Build failed."
        break
    }
    Write-Sam "Building project..."
    Write-MSBuild "msbuild $project /target:build"
	Wait-BuildCompleted-MSBuild 500
    Write-Sam "Project built."
}

Function Wait-BuildCompleted-MSBuild {
    param($timeout=-1)
	$resultevent = Wait-Event $msbuild_buildcomplete_id -Timeout $timeout
	try {
		Remove-Event $msbuild_buildcomplete_id -EA stop
	} catch {
		Throw New-Object TimeoutException
	}
	if($resultevent.MessageData -eq "BUILD FAILED") {
		Throw New-Object BuildFailedException
	}
}