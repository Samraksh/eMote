package main

/*
Simple go rpc based services. Checkout the requirements of what methods can be made an rpc.
shortly they are as follows.
The method’s type is exported.
The method is exported
The method has two arguments, both exported (or builtin types).
The method’s second argument is a pointer
The method has return type error
*/

import (
	"errors"
	"flag"
	"fmt"
	"log"
	"net/http"
	"net/rpc"
	svs "services"
	"strconv"
	"strings"
	"time"
)

const MaxPktSize = 256

//Gateway Main gateway service class.
type Gateway struct {
	core        *rpc.Client
	radio       *BtGattRadio
	clientID    string
	coreIP      string
	secRcvChan  chan []byte
	openRcvChan chan []byte
}

func NewGateway(coreIP string) *Gateway {
	//instantiate gateway service*rpc.Client
	g := new(Gateway)
	g.secRcvChan = make(chan []byte, 3)
	g.openRcvChan = make(chan []byte, 3)
	g.coreIP = coreIP
	return g
}

func (g *Gateway) dialCore(servIP string) (err error) {
	g.core, err = rpc.DialHTTP("tcp", servIP+":"+strconv.Itoa(svs.CorePort))
	return err
}

//Status RPC. Reports status of the service
func (g *Gateway) Status(req svs.GtwyStatusRequest, res *svs.GtwyStatusResponse) (err error) {
	if req.Name == "" || req.IP == "" {
		err = errors.New("Core name and IP address must be specified")
		return
	}
	err = g.dialCore(req.IP)
	if err != nil {
		return err
	}
	res.Status = svs.GSGood
	return
}

//Send RPC. Send a packet out on the network/radio interface
func (g *Gateway) Send(req svs.MsgRequest, res *svs.MsgResponse) (err error) {
	if req.Size > MaxPktSize {
		err = errors.New("Packet is bigger than max allowed packet on the radio/network")
		return
	} else {
		g.sendToRadio(req.Message, req.Size, req.Secure)
	}
	res.Status = true
	return
}

func (g *Gateway) sendToRadio(msg []byte, size int, secChannel bool) {
	log.Printf("Sending a message of size %d to radio\n", size)
	if secChannel {
		g.radio.SendSecure(msg, size)
	} else {
		g.radio.SendOpen(msg, size)
	}
}

func (g *Gateway) sendToCore(msg []byte, size int) (err error) {
	// Synchronous call
	req := svs.MsgRequest{msg, len(msg), g.clientID, false}
	var reply svs.MsgResponse

	if g.core == nil {
		if g.dialCore(g.coreIP) != nil {
			return errors.New("Core link has failed!! ")
		}
		log.Println("Core link not present, redail successful.")
	}
	//core connection is initialized
	if g.core.Call("Core.Receive", req, &reply) != nil {
		return errors.New("Core link is unstable!! ")
	}

	fmt.Printf("Core Sending Message Status: %t\n", reply.Status)
	return err
}

func (g *Gateway) openReceive() {
	for {
		//log.Println("Waiting for open channel msges ")
		msg := <-g.openRcvChan
		log.Println("Received a open msg from gateway of length: ", len(msg))
		err := g.sendToCore(msg, len(msg))
		if err != nil {
			log.Println("Core link is not available")
			//g.openRcvChan <- msg
		}
	}
}

func (g *Gateway) secReceive() {
	for {
		msg := <-g.secRcvChan
		log.Println("Received a secure msg from gateway of length: ", len(msg))
		err := g.sendToCore(msg, len(msg))
		if err != nil {
			log.Println("Core link is not available")
			//g.secRcvChan <- msg
		}
	}
}
func startRPCServer() {
	err := http.ListenAndServe(":"+strconv.Itoa(svs.GSPort), nil)
	if err != nil {
		fmt.Println(err.Error())
	}
}

func (g *Gateway) SendStatusReq() {
	log.Println("Gateway: Sending a status request to device")
	msg := []byte{101, 1, 2, 3, 4, 5, 6, 7}
	g.radio.SendOpen(msg, 8)
}
func main() {
	//Parse the client radio ID
	flag.Parse()
	var clientID string
	if len(flag.Args()) != 1 {
		clientID = "98:07:2D:37:DF:9B"
		//log.Fatalf("usage: %s [options] peripheral-id\n", os.Args[0])
		log.Println("Client ID not supplied. Trying default client ", clientID)
	} else {
		clientID = strings.ToUpper(flag.Args()[0])
	}

	//instantiate new gateway
	gateway := NewGateway("localhost")
	//instantiate new radio module
	gateway.radio = NewBtGattRadio(clientID, gateway.secRcvChan, gateway.openRcvChan)

	gateway.clientID = clientID
	//get the rpc server up and running
	rpc.Register(gateway)
	rpc.HandleHTTP()
	go startRPCServer()

	//Start receiving threads
	go gateway.openReceive()
	go gateway.secReceive()

	//instialise the radio
	gateway.radio.InitConn()

	//Send a status req
	time.Sleep(5 * time.Second) //wait for 5 secs for discovery to happen
	gateway.SendStatusReq()

	//Wait for events
	for {
		time.Sleep(time.Millisecond * 100)
	}
}
