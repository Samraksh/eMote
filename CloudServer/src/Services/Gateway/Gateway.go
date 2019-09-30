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
	"net/http"
	"net/rpc"
	svs "services"
	"strconv"
)

const MaxPktSize = 256

//Gateway Main gateway service class.
type Gateway struct{}

//Status RPC. Reports status of the service
func (g *Gateway) Status(req svs.GtwyStatusRequest, res *svs.GtwyStatusResponse) (err error) {
	if req.Name == "" {
		err = errors.New("A Service name must be specified")
		return
	}

	res.Status = svs.GSGood
	return
}

//Send RPC. Send a packet out on the network/radio interface
func (g *Gateway) Send(req svs.GtwySendRequest, res *svs.GtwySendResponse) (err error) {
	if req.Size > MaxPktSize {
		err = errors.New("Packet is bigger than max allowed packet on the radio/network")
		return
	}

	res.Status = svs.GSGood
	return
}

func main() {

	gateway := new(Gateway)
	rpc.Register(gateway)
	rpc.HandleHTTP()

	err := http.ListenAndServe(":"+strconv.Itoa(svs.GSPort), nil)
	if err != nil {
		fmt.Println(err.Error())
	}
}
