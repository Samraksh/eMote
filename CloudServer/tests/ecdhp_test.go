package tests

import (
	dm "DeviceManager"
	soc "Sockets"
	//"crypto/elliptic"
	//"encoding/hex"
	"fmt"
	//"github.com/aead/ecdh"
	//"math/big"
	"testing"
	"time"
)

func init() {

	soc.StartUDPServer(soc.COM_PORT)
	/*
		serv, err := soc.StartUDPServer(6001)
		if err != nil {
			t.Error("Starting udp server failed")
		}
		//important to call this a go func, otherwise the Test will get stuck
		go soc.HandleUDPPKts(serv)
	*/

	//time.Sleep(30 * time.Second)
}

func Test_ecdhProto_client(t *testing.T) {
	//t.Parallel()

	client := soc.StartClient("udp")

	//if client.Socket.
	//Initiate a ecdh shared secret session
	//var dhp *dm.EcdhProto = dm.NewEcdhProto(client)
	var dhp *dm.EcdhProto = dm.NewEcdhProto(384)
	bmsg := dhp.Initiate()
	if bmsg != nil {
		client.Write(bmsg)
	} else {
		fmt.Println("Test initiated successfully")
	}
	time.Sleep(10 * time.Second)
	defer fmt.Println("done")
}

/*
func TestOffCurve(t *testing.T) {
	p384 := elliptic.P384()
	x, y := new(big.Int).SetInt64(1), new(big.Int).SetInt64(1)
	if p384.IsOnCurve(x, y) {
		t.Errorf("FAIL: point off curve is claimed to be on the curve")
	}
	b := elliptic.Marshal(p384, x, y)
	x1, y1 := elliptic.Unmarshal(p384, b)
	if x1 != nil || y1 != nil {
		t.Errorf("FAIL: unmarshaling a point not on the curve succeeded")
	}
}
*/
