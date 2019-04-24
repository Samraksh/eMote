package main

import (
	"flag"
	"fmt"
	//"os"
	"strings"
	//"time"
	soc "Sockets"
)

//const COM_PORTO = "udp"
//const COM_PORT = 6001

func TcpClientMain(port int) {
	//client := soc.StartTCPClient(COM_PORT)
	//go client.receive()
	/*
		for {
			reader := bufio.NewReader(os.Stdin)
			message, _ := reader.ReadString('\n')
			connection.Write([]byte(strings.TrimRight(message, "\n")))
		}*/

	/*for i := 0; i < 200; i += 2 {
		fmt.Println("Sending msg: ", i)
		msg := CreateMsg(i, 32)
		client.socket.Write(msg)
		time.Sleep(3 * time.Second)
	}*/
}

//main
func main() {
	flagMode := flag.String("mode", "server", "start in client or server mode")
	flagProto := flag.String("proto", "udp", "use udp or tcp")
	flag.Parse()
	if strings.ToLower(*flagMode) == "client" {
		switch strings.ToLower(*flagProto) {
		case "tcp":
			soc.StartClient("tcp")
		default:
			soc.StartClient("udp")
		}
	} else {
		switch strings.ToLower(*flagProto) {
		case "tcp":
			fmt.Println("Starting tcp server..")
			soc.StartServerMode("tcp")
		default:
			//fmt.Println("Starting udp server..")
			soc.StartServerMode("udp")
		}

	}
}
