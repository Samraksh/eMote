package main

import (
	"fmt"
	"flag"
	"strings"
)

const COM_PORTO = "udp"
const COM_PORT = 6001

func StartServerMode(mode string) {
	if mode == "udp" {
		StartUDPServer(COM_PORT)
	} else {
		StartTCPServer(COM_PORT)
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
	flagProto := flag.String("proto", "tcp", "use udp instead of tcp")
	flag.Parse()
	if strings.ToLower(*flagMode) == "client" {
		switch strings.ToLower(*flagProto) {
		case "udp":
			StartClientMode("udp")
		default:
			StartClientMode("tcp")
		}	
	} else {
		switch strings.ToLower(*flagProto) {
		case "udp":
			fmt.Println("Starting udp server..")
			StartServerMode("udp")
		default:
			fmt.Println("Starting tcp server..")
			StartServerMode("tcp")
		}
		
	}
}
