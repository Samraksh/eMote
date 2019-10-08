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
	"os"
	svs "services"
	"strconv"
	"time"
)

const (
	//DGood Everything is good
	DGood = iota + 1
	//DUnInit Device uninitialized
	DUnInit
	//DNoKey Connected but session key is not setup
	DNoKey
)

//GatewayC Struct for storing the gateway client service information
type GatewayC struct {
	gtwy  *rpc.Client
	State int
}

//Core Main Core service class.
type Core struct {
	initialized bool
	deviceMap   map[string]GatewayC
	pktHndlr    *PacketHandler
}

//NewCore constructor for core
func NewCore() *Core {
	core := new(Core)
	core.deviceMap = make(map[string]GatewayC)
	return core
}

//Status RPC. Reports status of the service
func (cs *Core) Status(req svs.CoreStatusRequest, res *svs.CoreStatusResponse) (err error) {
	if req.Name == "" {
		err = errors.New("A Service name must be specified")
		return
	}

	res.Status = svs.CoreGood
	return
}

//Receive RPC. Receives incoming packets
func (cs *Core) Receive(req svs.MsgRequest, res *svs.MsgResponse) (err error) {
	//g.sendToRadio(req.Message, req.Size)
	log.Printf("Received a pkt from RG of size %d\n", req.Size)
	res.Status = true
	go cs.pktHndlr.IncomingMsgHandler(req.Message, req.Addr)
	return
}

//this function will not return if everything go well
func (cs *Core) startRPCServer() {
	err := http.ListenAndServe(":"+strconv.Itoa(svs.CorePort), nil)
	if err != nil {
		fmt.Println(err.Error())
	}
	log.Println("Core service up and running.")
}

//InitDevice Initialize a device by connecting to its gateway address
func (cs *Core) InitDevice(dtAddress string) (err error) {
	//Connect to gateway service
	fmt.Println("Connecting to gateway service at IP: ", dtAddress)

	gtwy, err := rpc.DialHTTP("tcp", dtAddress+":"+strconv.Itoa(svs.GSPort))
	if err != nil {
		log.Fatal("dialing:", err)
	}
	//insert the new gateway into the deviceMap
	gtwyC := GatewayC{gtwy, DUnInit}
	cs.deviceMap[dtAddress] = gtwyC

	// Synchronous call
	req := svs.GtwyStatusRequest{"Core", "localhost"}
	var reply svs.GtwyStatusResponse
	err = gtwy.Call("Gateway.Status", req, &reply)
	if err != nil {
		log.Fatal("Gateway Status error:", err)
		return err
	}
	fmt.Printf("Gateway Status: %s\n", svs.StatusLookup[reply.Status])

	if reply.Status == svs.GSGood {
		//update the status
		device := cs.deviceMap[dtAddress]
		device.State = DNoKey
		cs.deviceMap[dtAddress] = device
		//Start the periodic handler
		cs.InitializeSecureChannel(dtAddress)
	}
	return nil
}

//InitializeSecureChannel Initialize a secure channel by trigger key setup
func (cs *Core) InitializeSecureChannel(dtAddress string) {
	msg := []byte{'s', 'g', 'o', 'l', 'a', 'n', 'g'}
	cs.SendToGateway(msg, dtAddress)
}

//SendToGateway Sends a message to a already connected gateway through rpc
func (cs *Core) SendToGateway(msg []byte, dtAddress string) (err error) {
	// Synchronous call
	req := svs.MsgRequest{msg, len(msg), dtAddress}
	var reply svs.MsgResponse

	if val, ok := cs.deviceMap[dtAddress]; ok {
		//device connection is initialized
		err = val.gtwy.Call("Gateway.Send", req, &reply)
	} else {
		return errors.New("Device address does not exist in table")
	}
	fmt.Printf("Core Sending Message Status: %d\n", reply.Status)
	return err
}

func (cs *Core) OutGoingMsgHandler() {
	log.Println("Core: Outgoing hndlr is running...")
	for {
		outM := <-cs.pktHndlr.DeviceMsgChan
		//fmt.Println("PktHdlr: Sending out a msg")
		cs.SendToGateway(outM.Msg, outM.Addr)
	}
}

func main() {

	//First start the core service.
	core := NewCore()
	rpc.Register(core)
	rpc.HandleHTTP()

	dtAddress := "localhost"
	if len(os.Args) == 2 {
		dtAddress = os.Args[1]
		//os.Exit(1)
	} else {
		log.Println("No gateway IP provided.. using localhost")
	}

	//start a concurrent thread to lauch the rpc server
	go core.startRPCServer()

	//initialize the gateway device
	core.InitDevice(dtAddress)

	//start the core packethandler and the device manager
	core.pktHndlr = new(PacketHandler)
	go core.OutGoingMsgHandler()

	for {
		time.Sleep(time.Millisecond * 100)
	}
}
