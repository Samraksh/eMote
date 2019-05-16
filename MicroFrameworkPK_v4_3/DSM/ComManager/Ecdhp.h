#ifndef _ECDHP_H_
#define _ECDHP_H_

#include <PKCS11\CryptokiPAL.h>
#include <drivers/mss_sys_services/mss_sys_services.h>

//enum KeyType{Symetric, PK };
//enum SigType{HMAC, SHA256};
enum UserType{UT_RoT, UT_Kernel, UT_App};


//cloud security manager implements the interfaces of the securitymanager
class EcdhProto {
	//ke        ecdh.KeyExchange
	char priKey[48];
	bool init;
	char secretKey[48];
	UINT64 myNonce;
	UINT64 peerNonce;

public:
	char MyPubKey[48];
	char PeerPubKey[48];
	bool Initiator;
	char myMsg[48];
	char peerMsg[48];
	//MyRand     *mr.Rand //random source
	UINT16 EccSize;
	UINT16 SessionNo;
};


#endif //_ECDHP_H_
