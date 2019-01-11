param (
    [Parameter(HelpMessage="Specify app solution build: stm32|adapt|smartfusion2")][string]$solution = 'SmartFusion2',
	[Parameter(HelpMessage="Specify app project to build: TinyCLR|TinyBooter")][String]$project = "TinyCLR",
    [Parameter(HelpMessage="Specify action: build|clean: ")][string]$action = "build"
)

write-host "Welcome to Samraksh Emote Dev Prompt"

#$cur_path=$pwd.ToString()
$repo_path="D:\Main\Repos\eMote\"
write-host "Repo path is $repo_path"
$gcc_ver="6.3.1"
$buildCmd = "msbuild /t:" + $action + " " + $project + ".proj"

$envScript = $repo_path + "MicroFrameworkPK_v4_3\Solutions\setEnv.ps1 " + $gcc_ver

iex $envScript 
if (-Not $?) { "command failed"; exit }

$projPath= $repo_path+"MicroFrameworkPK_v4_3\Solutions\"+$solution+"\"+$project

cd $projPath
iex $buildCmd