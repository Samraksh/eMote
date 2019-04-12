package DeviceManager

import (
	//"bufio"
	"fmt"
	//"net"
	"strconv"
	//"os"
	//"strings"
	"encoding/binary"
	//"time"
)

type MsgType uint8

const (
	M_UNKNOWN MsgType = iota
	M_ECDH_REQ
	M_ECDH_ACK
	M_ECDH_RES
	M_ECDH_CON
	M_ECDH_TER
)

type Msg struct {
	msgType MsgType
	payload []byte
}

func NewMsg(payloadsize uint16, mtype MsgType) *Msg {
	msg := Msg{
		msgType: mtype,
		payload: make([]byte, payloadsize),
	}
	return &msg
}

func CreateMsg(msgid, msglen int) []byte {
	fmt.Println("Created Message of size: ", msglen)
	sendmsg := make([]byte, msglen)
	sendmsg = []byte("Hello Client: " + strconv.Itoa(msgid))
	//sendmsg+=
	fmt.Println("Message of size: ", len(sendmsg))
	return sendmsg
}

func ParseMsg(data []byte, msglen int) int {
	var bInt = data[13:]
	//fmt.Println("Received Msg of size: ",msglen, "Bytes: ",data[10],data[11],data[12],data[13],data[14],data[15],data[16], "Bytes: ", bInt)

	//var pInt []int
	//pInt :=&data[13]
	//x := binary.BigEndian.Uint32(bInt)
	x := binary.LittleEndian.Uint32(bInt)
	//int x:= strconv.Atoi(string(bInt))
	//fmt.Println("Received id: ",x)
	return int(x)
}
