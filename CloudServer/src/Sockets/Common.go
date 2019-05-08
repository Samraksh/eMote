package Sockets

import (
	Def "Definitions"
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
	Socket   net.Conn
	PktHndlr PacketHandler
	data     chan []byte
}

func (hndlr *PacketHandler) Write(buf []byte) int {
	x, err := hndlr.Socket.Write(buf)
	if err != nil {
		fmt.Println("Error Writing to socket: ", err)
		return -1
	}
	return x
}

func (hndlr *PacketHandler) WriteTo(buf []byte, addr net.Addr) int {
	x, err := hndlr.Socket.WriteTo(buf, addr)
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
	//Socket       net.Conn
	Socket       *net.UDPConn
	cryptoIPMsgC chan Def.IPMsg
	Server       bool
}

/*func (hndlr *PacketHandler) Receive(msg []byte, addr net.Addr) {
	fmt.Println("Got a msg of size: ", len(msg), ", from IP: ", addr)
	go hndlr.RouteMsg(msg, addr)
}
*/
func (hndlr *PacketHandler) RouteMsg(msg []byte, inaddr net.Addr) {
	switch msg[0] {
	case Def.M_ECDH_REQ, Def.M_ECDH_RES, Def.M_ECDH_FIN:
		dm.EcdhpStateMachine(msg, &hndlr.cryptoIPMsgC, inaddr)
	default:
		fmt.Println("PktHndlr::RouteMsg: Received an unknown message type, droping it.")
	}
}

func (hndlr *PacketHandler) StartIncomingHndlr() {
	buf := make([]byte, 1024)
	fmt.Println("Incoming handlr is running...")
	for {
		//udpConn, err := hndlr.Socket.(net.UDPConn)
		//fmt.Println("Reading..incoming..")
		n, addr, _ := hndlr.Socket.ReadFrom(buf)
		if n > 0 {
			//hndlr.Socket.
			fmt.Println("PktHndlr::IncomingHndlr: Got a message from server of size: ", n)
			hndlr.RouteMsg(buf[0:n], addr)
		}
	}
}

func (hndlr *PacketHandler) StartOutGoing(outC *chan Def.IPMsg) {
	fmt.Println("PktHdlr: Outgoing hndlr is running...")
	for {
		outM := <-hndlr.cryptoIPMsgC
		//fmt.Println("PktHdlr: Sending out a msg")
		if hndlr.Server {
			hndlr.WriteTo(outM.Msg, outM.Addr)
		} else {
			hndlr.Write(outM.Msg)
		}
		//*outC <- outM
	}
}
