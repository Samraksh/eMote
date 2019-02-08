package main

import (
	"flag"
	"strings"
)

//main
func main() {
	flagMode := flag.String("mode", "server", "start in client or server mode")
	flag.Parse()
	if strings.ToLower(*flagMode) == "server" {
		StartServerMode()
	} else {
		StartClientMode()
	}
}
