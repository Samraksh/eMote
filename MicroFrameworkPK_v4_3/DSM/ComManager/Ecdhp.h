#ifndef _ECDHP_H_
#define _ECDHP_H_

#include <PKCS11\CryptokiPAL.h>
#include <drivers/mss_sys_services/mss_sys_services.h>
#include "ComManager.h"

#define KEY_SIZE 384/8
#define HMAC_SIZE 256/8
#define MSG_SIZE 128

//enum KeyType{Symetric, PK };
//enum SigType{HMAC, SHA256};
enum UserType{UT_RoT, UT_Kernel, UT_App};

/* Note: This protocol uses uncompressed public key format (i.e. prefixed with a 0x04), however it is better to use compressed format to save space
 * We should move to that in future. This will save 48 bytes on the messages. The cloud-side golang implementation also needs to be fixed to do this.
 *
 * ECC publickeys are simply a point on a given curve. It is possible to compress ecc public keys,
 * becuase the curves are symmetric about the x-axis and each point on x-axis can possibly only have two y-values.
 * Therefore the compressed format uses simply the x-value, along with a byte to indicate the +ve or -ve y-value.
 *
 * For details about compressed and uncompressed formats use the following urls
 * https://learnmeabitcoin.com/glossary/public-key
 * https://bitcointalk.org/index.php?topic=644919.0
 */


//Exchange structures

//The Go marshaller on the cloud puts a size of each datafield in front of the field, hence we define the c structures along with the size fields.
//These size fields need to be filled out properly(i.e. the size in bytes of the member following it) for the unmarsheller on the Golang side to work properly.
typedef struct __attribute__ ((packed)){
	MsgTypeE type;
	INT16 sizeMsg;
	UINT8 Msg[MSG_SIZE];
	UINT8 sizePublickey;
	UINT8 Publickey[KEY_SIZE*2+1];
	//UINT8 sizeNonce;
	UINT64 Nonce;
	//UINT8 sizeEccSize;
	UINT16 EccSize;
	//UINT8 sizeSessionNo;
	UINT16 SessionNo;

	void PrepareTx(){
		type=M_ECDH_REQ;
		sizeMsg=MSG_SIZE;
		sizePublickey=KEY_SIZE*2;
		//sizeNonce=sizeof(UINT64);
		//sizeEccSize=sizeof(UINT16);
		//sizeSessionNo=sizeof(UINT16);
	}
}EcdhpRequestS;

//////////////////////// Response

typedef struct __attribute__ ((packed)){
	MsgTypeE type;
	UINT8 sizeHmac;
	UINT8 HmacSha256[HMAC_SIZE];
	UINT8 sizePublickey;
	UINT8 Publickey[KEY_SIZE*2+1];
	//UINT8 sizeNonce;
	UINT64 Nonce;
	//UINT8 sizeEccSize;
	UINT16 EccSize;
	//UINT8 sizeSessionNo;
	UINT16 SessionNo;

	void PrepareTx(){
		type=M_ECDH_RES;
		sizeHmac=HMAC_SIZE;
		sizePublickey=KEY_SIZE*2;
		//sizeNonce=sizeof(UINT64);
		//sizeEccSize=sizeof(UINT16);
		//sizeSessionNo=sizeof(UINT16);
	}
}EcdhpResponseS;



typedef struct __attribute__ ((packed)) {
	MsgTypeE type;
	UINT8 sizeHmac;
	UINT8 HmacSha256[HMAC_SIZE];
	//UINT8 sizeResult;
	bool Result;
	//UINT8 sizeSessionNo;
	UINT16 SessionNo;

	void PrepareTx(){
		type=M_ECDH_FIN;
		sizeHmac=HMAC_SIZE;
		//sizeResult=sizeof(bool);
		//sizeSessionNo=sizeof(UINT16);
	}
}EcdhpFinalizeS;




//cloud security manager implements the interfaces of the securitymanager
class EcdhProto {
public:
	UINT8 priKey[KEY_SIZE];
	bool init;
	UINT8 secretKey[KEY_SIZE];
	UINT64 myNonce;
	UINT64 peerNonce;
	UINT8 MyPubKey[KEY_SIZE*2];
	UINT8 PeerPubKey[KEY_SIZE*2];
	bool Initiator;
	UINT8 myMsg[128];
	UINT8 peerMsg[128];
	UINT16 EccSize;
	UINT16 SessionNo;

	bool Initiate(EcdhpRequestS* outStruct);
	bool ComputeSecret(UINT8 *secretKey);
	bool ComputeSecret1(UINT8 *peerPub, UINT8 *secretKey);
	bool CreateRequest(EcdhpRequestS* outStruct);
	bool CreateResponse(EcdhpRequestS* req, EcdhpResponseS* out);
	bool CreateFinalize(EcdhpResponseS* res, EcdhpFinalizeS* out);
	bool PutPeerPublicKey(UINT8 *publicKey, UINT16 size);
};


void PrintHex(uint8_t *x, uint16_t size){
	for (int i=0; i<size; i++){
		hal_printf("%X ", x[i]);
	}
	hal_printf("\n");
}

bool IsEqualHex(uint8_t *x, uint8_t *y, uint16_t size){
	for (int i=0; i<size; i++){
		if(x[i] != y[i]) return false;
	}
	return true;
}


#endif //_ECDHP_H_
