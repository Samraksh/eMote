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
	dm "DeviceManager"
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
	log.Println("Pkt is: ", req.Message)
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
func (cs *Core) InitDevice(gtAddress string, deviceAddr string) (err error) {
	//Connect to gateway service

	log.Println("Will keep trying untill connection to gateway service at IP: ", gtAddress)
	var gtwy *rpc.Client
	for {
		gtwy, err = rpc.DialHTTP("tcp", gtAddress+":"+strconv.Itoa(svs.GSPort))
		if err != nil {
			//log.Println("InitDevice: dialing:", err)
		} else {
			//insert the new gateway into the deviceMap
			gtwyC := GatewayC{gtwy, DUnInit}
			cs.deviceMap[deviceAddr] = gtwyC
			break
		}
		time.Sleep(3 * time.Second)
	}
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
		device := cs.deviceMap[deviceAddr]
		device.State = DNoKey
		cs.deviceMap[deviceAddr] = device
		//Start the periodic handler
		//cs.InitializeSecureChannel(dtAddress)
	}
	return nil
}

//InitializeSecureChannel Initialize a secure channel by trigger key setup
func (cs *Core) InitializeSecureChannel(dtAddress string) {
	msg := []byte{0, 'g', 'o', 'l', 'a', 'n', 'g'} // first byte indicates open/secure
	cs.SendToGateway(msg, dtAddress, false)
}

//SendToGateway Sends a message to a already connected gateway through rpc
func (cs *Core) SendToGateway(msg []byte, dtAddress string, sec bool) (err error) {
	// Synchronous call
	req := svs.MsgRequest{msg, len(msg), dtAddress, sec}
	var reply svs.MsgResponse
	fmt.Printf("SendToGateway:: Sending to gateway  %s\n", dtAddress)
	if val, ok := cs.deviceMap[dtAddress]; ok {
		//device connection is initialized
		err = val.gtwy.Call("Gateway.Send", req, &reply)
	} else {
		err = errors.New("Device address does not exist in table")
	}
	fmt.Printf("SendToGateway:: Message Status: %t\n", reply.Status)
	return err
}

func (cs *Core) OutGoingSecMsgHandler() {
	log.Println("Core: Outgoing hndlr is running...")
	for {
		outM := <-cs.pktHndlr.DeviceSecMsgChan
		fmt.Println("OutGoingSecMsgHandler:: Sending out a msg")
		cs.SendToGateway(outM.Msg, outM.Addr, true)
	}
}

func (cs *Core) OutGoingOpenMsgHandler() {
	log.Println("Core: Outgoing hndlr is running...")
	for {
		outM := <-cs.pktHndlr.DeviceOpenMsgChan
		fmt.Println("OutGoingOpenMsgHandler:: Sending out a msg")
		cs.SendToGateway(outM.Msg, outM.Addr, false)
	}
}

func main() {

	//First start the core service.
	core := NewCore()
	rpc.Register(core)
	rpc.HandleHTTP()

	//Initialize the device manager and devices db
	dm.InitDB()

	gtAddress := "localhost"
	deviceAddr := "98:07:2D:37:DF:9B"
	if len(os.Args) == 2 {
		gtAddress = os.Args[1]
		//os.Exit(1)
	} else {
		log.Println("No gateway IP provided.. using localhost")
	}

	//start a concurrent thread to lauch the rpc server
	go core.startRPCServer()

	//keep trying untill connection to gateway
	go core.InitDevice(gtAddress, deviceAddr)

	//start the core packethandler and the device manager
	core.pktHndlr = NewPacketHandler()
	go core.OutGoingOpenMsgHandler()
	go core.OutGoingSecMsgHandler()

	for {
		time.Sleep(time.Millisecond * 100)
	}
}
