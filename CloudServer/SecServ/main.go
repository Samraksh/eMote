package main

import (
	"flag"
	"strings"
)


func StartServerMode(string mode) {
	if mode == "tcp" {
		StartTCPServer()
	} else {
		StartUSPServer()
	}
}

//main
func main() {
	flagMode := flag.String("mode", "server", "start in client or server mode")
	flag.Parse()
	if strings.ToLower(*flagMode) == "server" {
		StartServerMode("udp")
	} else {
		StartClientMode()
	}
}
