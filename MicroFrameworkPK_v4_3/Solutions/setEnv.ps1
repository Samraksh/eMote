param (
	[Parameter(HelpMessage="Specify gcc version: 5.4.1|6.3.1")][string]$gcc_ver = "6.3.1"
)

#$cur_path=$pwd.ToString()
$repo_path=" D:\Main\Repos\eMote\"
write-host "Repo path is $repo_path"

$cmd_param= $gcc_ver + " " + $repo_path +"\..\TestSystem\Compilers\GCC" + $gcc_ver
$env_cmd=$repo_path+ "MicroFrameworkPK_v4_3\setenv_gcc.cmd " + $cmd_param
write-host "Envscript cmd: $env_cmd "


$envScript=$repo_path+"MicroFrameworkPK_v4_3\Samraksh\PSscripts\Invoke-Environment.ps1 " 

$fcommand = $envScript + '"' + $env_cmd + '"'

iex $fcommand 