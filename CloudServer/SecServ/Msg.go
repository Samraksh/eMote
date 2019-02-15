package main

import (
	//"bufio"
	//"fmt"
	//"net"
	"strconv"
	//"os"
	//"strings"
	"encoding/binary"
	//"time"
)


func CreateMsg (msgid, msglen int) ([]byte)  {
	sendmsg := make([]byte,msglen)
	sendmsg=[]byte("Hello Client: "+strconv.Itoa(msgid))
	//sendmsg+= 
	return sendmsg
}

func ParseMsg (data []byte, msglen int) int {
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
