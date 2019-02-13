package main

import (
	"flag"
	"strings"
)

const COM_PORTO = "udp"
const COM_PORT = 6001

func StartServerMode(mode string) {
	if mode == "tcp" {
		StartTCPServer(COM_PORT)
	} else {
		StartUDPServer(COM_PORT)
	}
}

func StartClientMode(mode string) {
	if mode == "tcp" {
		StartTCPClient(COM_PORT)
	} else {
		StartUDPClient(COM_PORT)
	}
}

//main
func main() {
	flagMode := flag.String("mode", "server", "start in client or server mode")
	flag.Parse()
	if strings.ToLower(*flagMode) == "server" {
		StartServerMode("udp")
	} else {
		StartClientMode("udp")
	}
}
