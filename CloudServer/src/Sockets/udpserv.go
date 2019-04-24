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
	"time"
)

type UDPServer struct {
	addr     string
	port     int
	server   *net.UDPConn
	pktHndlr PacketHandler
}

func (u *UDPServer) Run() (err error) {

	laddr, err := net.ResolveUDPAddr("udp", u.addr)
	if err != nil {
		return errors.New("could not resolve UDP addr")
	}

	u.server, err = net.ListenUDP("udp", laddr)
	fmt.Println("Beginging to run on ", laddr)
	if err != nil {
		return errors.New("could not listen on UDP")
	}

	return u.handleConnections()
}

func (u *UDPServer) handleConnections() error {
	var err error
	fmt.Println("UDP Handle connections is running...")
	for {
		buf := make([]byte, 2048)
		n, conn, err := u.server.ReadFromUDP(buf)
		//fmt.Println("Got new pkt")
		if err != nil {
			log.Println(err)
			break
		}
		if conn == nil {
			continue
		}

		go u.handleConnection(conn, buf[:n])
	}
	return err
}

func (u *UDPServer) handleConnection(addr *net.UDPAddr, cmd []byte) {
	u.pktHndlr.Receive(cmd, addr.IP)
	//u.server.WriteToUDP([]byte(fmt.Sprintf("Request recieved: %s", cmd)), addr)
}

// Close ensures that the UDPServer is shut down gracefully.
func (u *UDPServer) Close() error {
	fmt.Println("Closing UDP Server.")
	return u.server.Close()
}

func StartUDPServer(port int) {
	fmt.Println("Staring the udp server..")
	udpServ := UDPServer{
		port: COM_PORT,
		addr: "localhost:" + strconv.Itoa(port),
	}

	go func() {
		udpServ.Run()
	}()
	time.Sleep(3 * time.Second)
}

/*
func InitUDPServer(port int) (*net.UDPConn, error) {
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
	Conn, err := net.DialUDP("udp", nil, &net.UDPAddr{IP: []byte{0, 0, 0, 0}, Port: port, Zone: ""})

	if err != nil {
		println("Unable to connect: ", err)
	}
	//Conn.Write([]byte("hello"))
	client := &Client{Socket: Conn}

	//defer Conn.Close()
	fmt.Println("Done..")
	return client
}
func StartUDPClientLocal(port int) *Client {
	_ip := []byte{127, 0, 0, 1}
	return StartUDPClient(port, _ip)
}
