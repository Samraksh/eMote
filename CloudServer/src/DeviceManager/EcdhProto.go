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
	"crypto/hmac"
	cr "crypto/rand"
	"crypto/sha256"
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
	MyPubKey   crypto.PublicKey
	PeerPubKey crypto.PublicKey
	Initiator  bool
	myMsg      []byte
	peerMsg    []byte
	MyRand     *mr.Rand //random source
	EccSize    uint16
	SessionNo  uint16
	//MyCurve elliptic.Curve

	//private members
	ke        ecdh.KeyExchange
	priKey    crypto.PrivateKey
	init      bool
	secretKey []byte
	myNonce   uint64
	peerNonce uint64
	//socClient *Sockets.Client
}

var g_CurrentEcdhp *EcdhProto

func NewEcdhProto(eccsize uint16) *EcdhProto {
	if eccsize != 384 {
		return nil
	}
	tp384 := elliptic.P384()
	tke := ecdh.Generic(tp384)
	tpriKey, tpubKey, terr := tke.GenerateKey(cr.Reader)
	if terr != nil {
		fmt.Printf("Failed to generate Alice's private/public key pair: %s\n", terr)
		return nil
	}
	_rand := mr.New(mr.NewSource(987654312))

	nonce := _rand.Uint64()
	var _msg [128]byte
	//var bmsg []byte
	_rand.Read(_msg[:])

	dhp := EcdhProto{
		MyRand: _rand, //for testing needs to replace with a line
		//MyCurve: tp384,
		EccSize:  eccsize,
		ke:       tke,
		priKey:   tpriKey,
		MyPubKey: tpubKey,
		init:     true,
		myNonce:  nonce,
		myMsg:    _msg[:],
		//socClient: sCli,
	}
	return &dhp
}

///////////////////////////Request

//important: All elements need to be public(Capitalized) for Unmarshalling to work correctly.
type EcdhpRequestS struct {
	Msg            []byte
	Publickey      []byte
	Nonce          uint64
	Ecc_curve_size uint16
	SessionNo      uint16
}

func (dhp *EcdhProto) Initiate() []byte {
	var bmsg []byte
	dhp.SessionNo = uint16(dhp.MyRand.Intn(0xFFFF))
	dhp.Initiator = true
	bmsg = dhp.CreateRequest()
	return bmsg
}

func (dhp *EcdhProto) CreateRequest() []byte {

	outS := EcdhpRequestS{
		Msg:            dhp.myMsg,
		Publickey:      dhp.GetPublicKey(),
		Ecc_curve_size: dhp.EccSize,
		Nonce:          dhp.myNonce,
		SessionNo:      dhp.SessionNo,
	}
	//fmt.Println("Ecdh request struct: ", outS)

	b, err := binary.Marshal(outS)
	if err != nil {
		log.Fatal(err)
		return nil
	}
	/*var inS EcdhpRequestS
	err = binary.Unmarshal(b, &inS)
	if err != nil {
		log.Fatal(err)
		return nil
	}
	*/
	//fmt.Println("Size of marshalled output is : ", len(b), "Output : ", b)
	//pktType := []byte{Cons.M_ECDH_REQ}
	b = append([]byte{Cons.M_ECDH_REQ}, b...)
	//fmt.Println("Size of marshalled output is : ", len(b), "Output : ", b)
	//fmt.Println("Unmarshalled struct: ", inS)
	return b
	//dhp.socClient.Write(b)
}

func (dhp *EcdhProto) RecvRequest(sessionNo uint16, ecc_curve_size uint16, publickey []byte, msg [128]byte, nonce uint64) {

}

/*type EcdhpReqAckS struct {
	Msg       []byte
	Publickey []byte
	Nonce     uint64
	//sessionNo      uint16
	Ecc_curve_size uint16
}

func (dhp *EcdhProto) ReqAck(sessionNo uint16, ecc_curve_size uint16, publickey []byte, msg [128]byte, nonce uint64) {

}*/

//////////////////////// Response

type EcdhpResponseS struct {
	HmacSha256     []byte
	Publickey      []byte
	Nonce          uint64
	Ecc_curve_size uint16
	SessionNo      uint16
}

func (dhp *EcdhProto) RecvResponse(res *EcdhpRequestS) {

}

// You have received a new request for ecdh with public key,
//Get the  public key from peer, create secretKey using ecdh,
//create hmac of msg+nonce, create response using all this, marshal and sendout
func (dhp *EcdhProto) CreateResponse(req *EcdhpRequestS) []byte {
	//Setup the ecdhp struct
	dhp.SessionNo = uint16(req.SessionNo)
	dhp.Initiator = false
	dhp.PutPeerPublicKey(req.Publickey)

	secretKey := dhp.ComputeSecret()
	mac := hmac.New(sha256.New, secretKey)

	//Append the message and nonce
	nb := make([]byte, 8)
	binary.LittleEndian.PutUint64(nb, req.Nonce)
	m := append(req.Msg, nb...)

	//create hmac
	mac.Write(m)
	hmac := mac.Sum(nil)

	//create response struct
	resp := EcdhpResponseS{
		HmacSha256:     hmac,
		SessionNo:      dhp.SessionNo,
		Publickey:      dhp.GetPublicKey(),
		Nonce:          dhp.myNonce,
		Ecc_curve_size: dhp.EccSize,
	}

	b, _ := binary.Marshal(resp)
	return b
}

///////////////////// Confirm or Terminate

type EcdhpConfirmS struct {
	Msg       []byte
	Publickey []byte
	Nonce     uint64
	//sessionNo      uint16
	Ecc_curve_size uint16
}

func (dhp *EcdhProto) Confirm(sessionNo uint16, msgHMAC [32]byte, hmacSize uint16, nonce uint64) {

}

func (dhp *EcdhProto) SendConfirm(res *EcdhpResponseS) {

}

///Other methods

func (dhp *EcdhProto) ComputeSecret() []byte {
	dhp.secretKey = dhp.ke.ComputeSecret(dhp.priKey, dhp.PeerPubKey)
	return dhp.secretKey
}

func (dhp *EcdhProto) GetPublicKey() []byte {
	var pubKey []byte
	pkp, ok := dhp.MyPubKey.(ecdh.Point)
	if ok {
		pubKey = elliptic.Marshal(elliptic.P384(), pkp.X, pkp.Y)
	}
	return pubKey
}

func (dhp *EcdhProto) PutPeerPublicKey(mpub []byte) {
	//var pubKey []byte
	x, y := elliptic.Unmarshal(elliptic.P384(), mpub)
	dhp.PeerPubKey = ecdh.Point{X: x, Y: y}
}

/////////////////////////////Main state machine for the protocol
func EcdhpStateMachine(msg []byte, c chan []byte) {
	var outB []byte
	ret := UnMarshall(msg)
	switch ret := ret.(type) {
	default:
		log.Fatal("Unknown struct is returned, something is terribly wrong")
	case *EcdhpRequestS:
		fmt.Println("Got a request machine, checking session manager...")
		reqS := ret
		if GSM.IsPresent(reqS.SessionNo) == -1 {
			//This is new session request, create a new
			fmt.Println("EchdhpStateMachine: New Session request...")
			g_CurrentEcdhp = NewEcdhProto(reqS.Ecc_curve_size)
			outB = g_CurrentEcdhp.CreateResponse(reqS)
			fmt.Println("Created Response: ", outB)
		} else {
			fmt.Println("Already in session manager")
		}

	//case EcdhpReqAckS:
	case *EcdhpResponseS:
	case *EcdhpConfirmS:
		//case EcdhpTerminateS:
	}
	//created the response, this needs to be sent out.
	if outB != nil {
		c <- outB
	}
}

//Unmarshals incoming messages
func UnMarshall(msg []byte) interface{} {
	fmt.Println("MSG: ", msg[1:])
	var retS interface{}
	switch msg[0] {
	case Cons.M_ECDH_REQ:
		fmt.Println("Got a ECDH_REQ msg")
		retS = &EcdhpRequestS{}
	/*case Cons.M_ECDH_ACK:
	fmt.Println("Got a ECDH_ACK msg")
	retS = EcdhpReqAckS{}
	*/
	case Cons.M_ECDH_RES:
		fmt.Println("Got a ECDH_RESPONSE msg")
		retS = EcdhpResponseS{}
	case Cons.M_ECDH_CON:
		fmt.Println("Got a ECDH_CONFIRM msg")
		retS = EcdhpConfirmS{}
	/*case Cons.M_ECDH_TER:
	fmt.Println("Got a ECDH_TERMINATE msg")
	retS = EcdhpTerminateS{}
	*/
	default:
		return nil
	}
	fmt.Println("Unmarsharing..")
	err := binary.Unmarshal(msg[1:], retS)
	if err != nil {
		log.Fatal("Unmarshalling failed for ecdh_protocol : ", err)
	} else {
		fmt.Println(retS)
	}
	return retS
}
