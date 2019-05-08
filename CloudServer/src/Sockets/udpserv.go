package Sockets

import (
	//"bufio"
	"errors"
	"fmt"
	"log"
	"net"
	"strconv"
	//"os"
	//"strings"
	//"encoding/binary"
	Def "Definitions"
	"time"
)

type UDPSocketConn struct {
	addr      string
	port      int
	conn      *net.UDPConn
	outIPMsgC chan Def.IPMsg
	PktHndlr  PacketHandler
}

func (u *UDPSocketConn) Run() (err error) {

	laddr, err := net.ResolveUDPAddr("udp", u.addr)
	if err != nil {
		return errors.New("could not resolve UDP addr")
	}

	u.conn, err = net.ListenUDP("udp", laddr)

	fmt.Println("Beginging to run on ", laddr)
	if err != nil {
		return errors.New("could not listen on UDP")
	}
	u.PktHndlr.Socket = u.conn
	//start the outgoing thread
	go u.HandleOutgoing()
	go u.PktHndlr.StartOutGoing(&u.outIPMsgC)

	return u.handleConnections()
}

func (u *UDPSocketConn) HandleOutgoing() error {
	fmt.Println("UDP Server Outgoing hdlr running")
	for {
		select {
		case outM := <-u.outIPMsgC:
			fmt.Printf("Sending a message from udp server of length: %d \n", len(outM.Msg))
			_, err := u.conn.WriteTo(outM.Msg, outM.Addr)
			if err != nil {
				fmt.Println("Error sending msg back to client: ", err)
			}
			fmt.Println("msg sent to : ", outM.Addr)
		}
	}
}

func (u *UDPSocketConn) handleConnections() error {
	var err error
	fmt.Println("UDP Handle connections is running...")
	for {
		buf := make([]byte, 2048)
		//n, addr, err := u.conn.ReadFromUDP(buf)
		n, addr, err := u.conn.ReadFrom(buf)
		//fmt.Println("Got new pkt")
		if err != nil {
			log.Println(err)
			break
		}
		if addr == nil {
			continue
		}
		/*x, e := u.conn.WriteTo([]byte("hello"), addr)
		if e != nil {
			fmt.Println("Problem writting back to client: ", err)
		}
		fmt.Printf("Wrote %d bytes back to client", x)
		*/
		go u.handleConnection(addr, buf[:n])
	}
	return err
}

func (u *UDPSocketConn) handleConnection(addr net.Addr, cmd []byte) {
	u.PktHndlr.RouteMsg(cmd, addr)
	//u.server.WriteToUDP([]byte(fmt.Sprintf("Request recieved: %s", cmd)), addr)
}

// Close ensures that the UDPSocketConn is shut down gracefully.
func (u *UDPSocketConn) Close() error {
	fmt.Println("Closing UDP Server.")
	return u.conn.Close()
}

func StartUDPServer(port int) {
	fmt.Println("Staring the udp server..")
	udpServ := UDPSocketConn{
		port:      COM_PORT,
		addr:      "localhost:" + strconv.Itoa(port),
		outIPMsgC: make(chan Def.IPMsg),
		PktHndlr: PacketHandler{
			cryptoIPMsgC: make(chan Def.IPMsg),
			Server:       true,
		},
	}

	go func() {
		udpServ.Run()
	}()
	time.Sleep(1 * time.Second)
}

/*
func InitUDPSocketConn(port int) (*net.UDPConn, error) {
	serverConn, _ := net.ListenUDP("udp", &net.UDPAddr{IP: []byte{0, 0, 0, 0}, Port: port, Zone: ""})
	return serverConn, nil
}
func HandleUDPPKts(servConn *net.UDPConn) error {
	fmt.Println("Listening for incoming packets..")
	var pktHndlr = PacketHandler{}
	defer servConn.Close()
	buf := make([]byte, 1024)
	for {
		n, addr, err := servConn.ReadFromUDP(buf)
		if err != nil {
			fmt.Println(err)
			return err
		}

		//client := &Client{socket: Conn}

		go pktHndlr.Receive(buf[0:n], addr)
		//fmt.Println("Received ", string(buf[0:n]), " from ", addr)

		//msgid := ParseMsg(buf[0:n], n)
		//_msg := CreateMsg(msgid+1, n)
		//fmt.Println("Received id: ", msgid, " Sending response:  ", msgid+1)
		//ServerConn.WriteToUDP(_msg, addr)
		//time.Sleep(3 * time.Millisecond)
		//ServerConn.WriteToUDP(_msg,addr)
	}
}
*/

func StartUDPClient(port int, _ip []byte) *Client {

	udpAddr := &net.UDPAddr{IP: _ip, Port: port, Zone: ""}
	fmt.Println("Starting UDP client to connect on port: ", udpAddr)
	//Conn, err := net.DialUDP("udp", nil, udpAddr)
	conn, err := net.DialUDP("udp", nil, &net.UDPAddr{IP: []byte{0, 0, 0, 0}, Port: port, Zone: ""})

	if err != nil {
		println("Unable to connect: ", err)
	}
	//Conn.Write([]byte("hello"))
	client := &Client{
		//Socket: Conn,
		PktHndlr: PacketHandler{
			Socket:       conn,
			cryptoIPMsgC: make(chan Def.IPMsg),
			Server:       false,
		},
	}

	/*udpClient := UDPSocketConn{
		port:    COM_PORT,
		addr:    "localhost:" + strconv.Itoa(port),
		outChan: make(chan []byte),
		PktHndlr: PacketHandler{
			cryptoChannel: make(chan []byte),
		},
	}*/
	//defer Conn.Close()
	//go client.PktHndlr.Start()

	//Start handler for incomping
	go client.PktHndlr.StartIncomingHndlr()
	go client.PktHndlr.StartOutGoing(&client.PktHndlr.cryptoIPMsgC)
	fmt.Println("Done..")
	return client
}
func StartUDPClientLocal(port int) *Client {
	_ip := []byte{127, 0, 0, 1}
	return StartUDPClient(port, _ip)
}
