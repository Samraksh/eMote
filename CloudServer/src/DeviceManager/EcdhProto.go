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
	mr "math/rand"
	//"encoding/hex"
	"fmt"
	"github.com/aead/ecdh"
	//"time"
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
	}
	return &dhp
}

func (dhp *EcdhProto) Request(sessionNo uint16, ecc_curve_size uint16, publickey []byte, msg [128]byte, nonce uint64) {

}

func (dhp *EcdhProto) ReqAck(sessionNo uint16, ecc_curve_size uint16, publickey []byte, msg [128]byte, nonce uint64) {

}

func (dhp *EcdhProto) Response(sessionNo uint16, msgHMAC [32]byte, hmacSize uint16, nonce uint64) {

}

func (dhp *EcdhProto) Confirm(sessionNo uint16, msgHMAC [32]byte, hmacSize uint16, nonce uint64) {

}

func (dhp *EcdhProto) Terminate(sessionNo uint16, errorCode uint8) {

}

func (dhp *EcdhProto) StateMachine(msg *Msg) {

}
