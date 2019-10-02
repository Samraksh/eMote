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
	"fmt"
	"log"
	"net/http"
	"net/rpc"
	svs "services"
	"strconv"
	"time"
)

const MaxPktSize = 256

//Gateway Main gateway service class.
type Gateway struct {
	core *rpc.Client
}

//Status RPC. Reports status of the service
func (g *Gateway) Status(req svs.GtwyStatusRequest, res *svs.GtwyStatusResponse) (err error) {
	if req.Name == "" || req.IP == "" {
		err = errors.New("Core name and IP address must be specified")
		return
	}

	g.core, err = rpc.DialHTTP("tcp", req.IP+":"+strconv.Itoa(svs.CorePort))
	if err != nil {
		log.Fatal("dialing:", err)
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
		g.sendToRadio(req.Message, req.Size)
	}
	res.Status = true
	return
}

func (g *Gateway) sendToRadio(msg []byte, size int) {
	log.Printf("Sending a message of size %d to radio\n", size)
	g.sendToCore(msg, size)
}

func (g *Gateway) sendToCore(msg []byte, size int) (err error) {
	// Synchronous call
	req := svs.MsgRequest{msg, len(msg)}
	var reply svs.MsgResponse

	if g.core != nil {
		//core connection is initialized
		err = g.core.Call("Core.Receive", req, &reply)
	} else {
		return errors.New("Core link does not exist ")
	}

	fmt.Printf("Core Sending Message Status: %d\n", reply.Status)
	return err
}

func main() {

	gateway := new(Gateway)
	rpc.Register(gateway)
	rpc.HandleHTTP()

	err := http.ListenAndServe(":"+strconv.Itoa(svs.GSPort), nil)
	if err != nil {
		fmt.Println(err.Error())
	}

	for {
		time.Sleep(time.Millisecond * 100)
	}
}
