package Sockets

import (
	Cons "Constants"
	dm "DeviceManager"
	"fmt"
	"github.com/kelindar/binary"
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
		return -1
		fmt.Println("Error Writing to socket: ", err)
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
}

func (hndlr *PacketHandler) Receive(msg []byte, addr net.IP) {
	fmt.Println("Got a msg of size: ", len(msg), ", from IP: ", addr)
	UnMarshall(msg)
}

func UnMarshall(msg []byte) {

	switch msg[0] {
	case Cons.M_ECDH_REQ:
		fmt.Println("Got a ECDH_REQ msg")
		reqS := &dm.EcdhpRequestS{}
		//s := &s0{}
		//err := Unmarshal(s0b, s)
		fmt.Println("MSG: ", msg[1:])
		err := binary.Unmarshal(msg[1:], reqS)
		if err != nil {
			fmt.Println("Unmarshalling failed for ecdh_request: ", err)
		} else {
			fmt.Println("ECDH Request Struct: ", reqS)
		}

	case Cons.M_ECDH_ACK:
	case Cons.M_ECDH_RES:
	case Cons.M_ECDH_CON:
	case Cons.M_ECDH_TER:
	default:
	}
}
