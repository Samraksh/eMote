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
	case Def.M_SEC_M_CH, Def.M_SEC_D_CH, Def.M_SEC_STATUS_RQ, Def.M_SEC_STATUS_RES:
		hndlr.sc.HandleIncoming(msg, addr)
	case Def.M_STATUS_RES:
		if msg[1] == byte(dm.DS_UnInit) {
			log.Println("Device: ", addr, "status is not initialized")
			hndlr.InitiateSecureChannel(addr)
		} else {
			log.Printf("Unknown device status %d\n", msg[1])
		}
	case Def.M_UNKNOWN:
	default:
		fmt.Println("PktHndlr::RouteMsg: Received an unknown message type, droping it.")
	}
}
