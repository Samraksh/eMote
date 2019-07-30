param ([Switch]
    
    [Alias("H","-h")]$Help = $false,
    [Parameter(HelpMessage="specify libpath")][string]$libpath,
	[Parameter(HelpMessage="Specify patrern")][string]$pattern = "rot",
	[Parameter(HelpMessage="Sort on Ram size or Code size")][bool]$exclude = 0
)

if ($libpath -eq ""){
    echo "Pass a directory and pattern you want to list files  for ";
    exit
}

if ($exclude){
    $libfiles = Get-ChildItem $libPath -exclude *$pattern* | Where-Object {$_.Extension -match  "lib"} 
}
else {
    $libfiles = Get-ChildItem $libPath -filter "*$pattern*.lib"
}

return $libfiles.Name
