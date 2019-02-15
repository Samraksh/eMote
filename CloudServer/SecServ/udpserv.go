package main

import (
	//"bufio"
	"fmt"
	"net"
	"strconv"
	//"os"
	//"strings"
	"encoding/binary"
	"time"
)

/*
type UDPManager struct {
	//clients    map[*UDPClient]bool
	//broadcast  chan []byte
	//register   chan *UDPClient
	//unregister chan *UDPClient
	conn net.PacketConn
	data   chan []byte
}



func (manager *UDPManager) udpreceive() {
	for {
		message := make([]byte, 4096)
		length, addr, err := manager.conn.ReadFrom(message)
		if err != nil {
			//manager.unregister <- client
			//conn.Close()
			break
		}
		if length > 0 {
			fmt.Println("RECEIVED: " + string(message))
			//manager.broadcast <- message
			//msgid := 10
			//msglen := length
			//parseMsg(message)
			manager.createMsg(10, length)
			manager.udpsend(addr);
		}
	}
}

func (manager *UDPManager) udpsend(destAddr net.Addr) {
	//defer manager..Close()
	for {
		select {
		case message, ok := <-manager.data:
			if !ok {
				return
			}
			manager.conn.WriteTo(message,destAddr)
		}
	}
}


func StartUDPServer(port int) {
	fmt.Println("Starting UDPserver...on port ",port)
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
		go manager.udpreceive()
		//go manager.send(client)
	}
}
*/



func createMsg (msgid, msglen int) ([]byte)  {
	sendmsg := make([]byte,msglen)
	sendmsg=[]byte("Hello Client: "+strconv.Itoa(msgid))
	//sendmsg+= 
	return sendmsg
}

func parseMsg (data []byte, msglen int) int {
	var bInt = data[13:]
	//fmt.Println("Received Msg of size: ",msglen, "Bytes: ",data[10],data[11],data[12],data[13],data[14],data[15],data[16], "Bytes: ", bInt)
	
	//var pInt []int
	//pInt :=&data[13]
	//x := binary.BigEndian.Uint32(bInt)
	x := binary.LittleEndian.Uint32(bInt)
	//int x:= strconv.Atoi(string(bInt))
	//fmt.Println("Received id: ",x)
	return int(x)
}


func StartUDPServer(port int) {
	ServerConn, _ := net.ListenUDP("udp", &net.UDPAddr{IP:[]byte{0,0,0,0},Port:port,Zone:""})
  	defer ServerConn.Close()
	buf := make([]byte, 1024)
	for {
		n, addr, _ := ServerConn.ReadFromUDP(buf)
		//fmt.Println("Received ", string(buf[0:n]), " from ", addr)

		msgid := parseMsg(buf[0:n],n);
		_msg := createMsg(msgid+1,n)
		fmt.Println("Received id: ",msgid," Sending response:  ",msgid+1 )
		ServerConn.WriteToUDP(_msg,addr)
		time.Sleep(3 * time.Millisecond)
		//ServerConn.WriteToUDP(_msg,addr)
	}
}

func StartUDPClient(port int) {
	fmt.Println("Starting UDP client to connect on port: ", port)
	Conn, _ := net.DialUDP("udp", nil, &net.UDPAddr{IP:[]byte{127,0,0,1},Port:port,Zone:""})
	defer Conn.Close()
	Conn.Write([]byte("hello"))
	fmt.Println("Done..")
}