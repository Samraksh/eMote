/*
Author: Mukundan Sridhran
The Samraksh Company
All Rights Reserved
*/

package DeviceManager

import (
	//"bytes"
	"crypto"
	"crypto/elliptic"
	cr "crypto/rand"
	//eb "encoding/binary"
	//"encoding/hex"
	"fmt"
	"github.com/aead/ecdh"
	"github.com/kelindar/binary"
	"log"
	mr "math/rand"
	//"time"
	Cons "Constants"
)

//cloud security manager implements the interfaces of the securitymanager

type EcdhProto struct {
	//public members
	PubKey crypto.PublicKey
	//PubKey []byte
	Msg    []byte
	MyRand *mr.Rand //random source
	//MyCurve elliptic.Curve

	//private members
	ke        ecdh.KeyExchange
	priKey    crypto.PrivateKey
	init      bool
	secretKey []byte
	//socClient *Sockets.Client
}

func NewEcdhProto() *EcdhProto {
	tp384 := elliptic.P384()
	tke := ecdh.Generic(tp384)
	tpriKey, tpubKey, terr := tke.GenerateKey(cr.Reader)
	if terr != nil {
		fmt.Printf("Failed to generate Alice's private/public key pair: %s\n", terr)
		return nil
	}

	dhp := EcdhProto{
		MyRand: mr.New(mr.NewSource(987654312)), //for testing needs to replace with a line
		//MyCurve: tp384,
		ke:     tke,
		priKey: tpriKey,
		PubKey: tpubKey,
		init:   true,
		//socClient: sCli,
	}
	return &dhp
}

//important: All elements need to be public(Capitalized) for Unmarshalling to work correctly.
type EcdhpRequestS struct {
	Msg       []byte
	Publickey []byte
	Nonce     uint64
	//sessionNo      uint16
	Ecc_curve_size uint16
}

func (dhp *EcdhProto) Request(_ecc_curve_size uint16, _publickey []byte, _msg []byte, _nonce uint64) []byte {

	outS := EcdhpRequestS{
		Msg:            _msg,
		Publickey:      _publickey,
		Ecc_curve_size: _ecc_curve_size,
		Nonce:          _nonce,
	}
	fmt.Println("Ecdh request struct: ", outS)

	b, err := binary.Marshal(outS)
	if err != nil {
		log.Fatal(err)
		return nil
	}
	var inS EcdhpRequestS
	err = binary.Unmarshal(b, &inS)
	if err != nil {
		log.Fatal(err)
		return nil
	}
	fmt.Println("Size of marshalled output is : ", len(b), "Output : ", b)
	//pktType := []byte{Cons.M_ECDH_REQ}
	b = append([]byte{Cons.M_ECDH_REQ}, b...)
	fmt.Println("Size of marshalled output is : ", len(b), "Output : ", b)
	fmt.Println("Unmarshalled struct: ", inS)
	return b
	//dhp.socClient.Write(b)
}

func (dhp *EcdhProto) ReqAck(sessionNo uint16, ecc_curve_size uint16, publickey []byte, msg [128]byte, nonce uint64) {

}

func (dhp *EcdhProto) Response(sessionNo uint16, msgHMAC [32]byte, hmacSize uint16, nonce uint64) {

}

func (dhp *EcdhProto) Confirm(sessionNo uint16, msgHMAC [32]byte, hmacSize uint16, nonce uint64) {

}

func (dhp *EcdhProto) Terminate(sessionNo uint16, errorCode uint8) {

}

func (dhp *EcdhProto) SendRequest(sessionNo uint16, ecc_curve_size uint16, publickey []byte, msg [128]byte, nonce uint64) {

}

func (dhp *EcdhProto) SendReqAck(sessionNo uint16, ecc_curve_size uint16, publickey []byte, msg [128]byte, nonce uint64) {

}

func (dhp *EcdhProto) SendResponse(sessionNo uint16, msgHMAC [32]byte, hmacSize uint16, nonce uint64) {

}

func (dhp *EcdhProto) SendConfirm(sessionNo uint16, msgHMAC [32]byte, hmacSize uint16, nonce uint64) {

}

func (dhp *EcdhProto) SendTerminate(sessionNo uint16, errorCode uint8) {

}

func (dhp *EcdhProto) StateMachine(msg []byte) {

}

func (dhp *EcdhProto) Initiate() []byte {
	nonce := dhp.MyRand.Uint64()
	eccsize := uint16(384)
	var msg [128]byte
	var bmsg []byte
	dhp.MyRand.Read(msg[:])
	pkp, ok := dhp.PubKey.(ecdh.Point)
	if ok {
		//copy(pubKey[:], elliptic.Marshal(elliptic.P384(), pkp.X, pkp.Y))
		pubKey := elliptic.Marshal(elliptic.P384(), pkp.X, pkp.Y)
		//fmt.Printf("Alice Public Key of size %d\n %s\n", len(pubKey), hex.Dump(pubKey))
		bmsg = dhp.Request(eccsize, pubKey, msg[:], nonce)
	} else {
		fmt.Println("Type Assertion failed: public key is not byte array")
	}
	return bmsg
}
