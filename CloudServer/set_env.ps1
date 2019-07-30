
$scriptpath = split-path -parent $MyInvocation.MyCommand.Path
$ExtPath = $scriptpath+ "\ExtCode\"
echo $scriptpath
Write-Output "current gopath: ", $env:GoPath 
$env:GoPath += ";$scriptpath;$ExtPath"
Write-Output "current gopath: ", $env:GoPath 
