package Sockets

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
	//"time"
)

//reference
//https://golang.org/pkg/net/
//https://www.thepolyglotdeveloper.com/2017/05/network-sockets-with-the-go-programming-language/

//udp server
//http://www.minaandrawos.com/2016/05/14/udp-vs-tcp-in-golang/

type TCPClientManager struct {
	clients    map[*Client]bool
	receiveMsg chan []byte
	register   chan *Client
	unregister chan *Client
	msgCbk     MsgCallback
}

func (manager *TCPClientManager) start() {
	for {
		select {
		case connection := <-manager.register:
			manager.clients[connection] = true
			fmt.Println("Added new connection!")
		case connection := <-manager.unregister:
			if _, ok := manager.clients[connection]; ok {
				close(connection.data)
				delete(manager.clients, connection)
				fmt.Println("A connection has terminated!")
			}
		case message := <-manager.receiveMsg:
			for connection := range manager.clients {
				select {
				case connection.data <- message:
				default:
					close(connection.data)
					delete(manager.clients, connection)
				}
			}
		}
	}
}

func (manager *TCPClientManager) receive(client *Client) {
	for {
		message := make([]byte, 4096)
		length, err := client.Socket.Read(message)
		if err != nil {
			manager.unregister <- client
			client.Socket.Close()
			break
		}
		if length > 0 {
			fmt.Println("RECEIVED: " + string(message))
			manager.receiveMsg <- message

			//msgid := ParseMsg(message,length);
			//_msg := CreateMsg(msgid+1, length)
			//fmt.Println("Received id: ",msgid," Sending response:  ",msgid+1 )
			//client.Socket.Write(_msg)
			//time.Sleep(3 * time.Millisecond)
		}
	}
}

func (manager *TCPClientManager) send(client *Client) {
	defer client.Socket.Close()
	for {
		select {
		case message, ok := <-client.data:
			if !ok {
				return
			}
			client.Socket.Write(message)
		}
	}
}

func (manager *TCPClientManager) RegisterMsgCB(cb MsgCallback) {
	manager.msgCbk = cb
}

//Start the TCP server/client
func StartTCPServer(port int) (net.Listener, *TCPClientManager, error) {
	fmt.Println("Starting server...")
	listener, err := net.Listen("tcp", ":"+strconv.Itoa(port))
	if err != nil {
		fmt.Println(err)
		return nil, nil, err
	}
	manager := &TCPClientManager{
		clients:    make(map[*Client]bool),
		receiveMsg: make(chan []byte),
		register:   make(chan *Client),
		unregister: make(chan *Client),
	}
	go manager.start()
	return listener, manager, nil
}

func HanldeConnections(listener net.Listener, manager *TCPClientManager) {
	for {
		connection, err := listener.Accept()
		if err != nil {
			fmt.Println(err)
		}
		client := &Client{Socket: connection, data: make(chan []byte)}
		manager.register <- client
		//go manager.receive(client)
		//go manager.send(client)
	}

}

func (client *Client) receive() {
	for {
		message := make([]byte, 4096)
		length, err := client.Socket.Read(message)
		if err != nil {
			client.Socket.Close()
			break
		}
		if length > 0 {
			fmt.Println("RECEIVED: " + string(message))
		}
	}
}

func StartTCPClient(port int) *Client {
	//var pktHndlr = PacketHandler{}
	fmt.Println("Starting client...")
	connection, error := net.Dial("tcp", "localhost:"+strconv.Itoa(port))
	if error != nil {
		fmt.Println(error)
	}
	client := &Client{Socket: connection}
	go client.receive()
	for {
		reader := bufio.NewReader(os.Stdin)
		message, _ := reader.ReadString('\n')
		connection.Write([]byte(strings.TrimRight(message, "\n")))
	}
	return client
}
