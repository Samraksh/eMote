package Definitions

import (
	"net"
)

//type MsgType byte
const (
	M_UNKNOWN byte = iota
	M_ECDH_REQ
	//M_ECDH_ACK
	M_ECDH_RES
	M_ECDH_FIN
	//M_ECDH_TER
)

type IPMsg struct {
	Msg  []byte
	Addr net.Addr
}
