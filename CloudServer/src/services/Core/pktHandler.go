package main

import (
	"fmt"

	Def "Definitions"
	dm "DeviceManager"
)

type PacketHandler struct {
	DeviceMsgChan chan Def.GenMsg
}

func (hndlr *PacketHandler) IncomingMsgHandler(msg []byte, addr string) {
	switch msg[0] {
	case Def.M_ECDH_REQ, Def.M_ECDH_RES, Def.M_ECDH_FIN:
		dm.EcdhpStateMachine(msg, &hndlr.DeviceMsgChan, addr)
	default:
		fmt.Println("PktHndlr::RouteMsg: Received an unknown message type, droping it.")
	}
}
