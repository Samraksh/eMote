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
)

//Core Main Core service class.
type Core struct{}

//Status RPC. Reports status of the service
func (g *Core) Status(req svs.CoreStatusRequest, res *svs.CoreStatusResponse) (err error) {
	if req.Name == "" {
		err = errors.New("A Service name must be specified")
		return
	}

	res.Status = svs.CoreGood
	return
}

//this function will not return if everything go well
func startRPCServer() {
	err := http.ListenAndServe(":"+strconv.Itoa(svs.CorePort), nil)
	if err != nil {
		fmt.Println(err.Error())
	}
}

func main() {

	//First start the core service.
	core := new(Core)
	rpc.Register(core)
	rpc.HandleHTTP()

	//start a concurrent thread to lauch the rpc server
	go startRPCServer()

	serverAddress := "localhost"
	if len(os.Args) == 2 {
		serverAddress = os.Args[1]
		//os.Exit(1)
	} else {
		fmt.Println("No gateway IP provided.. using localhost")
	}

	//Connect to gateway service
	fmt.Println("Connecting to gateway service at IP: ", serverAddress)
	gtwy, err := rpc.DialHTTP("tcp", serverAddress+":"+strconv.Itoa(svs.GSPort))
	if err != nil {
		log.Fatal("dialing:", err)
	}

	// Synchronous call
	req := svs.GtwyStatusRequest{"Core"}
	var reply svs.GtwyStatusResponse
	err = gtwy.Call("Gateway.Status", req, &reply)
	if err != nil {
		log.Fatal("Gateway Status error:", err)
	}
	fmt.Printf("Gateway Status: %s\n", svs.StatusLookup[reply.Status])
	/*
		var quot Quotient
		err = gtwy.Call("Arith.Divide", args, &quot)
		if err != nil {
			log.Fatal("arith error:", err)
		}
		fmt.Printf("Arith: %d/%d=%d remainder %d\n", args.A, args.B, quot.Quo, quot.Rem)
	*/
}
