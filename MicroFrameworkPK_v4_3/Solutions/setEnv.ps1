param (
    [Parameter(HelpMessage="Specify gcc version: 5.4.1|6.3.1")][string]$repoPath,
    [Parameter(HelpMessage="Specify gcc version: 5.4.1|6.3.1")][string]$gcc_ver = "5.4.1"
)


write-host "Repo path is $repoPath"

$cmd_param= $gcc_ver + " " + $repoPath +"\..\TestSystem\Compilers\GCC" + $gcc_ver
$env_cmd=$repoPath+ "MicroFrameworkPK_v4_3\setenv_gcc.cmd " + $cmd_param
write-host "Envscript cmd: $env_cmd "


$envScript=$repoPath+"MicroFrameworkPK_v4_3\Samraksh\PSscripts\Invoke-Environment.ps1 " 

$fcommand = $envScript + '"' + $env_cmd + '"'

iex $fcommand 