package Sockets

import (
	Cons "Constants"
	dm "DeviceManager"
	"fmt"
	//"github.com/kelindar/binary"
	"net"
)

var COM_PORTO string = "udp"
var COM_PORT int = 6001
var COM_IP = []byte{127, 0, 0, 1}

type MsgCallback = func(msg []byte, addr net.Addr)

type Client struct {
	Socket net.Conn
	data   chan []byte
}

func (cli *Client) Write(buf []byte) int {
	x, err := cli.Socket.Write(buf)
	if err != nil {
		fmt.Println("Error Writing to socket: ", err)
		return -1
	}
	return x
}

func StartClient(mode string) *Client {
	var client *Client
	if mode == "tcp" {
		client = StartTCPClient(COM_PORT)
	} else {
		client = StartUDPClient(COM_PORT, COM_IP)
	}
	return client
}

func StartServerMode(mode string) {
	if mode == "udp" {
		StartUDPServer(COM_PORT)
	} else {
		StartTCPServer(COM_PORT)
	}
}

type PacketHandler struct {
	cryptoChannel chan []byte
}

func (hndlr *PacketHandler) Receive(msg []byte, addr net.IP) {
	fmt.Println("Got a msg of size: ", len(msg), ", from IP: ", addr)
	go hndlr.RouteMsg(msg)
}

func (hndlr *PacketHandler) RouteMsg(msg []byte) {

	switch msg[0] {
	case Cons.M_ECDH_REQ, Cons.M_ECDH_ACK, Cons.M_ECDH_RES, Cons.M_ECDH_CON, Cons.M_ECDH_TER:
		dm.EcdhpStateMachine(msg, hndlr.cryptoChannel)
	default:
	}
}

func (hndlr *PacketHandler) Start(outC chan []byte) {
	for {
		select {
		case outM := <-hndlr.cryptoChannel:
			outC <- outM
		}
	}
}
