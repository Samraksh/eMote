package Definitions

import (
	"net"
)

//type MsgType byte
const (
	M_UNKNOWN    byte = iota + 100
	M_STATUS_RQ       //Status request in open from core to device
	M_STATUS_RES      //response from device in open,
	M_ECDH_REQ        //ecdh req
	//M_ECDH_ACK
	//M_ECDH_TER
	M_ECDH_RES //ecdh response
	M_ECDH_FIN //ecdh finalize
	//M_SEC_M_CH
	/// All messages above are open messages. All messages below wil/ be handled by SecureComs module.
	M_SEC_D_CH       //Secure data message, meant for app
	M_SEC_STATUS_RQ  //state req from core to device
	M_SEC_STATUS_RES // reponse back from device
	M_SEC_BIN_RQ     //secure binary update request from device
	M_SEC_BIN_RES    // binary update protocol messages from core to device  or vise versa
	M_SEC_DETECT
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
