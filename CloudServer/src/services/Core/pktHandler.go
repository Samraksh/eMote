package main

import (
	Def "Definitions"
	dm "DeviceManager"
	"fmt"
	"log"
)

type PacketHandler struct {
	DeviceMsgChan chan Def.GenMsg
	sc            *SecureComs
}

func (hndlr *PacketHandler) Init() {
	hndlr.sc = new(SecureComs)
}

func (hndlr *PacketHandler) InitiateSecureChannel(device string) {
	var dhp *dm.EcdhProto = dm.NewEcdhProto(384)
	bmsg := dhp.Initiate()
	if bmsg != nil {
		hndlr.DeviceMsgChan <- Def.GenMsg{
			Addr: device,
			Msg:  bmsg,
		}
		log.Println("Ecdh initiated with device: ", device)
	}
}

func (hndlr *PacketHandler) IncomingMsgHandler(msg []byte, addr string) {
	switch msg[0] {
	case Def.M_ECDH_REQ, Def.M_ECDH_RES, Def.M_ECDH_FIN:
		dm.EcdhpStateMachine(msg, &hndlr.DeviceMsgChan, addr)
	case Def.M_SEC_COM:
		hndlr.sc.HandleIncoming(msg, addr)
	case Def.M_UNKNOWN:
		hndlr.InitiateSecureChannel(addr)
	default:
		fmt.Println("PktHndlr::RouteMsg: Received an unknown message type, droping it.")
	}
}
