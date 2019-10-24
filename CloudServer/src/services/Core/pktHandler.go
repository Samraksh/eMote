package main

import (
	Def "Definitions"
	dm "DeviceManager"
	"bytes"
	"fmt"
	"log"
)

type PacketHandler struct {
	DeviceSecMsgChan  chan Def.GenMsg
	DeviceOpenMsgChan chan Def.GenMsg
	sc                *SecureComs
}

func NewPacketHandler() *PacketHandler {
	hndlr := new(PacketHandler)
	hndlr.DeviceSecMsgChan = make(chan Def.GenMsg, 2)
	hndlr.DeviceOpenMsgChan = make(chan Def.GenMsg, 2)
	hndlr.sc = NewSecureComs(&hndlr.DeviceSecMsgChan)
	return hndlr
}

func (hndlr *PacketHandler) InitiateSecureChannel(device string) {
	var dhp *dm.EcdhProto = dm.NewEcdhProto(384)
	bmsg := dhp.Initiate()
	if bmsg != nil {
		hndlr.DeviceOpenMsgChan <- Def.GenMsg{
			Addr: device,
			Msg:  bmsg,
		}
		log.Println("Ecdh initiated with device: ", device, "Msg size: ", len(bmsg))
	}
}

func (hndlr *PacketHandler) OpenMsgHandler(msg []byte, addr string) {
	switch msg[0] {
	case Def.M_ECDH_REQ, Def.M_ECDH_RES, Def.M_ECDH_FIN:
		dm.EcdhpStateMachine(msg, &hndlr.DeviceOpenMsgChan, addr)
	case Def.M_SEC_D_CH, Def.M_SEC_STATUS_RQ, Def.M_SEC_STATUS_RES:
		hndlr.sc.HandleIncoming(msg, addr)
	case Def.M_STATUS_RES:
		if msg[1] == byte(dm.DS_UnInit) {
			log.Println("Device: ", addr, "status is not initialized")
			//hndlr.InitiateSecureChannel(addr)
		} else {
			log.Printf("Unknown device status %d\n", msg[1])
		}
	case Def.M_UNKNOWN:
	default:
		fmt.Println("PktHndlr::OpenMsgHandler: Received an unknown message type, droping it.")
	}
}
func (hndlr *PacketHandler) IncomingMsgHandler(msg []byte, addr string) {
	switch t := msg[0]; {
	case t == 77: //These are binary update messages starting with (MSpktV1)
		header := []byte("MSpktV1")
		if bytes.Compare(msg[:7], header) == 0 {
			log.Println("Received a binary update message")
			hndlr.sc.binaryUpdateHandler(msg, addr)
		} else {
			log.Println("Received a message starting with M: ", string(msg[:7]))
		}
	case t < byte(Def.M_SEC_STATUS_RES):
		hndlr.OpenMsgHandler(msg, addr)
	case t >= byte(Def.M_SEC_STATUS_RES):
		hndlr.sc.HandleIncoming(msg, addr)
	default:
		fmt.Println("PktHndlr::IncomingMsgHandler: Received an unknown message type, droping it.")
	}
}
