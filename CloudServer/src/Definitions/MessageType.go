package Definitions

import (
	"net"
)

//type MsgType byte
const (
	M_UNKNOWN byte = iota + 100
	M_ECDH_REQ
	//M_ECDH_ACK
	M_ECDH_RES
	M_ECDH_FIN
	M_SEC_M_CH
	M_SEC_D_CH
	M_STATUS_RQ
	M_STATUS_RES
	M_SEC_STATUS_RQ
	M_SEC_STATUS_RES
	//M_ECDH_TER
)

type IPMsg struct {
	Msg  []byte
	Addr net.Addr
}

//
type GenMsg struct {
	Msg  []byte
	Addr string
}
