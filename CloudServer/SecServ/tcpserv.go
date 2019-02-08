package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strings"
)

//reference
//https://golang.org/pkg/net/
//https://www.thepolyglotdeveloper.com/2017/05/network-sockets-with-the-go-programming-language/

//udp server
//http://www.minaandrawos.com/2016/05/14/udp-vs-tcp-in-golang/

const COM_PORTO = "udp"
const COM_PORT = "6001"

type TCPClientManager struct {
	clients    map[*TCPClient]bool
	broadcast  chan []byte
	register   chan *TCPClient
	unregister chan *TCPClient
}

type TCPClient struct {
	socket net.Conn
	data   chan []byte
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
		case message := <-manager.broadcast:
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

func (manager *TCPClientManager) receive(client *TCPClient) {
	for {
		message := make([]byte, 4096)
		length, err := client.socket.Read(message)
		if err != nil {
			manager.unregister <- client
			client.socket.Close()
			break
		}
		if length > 0 {
			fmt.Println("RECEIVED: " + string(message))
			manager.broadcast <- message
		}
	}
}

func (manager *TCPClientManager) send(client *TCPClient) {
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

func StartTCPServer(string) {
	fmt.Println("Starting server...")
	listener, error := net.Listen("tcp", ":12345")
	if error != nil {
		fmt.Println(error)
	}
	manager := TCPClientManager{
		clients:    make(map[*TCPClient]bool),
		broadcast:  make(chan []byte),
		register:   make(chan *TCPClient),
		unregister: make(chan *TCPClient),
	}
	go manager.start()
	for {
		connection, _ := listener.Accept()
		if error != nil {
			fmt.Println(error)
		}
		client := &TCPClient{socket: connection, data: make(chan []byte)}
		manager.register <- client
		go manager.receive(client)
		go manager.send(client)
	}
}

func (client *TCPClient) receive() {
	for {
		message := make([]byte, 4096)
		length, err := client.socket.Read(message)
		if err != nil {
			client.socket.Close()
			break
		}
		if length > 0 {
			fmt.Println("RECEIVED: " + string(message))
		}
	}
}

func StartClientMode() {
	fmt.Println("Starting client...")
	connection, error := net.Dial(COM_PORTO, "localhost:"+COM_PORT)
	if error != nil {
		fmt.Println(error)
	}
	client := &TCPClient{socket: connection}
	go client.receive()
	for {
		reader := bufio.NewReader(os.Stdin)
		message, _ := reader.ReadString('\n')
		connection.Write([]byte(strings.TrimRight(message, "\n")))
	}
}
