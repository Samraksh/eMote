package tests

import (
	dm "DeviceManager"
	"crypto/elliptic"
	"encoding/hex"
	"fmt"
	"github.com/aead/ecdh"
	"math/big"
	"testing"
)

func Test_ecdhProto(t *testing.T) {

	//Initiate a ecdh shared secret session
	var dhp *dm.EcdhProto = dm.NewEcdhProto()
	sn := uint16(dhp.MyRand.Uint32())
	nonce := dhp.MyRand.Uint64()
	eccsize := uint16(384)
	var msg [128]byte
	dhp.MyRand.Read(msg[:])
	pkp, ok := dhp.PubKey.(ecdh.Point)
	if ok {
		pubKey := elliptic.Marshal(elliptic.P384(), pkp.X, pkp.Y)
		fmt.Printf("Alice Public Key of size %d\n %s\n", len(pubKey), hex.Dump(pubKey))
		dhp.Request(sn, eccsize, pubKey[:], msg, nonce)
	} else {
		fmt.Println("Type Assertion failed: public key is not byte array")
	}

}

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
