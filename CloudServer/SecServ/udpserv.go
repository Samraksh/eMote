package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strings"
)

type UDPManager struct {
	/*clients    map[*UDPClient]bool
	broadcast  chan []byte
	register   chan *UDPClient
	unregister chan *UDPClient*/
	conn net.PacketConn
	data   chan []byte
}
/*
type UDPClient struct {
	socket net.PacketConn
	data   chan []byte
}*/


func (manager *UDPManager) createMsg (int msgid, int msglen) ([]byte)  {
	sendmsg := make([]byte,msglen)
	x:=10+1
	return sendmsg
}

func (manager *UDPManager) parseMsg (byte[] data) int {
	x:=10
	return x
}

func (manager *UDPManager) udpreceive() {
	for {
		message := make([]byte, 4096)
		length, addr, err := conn.ReadFrom(message)
		if err != nil {
			//manager.unregister <- client
			//conn.Close()
			break
		}
		if length > 0 {
			fmt.Println("RECEIVED: " + string(message))
			//manager.broadcast <- message
			int msgid = 10
			int msglen = length
			//parseMsg(message)
			createMsg(10, length)
		}
	}
}

func (manager *UDPManager) udpsend() {
	defer client.socket.Close()
	for {
		select {
		case message, ok := <-client.data:
			if !ok {
				return
			}
			client.socket.Write(message)
		}
	}
}


func StartUDPServer(int port) {
	fmt.Println("Starting UDPserver...")
	pc, error := net.ListenPacket("udp", ":"+ strconv.Itoa(port) )
	if error != nil {
		fmt.Println(error)
	}
	manager := UDPManager{
		conn:    pc,
		data:  make(chan []byte),
	}
	//go manager.start()
	for {
		/*connection, _ := listener.Accept()
		if error != nil {
			fmt.Println(error)
		}
		client := &Client{socket: connection, data: make(chan []byte)}
		manager.register <- client
		*/
		go manager.udpreceive()
		//go manager.send(client)
	}
}
