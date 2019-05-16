#include "Ecdhp.h"
#include <crypto.h>

EcdhProto g_ServerEcdhp,g_ClientEcdhp;


BOOL NewEcdhProto(uint16_t eccsize, EcdhProto* dhp)  {

	hal_printf("NewEcdhProto::Creating new ECDH instance");
	if eccsize != 384 {
		return FALSE;
	}
	tp384 := elliptic.P384();
	tke := ecdh.Generic(tp384)
	tpriKey, tpubKey, terr := tke.GenerateKey(cr.Reader)

	if terr != nil {
		hal_printf("Failed to generate private/public key pair: %s\n", terr)
		return nil
	}
	//_rand := mr.New(mr.NewSource(987654312))

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
	//hal_printf("My public key:", dhp.MyPubKey)

}

///////////////////////////Request

//important: All elements need to be public(Capitalized) for Unmarshalling to work correctly.
type EcdhpRequestS struct {
	Msg            []byte
	Publickey      []byte
	Nonce          uint64
	Ecc_curve_size uint16_t
	SessionNo      uint16_t
}

func (dhp *EcdhProto) Initiate() []byte {
	var bmsg []byte
	g_ClientEcdhp = dhp
	dhp.SessionNo = uint16_t(dhp.MyRand.Intn(0xFFFF))
	dhp.Initiator = true
	bmsg = dhp.CreateRequest()
	hal_printf("No of active sessions: ", GSM.Length())
	GSM.Add(dhp.SessionNo)
	hal_printf("No of active sessions: ", GSM.Length())
	hal_printf("Initiating new ecdh protocol with session no: ", dhp.SessionNo)
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
	//hal_printf("Ecdh request struct: ", outS)

	b, err := binary.Marshal(outS)
	if err != nil {
		log.Fatal(err)
		return nil
	}
	b = append([]byte{Def.M_ECDH_REQ}, b...)
	//hal_printf("Size of marshalled output is : ", len(b), "Output : ", b)
	//hal_printf("Unmarshalled struct: ", inS)
	return b
}

/*type EcdhpReqAckS struct {
	Msg       []byte
	Publickey []byte
	Nonce     uint64
	//sessionNo      uint16_t
	Ecc_curve_size uint16_t
}

func (dhp *EcdhProto) ReqAck(sessionNo uint16_t, ecc_curve_size uint16_t, publickey []byte, msg [128]byte, nonce uint64) {

}*/

//////////////////////// Response

type EcdhpResponseS struct {
	HmacSha256   []byte
	Publickey    []byte
	Nonce        uint64
	EccCurveSize uint16_t
	SessionNo    uint16_t
}

// You have received a new request for ecdh with public key,
//Get the  public key from peer, create secretKey using ecdh,
//create hmac of msg+nonce, create response using all this, marshal and sendout
func (dhp *EcdhProto) CreateResponse(req *EcdhpRequestS) []byte {
	//Setup the ecdhp struct
	dhp.SessionNo = uint16_t(req.SessionNo)
	dhp.Initiator = false
	dhp.PutPeerPublicKey(req.Publickey)

	secretKey := dhp.ComputeSecret()
	//hal_printf("Key: ", secretKey)
	mac := hmac.New(sha256.New, secretKey)

	//Append the message and nonce
	nb := make([]byte, 8)
	binary.LittleEndian.PutUint64(nb, req.Nonce)
	m := append(req.Msg, nb...)

	//hal_printf("creating hash of msg of len: ", len(m), ", MSG:", m)
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

	b, _ := binary.Marshal(resp)

	b = append([]byte{Def.M_ECDH_RES}, b...)
	return b
}

///////////////////// Finalize or Terminate

type EcdhpFinalizeS struct {
	HmacSha256 []byte
	Result     bool
	//Publickey      []byte
	//Nonce          uint64
	SessionNo uint16_t
	//Ecc_curve_size uint16_t
}

///Create the Finalizeation message.
func (dhp *EcdhProto) CreateFinalize(res *EcdhpResponseS) []byte {

	if dhp.SessionNo != uint16_t(res.SessionNo) {
		log.hal_printf("Sessions numbers dont match, (mine)%d != %d(incoming)", dhp.SessionNo, res.SessionNo)
		return nil
	}
	dhp.Initiator = false

	dhp.PutPeerPublicKey(res.Publickey)
	//hal_printf("Peer public key:", dhp.PeerPubKey)
	secretKey := dhp.ComputeSecret()
	mac := hmac.New(sha256.New, secretKey)

	//hal_printf("Key: ", secretKey)
	//compute the hash and check if we are getting the same.
	nb := make([]byte, 8)
	binary.LittleEndian.PutUint64(nb, dhp.myNonce)

	m := append(dhp.myMsg, nb...)
	//hal_printf("creating hash of msg of len: ", len(m), ", MSG:", m)
	//create hmac
	mac.Write(m)
	hmac := mac.Sum(nil)
	_result := true
	if !bytes.Equal(hmac, res.HmacSha256) {
		//something wrong.
		hal_printf("My hmac is", hmac, "I received: ", res.HmacSha256)
		_result = false
	}

	hal_printf("My hmac is", hmac, "I received: ", res.HmacSha256)
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

	b, _ := binary.Marshal(conf)

	b = append([]byte{Def.M_ECDH_FIN}, b...)
	return b
}

/*func (dhp *EcdhProto) SendFinalize(res *EcdhpResponseS) {

}*/

///Other methods
func (dhp *EcdhProto) ComputeSecret1(peerPub crypto.PublicKey) []byte {
	if err := dhp.ke.Check(peerPub); err != nil {
		hal_printf("Peer's public key is not on the curve: %s\n", err)
		return nil
	}
	//hal_printf("ComputeSecret::Passed check.")
	dhp.secretKey = dhp.ke.ComputeSecret(dhp.priKey, peerPub)

	return dhp.secretKey
}

func (dhp *EcdhProto) ComputeSecret() []byte {
	//hal_printf("Peer public key:", dhp.PeerPubKey)
	if err := dhp.ke.Check(dhp.PeerPubKey); err != nil {
		hal_printf("Peer's public key is not on the curve: %s\n", err)
		return nil
	}
	//hal_printf("ComputeSecret::Passed check.")

	dhp.secretKey = dhp.ke.ComputeSecret(dhp.priKey, dhp.PeerPubKey)
	//hal_printf("Secret key\n %s\n", hex.Dump(dhp.secretKey))
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
func EcdhpStateMachine(msg []byte, dataC *chan Def.IPMsg, inaddr net.Addr) {
	var outB []byte
	ret := UnMarshall(msg)
	switch ret := ret.(type) {
	default:
		log.Fatal("Unknown struct is returned, something is terribly wrong")
	case *EcdhpRequestS:
		hal_printf("Got a request machine, checking session manager...")
		reqS := ret
		//if GSM.IsPresent(reqS.SessionNo) == -1 {
		//This is new session request, create a new
		hal_printf("EchdhpStateMachine: New Session request...")
		g_ServerEcdhp = NewEcdhProto(reqS.Ecc_curve_size)
		outB = g_ClientEcdhp.CreateResponse(reqS)
		//hal_printf("Created Response: ", outB)
		//} else {
		//	hal_printf("Already in session manager")
		//}

	//case EcdhpReqAckS:
	case *EcdhpResponseS:
		hal_printf("Got a response , checking session manager...")
		resS := ret
		if GSM.IsPresent(resS.SessionNo) == -1 {
			log.Print("Something wrong, got a ecdh response, but never sent a request for session: ", resS.SessionNo)
		} else {
			outB = g_ClientEcdhp.CreateFinalize(resS)
			//hal_printf("Created Finalizeation: ", outB)
		}
	case *EcdhpFinalizeS:
		hal_printf("Got a Finalize , checking session manager...")
		finS := ret
		if GSM.IsPresent(finS.SessionNo) == -1 {
			log.Print("Something wrong, got a ecdh response, but never sent a request for session: ", finS.SessionNo)
		} else {
			if finS.Result {
				hal_printf("Received a successful finalize message for session: ", finS.SessionNo)
				hal_printf("Session: ", finS.SessionNo, " is complete. Removing from session manager")
				GSM.Delete(finS.SessionNo)
			}
		}
	}
	//created the response, this needs to be sent out.
	if outB != nil {
		//hal_printf("Sending response back to socket")
		*dataC <- Def.IPMsg{
			Addr: inaddr,
			Msg:  outB,
		}
	}
}

//Unmarshals incoming messages
void UnMarshall(msg []byte) interface{} {
	//hal_printf("MSG: ", msg[1:])
	var retS interface{}
	switch msg[0] {
	case Def.M_ECDH_REQ:
		hal_printf("Got a ECDH_REQ msg")
		retS = &EcdhpRequestS{}

	case Def.M_ECDH_RES:
		hal_printf("Got a ECDH_RESPONSE msg")
		retS = &EcdhpResponseS{}
	case Def.M_ECDH_FIN:
		hal_printf("Got a ECDH_Finalize msg")
		retS = &EcdhpFinalizeS{}
	default:
		return nil
	}
	hal_printf("Unmarsharing..")
	err := binary.Unmarshal(msg[1:], retS)
	if err != nil {
		log.Fatal("Unmarshalling failed for ecdh_protocol : ", err)
	} else {
		//hal_printf(retS)
	}
	return retS
}
