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
	"encoding/gob"

	//eb "encoding/binary"
	//"encoding/hex"
	"fmt"
	"log"
	mr "math/rand"

	"github.com/aead/ecdh"
	"github.com/kelindar/binary"

	//"time"
	Def "Definitions"
	"bytes"
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
	enc       *gob.Encoder
	//dec       *gob.Decoder
	//socClient *Sockets.Client
}

var g_ServerEcdhp *EcdhProto
var g_ClientEcdhp *EcdhProto
var outBytes bytes.Buffer // Stand-in for a network connection
var inBytes bytes.Buffer

func NewEcdhProto(eccsize uint16) *EcdhProto {

	fmt.Println("NewEcdhProto::Creating new ECDH instance")
	if eccsize != 384 {
		return nil
	}
	tp384 := elliptic.P384()
	tke := ecdh.Generic(tp384)
	tpriKey, tpubKey, terr := tke.GenerateKey(cr.Reader)

	if terr != nil {
		fmt.Printf("Failed to generate private/public key pair: %s\n", terr)
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

	dhp.enc = gob.NewEncoder(&outBytes) // Will write to network.
	//dhp.dec = gob.NewDecoder(&inBytes)

	//fmt.Println("My public key:", dhp.MyPubKey)
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

func (reqS *EcdhpRequestS) Marshal() []byte {
	n := make([]byte, 8)
	binary.LittleEndian.PutUint64(n, reqS.Nonce)
	s := make([]byte, 2)
	binary.LittleEndian.PutUint16(s, reqS.Ecc_curve_size)
	sn := make([]byte, 2)
	binary.LittleEndian.PutUint16(sn, reqS.SessionNo)
	ret := append(reqS.Msg, reqS.Publickey...)
	ret = append(ret, n...)
	ret = append(ret, s...)
	ret = append(ret, sn...)
	return ret
}

func (reqS *EcdhpRequestS) UnMarshal(b []byte) {
	copy(reqS.Msg, b[:127])          //97 bytes
	copy(reqS.Publickey, b[128:224]) //97 bytes

	reqS.Nonce = uint64(binary.LittleEndian.Uint64(b[225:232]))
	reqS.Ecc_curve_size = uint16(binary.LittleEndian.Uint16(b[233:234]))
	reqS.SessionNo = uint16(binary.LittleEndian.Uint16(b[235:236]))
}

func (dhp *EcdhProto) Initiate() []byte {
	var bmsg []byte
	g_ClientEcdhp = dhp
	dhp.SessionNo = uint16(dhp.MyRand.Intn(0xFFFF))
	dhp.Initiator = true
	bmsg = dhp.CreateRequest()
	fmt.Println("No of active sessions: ", GSM.Length())
	GSM.Add(dhp.SessionNo)
	fmt.Println("No of active sessions: ", GSM.Length())
	fmt.Println("Initiating new ecdh protocol with session no: ", dhp.SessionNo)
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
	fmt.Println("Ecdh request struct: ", outS)

	//b, err := binary.Marshal(outS)
	//dhp.enc.Encode(outS)
	//b := outBytes.Bytes()
	b := outS.Marshal()

	if b == nil {
		log.Fatal("Encoding failed")
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
	//pktType := []byte{Def.M_ECDH_REQ}
	b = append([]byte{Def.M_ECDH_REQ}, b...)
	fmt.Println("Size of marshalled output is : ", len(b), "Output : ", b)
	//fmt.Println("Unmarshalled struct: ", inS)
	return b
	//dhp.socClient.Write(b)
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
	HmacSha256   []byte
	Publickey    []byte
	Nonce        uint64
	EccCurveSize uint16
	SessionNo    uint16
}

func (reqS *EcdhpResponseS) Marshal() []byte {
	n := make([]byte, 8)
	binary.LittleEndian.PutUint64(n, reqS.Nonce)
	s := make([]byte, 2)
	binary.LittleEndian.PutUint16(s, reqS.EccCurveSize)
	sn := make([]byte, 2)
	binary.LittleEndian.PutUint16(sn, reqS.SessionNo)
	ret := append(reqS.HmacSha256, reqS.Publickey...)
	ret = append(ret, n...)
	ret = append(ret, s...)
	ret = append(ret, sn...)
	return ret
}

func (reqS *EcdhpResponseS) UnMarshal(b []byte) {
	var hmacSize int = 32
	var pkSize int = 97
	copy(reqS.HmacSha256, b[:hmacSize])
	copy(reqS.Publickey, b[hmacSize+1:hmacSize+pkSize])

	reqS.Nonce = uint64(binary.LittleEndian.Uint64(b[hmacSize+pkSize+1 : hmacSize+pkSize+8]))
	reqS.EccCurveSize = uint16(binary.LittleEndian.Uint16(b[hmacSize+pkSize+9 : hmacSize+pkSize+10]))
	reqS.SessionNo = uint16(binary.LittleEndian.Uint16(b[hmacSize+pkSize+11 : hmacSize+pkSize+12]))
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
	//fmt.Println("Key: ", secretKey)
	mac := hmac.New(sha256.New, secretKey)

	//Append the message and nonce
	nb := make([]byte, 8)
	binary.LittleEndian.PutUint64(nb, req.Nonce)
	m := append(req.Msg, nb...)

	//fmt.Println("creating hash of msg of len: ", len(m), ", MSG:", m)
	//create hmac
	mac.Write(m)
	hmac := mac.Sum(nil)

	//create response struct
	resp := EcdhpResponseS{
		HmacSha256:   hmac,
		SessionNo:    dhp.SessionNo,
		Publickey:    dhp.GetPublicKey(),
		Nonce:        dhp.myNonce,
		EccCurveSize: dhp.EccSize,
	}

	//b, _ := binary.Marshal(resp)
	dhp.enc.Encode(resp)
	b := outBytes.Bytes()

	b = append([]byte{Def.M_ECDH_RES}, b...)
	return b
}

///////////////////// Finalize or Terminate

type EcdhpFinalizeS struct {
	HmacSha256 []byte
	Result     bool
	//Publickey      []byte
	//Nonce          uint64
	SessionNo uint16
	//Ecc_curve_size uint16
}

func (reqS *EcdhpFinalizeS) Marshal() []byte {
	//n := make([]byte, 8)
	//binary.LittleEndian.PutUint64(n, reqS.Nonce)
	r := make([]byte, 1)
	if reqS.Result {
		r[0] = 1
	} else {
		r[0] = 0
	}
	sn := make([]byte, 2)
	binary.LittleEndian.PutUint16(sn, reqS.SessionNo)
	ret := append(reqS.HmacSha256, r...)
	ret = append(ret, sn...)
	return ret
}

func (fin *EcdhpFinalizeS) UnMarshal(b []byte) {
	var hmacSize int = 32
	//var pkSize int = 97
	copy(fin.HmacSha256, b[:hmacSize])
	//copy(reqS.Publickey, b[hmacSize+1:hmacSize+pkSize])

	fin.Result = (b[hmacSize+1] >= 1)
	fin.SessionNo = uint16(binary.LittleEndian.Uint16(b[hmacSize+2 : hmacSize+3]))
}

///Create the Finalizeation message.
func (dhp *EcdhProto) CreateFinalize(res *EcdhpResponseS) []byte {

	if dhp.SessionNo != uint16(res.SessionNo) {
		log.Printf("Sessions numbers dont match, (mine)%d != %d(incoming)", dhp.SessionNo, res.SessionNo)
		return nil
	}
	dhp.Initiator = false

	dhp.PutPeerPublicKey(res.Publickey)
	//fmt.Println("Peer public key:", dhp.PeerPubKey)
	secretKey := dhp.ComputeSecret()
	mac := hmac.New(sha256.New, secretKey)

	//fmt.Println("Key: ", secretKey)
	//compute the hash and check if we are getting the same.
	nb := make([]byte, 8)
	binary.LittleEndian.PutUint64(nb, dhp.myNonce)

	m := append(dhp.myMsg, nb...)
	//fmt.Println("creating hash of msg of len: ", len(m), ", MSG:", m)
	//create hmac
	mac.Write(m)
	hmac := mac.Sum(nil)
	_result := true
	if !bytes.Equal(hmac, res.HmacSha256) {
		//something wrong.
		fmt.Println("My hmac is", hmac, "I received: ", res.HmacSha256)
		_result = false
	}

	fmt.Println("My hmac is", hmac, "I received: ", res.HmacSha256)
	//create the final Finalizeation
	binary.LittleEndian.PutUint64(nb, res.Nonce)
	m = append(res.HmacSha256, nb...)

	//create hmac
	mac.Write(m)
	hmac = mac.Sum(nil)

	//create response struct
	conf := EcdhpFinalizeS{
		HmacSha256: hmac,
		SessionNo:  dhp.SessionNo,
		Result:     _result,
		//Publickey:      dhp.GetPublicKey(),
		//Nonce:          dhp.myNonce,
		//Ecc_curve_size: dhp.EccSize,
	}

	//b, _ := binary.Marshal(conf)
	dhp.enc.Encode(conf)
	b := outBytes.Bytes()

	b = append([]byte{Def.M_ECDH_FIN}, b...)
	return b
}

/*func (dhp *EcdhProto) SendFinalize(res *EcdhpResponseS) {

}*/

///Other methods
func (dhp *EcdhProto) ComputeSecret1(peerPub crypto.PublicKey) []byte {
	if err := dhp.ke.Check(peerPub); err != nil {
		fmt.Printf("Peer's public key is not on the curve: %s\n", err)
		return nil
	}
	//fmt.Println("ComputeSecret::Passed check.")
	dhp.secretKey = dhp.ke.ComputeSecret(dhp.priKey, peerPub)

	return dhp.secretKey
}

func (dhp *EcdhProto) ComputeSecret() []byte {
	//fmt.Println("Peer public key:", dhp.PeerPubKey)
	if err := dhp.ke.Check(dhp.PeerPubKey); err != nil {
		fmt.Printf("Peer's public key is not on the curve: %s\n", err)
		return nil
	}
	//fmt.Println("ComputeSecret::Passed check.")

	dhp.secretKey = dhp.ke.ComputeSecret(dhp.priKey, dhp.PeerPubKey)
	//fmt.Printf("Secret key\n %s\n", hex.Dump(dhp.secretKey))
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
func EcdhpStateMachine(msg []byte, dataC *chan Def.GenMsg, inaddr string) {
	var outB []byte
	ret := UnMarshall(msg)
	switch ret := ret.(type) {
	default:
		log.Fatal("Unknown struct is returned, something is terribly wrong")
	case *EcdhpRequestS:
		fmt.Println("Got a request machine, checking session manager...")
		reqS := ret
		//if GSM.IsPresent(reqS.SessionNo) == -1 {
		//This is new session request, create a new
		fmt.Println("EchdhpStateMachine: New Session request...")
		g_ServerEcdhp = NewEcdhProto(reqS.Ecc_curve_size)
		outB = g_ClientEcdhp.CreateResponse(reqS)
		//fmt.Println("Created Response: ", outB)
		//} else {
		//	fmt.Println("Already in session manager")
		//}

	//case EcdhpReqAckS:
	case *EcdhpResponseS:
		fmt.Println("Got a response , checking session manager...")
		resS := ret
		if GSM.IsPresent(resS.SessionNo) == -1 {
			log.Print("Something wrong, got a ecdh response, but never sent a request for session: ", resS.SessionNo)
		} else {
			outB = g_ClientEcdhp.CreateFinalize(resS)
			//fmt.Println("Created Finalizeation: ", outB)
		}
	case *EcdhpFinalizeS:
		fmt.Println("Got a Finalize , checking session manager...")
		finS := ret
		if GSM.IsPresent(finS.SessionNo) == -1 {
			log.Print("Something wrong, got a ecdh response, but never sent a request for session: ", finS.SessionNo)
		} else {
			if finS.Result {
				fmt.Println("Received a successful finalize message for session: ", finS.SessionNo)
				fmt.Println("Session: ", finS.SessionNo, " is complete. Removing from session manager")
				GSM.Delete(finS.SessionNo)
			}
		}
	}
	//created the response, this needs to be sent out.
	if outB != nil {
		//fmt.Println("Sending response back to socket")
		*dataC <- Def.GenMsg{
			Addr: inaddr,
			Msg:  outB,
		}
	}
}

//Unmarshals incoming messages
func UnMarshall(msg []byte) interface{} {
	//fmt.Println("MSG: ", msg[1:])
	var retS interface{}
	switch msg[0] {
	case Def.M_ECDH_REQ:
		fmt.Println("Got a ECDH_REQ msg")
		inS := EcdhpRequestS{}
		inS.UnMarshal(msg)
		retS = &inS
	case Def.M_ECDH_RES:
		fmt.Println("Got a ECDH_RESPONSE msg")
		inS := EcdhpResponseS{}
		inS.UnMarshal(msg)
		retS = &inS
	case Def.M_ECDH_FIN:
		fmt.Println("Got a ECDH_Finalize msg")
		inS := &EcdhpFinalizeS{}
		inS.UnMarshal(msg)
		retS = &inS
	default:
		return nil
	}
	fmt.Println("Unmarsharing..")

	/*dec := gob.NewDecoder(&inBytes)
	//err := binary.Unmarshal(msg[1:], retS)
	//err := dec.Decode(retS)
	if err != nil {
		log.Fatal("Unmarshalling failed for ecdh_protocol : ", err)
	} else {
		//fmt.Println(retS)
	}*/
	return retS
}
