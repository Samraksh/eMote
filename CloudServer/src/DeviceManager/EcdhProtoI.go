/*
Author: Mukundan Sridhran
The Samraksh Company
All Rights Reserved
*/

/*Security Manager Server Interface
This file defines the interface of the Security Manager Server, aka Mothership. The Device Security Manager is the counter
part to this which runs on the embedded device.
*/
package DeviceManager

//StateCode
const (
	ERR_UNKNOWN uint8 = 1 + iota
	ERR_UNSUP_CURVE
	ERR_UNSUP_MECH
	TERMINATE //Remote sent terminate
	REQ_COM   //Request completed
	ACK_COM   //Ack completed
	RES_COM   //Response completed
	SUCCESS   //Protocol is successful and dont
)

/* ECDH Protocol description

N1         N2
------------->KeyRequest
KeyRequestAck<------------
------------->KeyResponse
KeyConfirmation<----------
*/
type EcdhProtoI interface {
	Request(ecc_curve uint16, publickey []byte, msg [128]byte, nonce uint64)
	ReqAck(sessionNo uint16, ecc_curve uint16, publickey []byte, msg [128]byte, nonce uint64)
	Response(sessionNo uint16, msgHMAC [32]byte, hmacSize uint16, nonce uint64)
	Confirm(sessionNo uint16, msgHMAC [32]byte, hmacSize uint16, nonce uint64)
	Terminate(sessionNo uint16, errorCode uint8)

	//StateMachine()
}
