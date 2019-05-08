package Sockets

import (
	"net"
	"strconv"
	"strings"
)

//Creates a UDPAddr from a String address
func StringToUDPAddr(_addr string) net.UDPAddr {
	fields := strings.Split(_addr, ":")
	port, _ := strconv.Atoi(fields[1])
	ret := net.UDPAddr{
		IP:   net.IP(fields[0]),
		Port: port,
	}
	return ret
}
