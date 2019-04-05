/*
Author: Mukundan Sridhran
The Samraksh Company
All Rights Reserved
*/

/*Security Manager Server Interface
This file defines the interface of the Security Manager Server, aka Mothership. The Device Security Manager is the counter
part to this which runs on the embedded device.
*/
package SecurityManager

//ErrorCode
const (
	ERR_UNKNOWN uint8 = 1 + iota
	ERR_UNSUP_CURVE
	ERR_UNSUP_MECH
)

/*
N1         N2
------------->KeyRequest
KeyRequestAck<------------
------------->KeyResponse
KeyConfirmation<----------
*/
type ecdh_proto interface {
	Request(sessionNo int, ecc_curve int, publickey byte, publicKeySize int, msg byte, nonce uint64)
	ReqAck(sessionNo int, ecc_curve int, publickey byte, publicKeySize int, msg byte, nonce uint64)
	Response(sessionNo int, msgHMAC byte, hmacSize int, nonce uint64)
	Confirm(sessionNo int, msgHMAC byte, hmacSize int, nonce uint64)
	Terminate(sessionNo int, publickey byte, publicKeySize int, errorCode uint8)
}
