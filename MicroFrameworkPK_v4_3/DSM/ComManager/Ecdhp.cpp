#include "Ecdhp.h"
#include <DeviceCode/Crypto/SF2_HW_Crypto.h>
#include <crypto.h>
#include "Sessions.h"
#include "ComManager.h"

EcdhProto g_ServerEcdhp,g_ClientEcdhp;
extern SessionManager GSM;


BOOL NewEcdhProto(uint16_t eccsize, EcdhProto* dhp)  {

	hal_printf("NewEcdhProto::Creating new ECDH instance");
	if (eccsize != 384) {
		return FALSE;
	}
	sf2_ecc_key_t newKey;
	if( SF2_ECC384_PKEY(&newKey, TRUE)!= CRYPTO_SUCCESS){
		return FALSE;
	}
	memcpy(dhp->MyPubKey,newKey.publicKey, KEY_SIZE*2);
	Crypto_GetRandomBytes((uint8_t*)dhp->myNonce,8);
	Crypto_GetRandomBytes(dhp->myMsg,128);
}

///////////////////////////Request

//returns the size of the msg
bool EcdhProto::Initiate(EcdhpRequestS* outStruct){
	//var bmsg []byte
	//g_ClientEcdhp = dhp
	SessionNo=Crypto_GetRandomUInt16();
	Initiator = true;
	CreateRequest(outStruct);
	hal_printf("No of active sessions: ", GSM.Length());
	if (!GSM.Add(SessionNo)) {return false;}
	hal_printf("No of active sessions: ", GSM.Length());
	hal_printf("Initiating new ecdh protocol with session no: ", SessionNo);
	return true;
}

bool EcdhProto::CreateRequest(EcdhpRequestS* outStruct) {
	outStruct->EccSize = this->EccSize;
	memcpy(outStruct->Msg, this->myMsg, KEY_SIZE);
	outStruct->Nonce= this->myNonce;
	memcpy(outStruct->Publickey, this->MyPubKey, KEY_SIZE);
	outStruct->SessionNo= this->SessionNo;
	return true;
}


// You have received a new request for ecdh with public key,
//Get the  public key from peer, create secretKey using ecdh,
//create hmac of msg+nonce, create response using all this, marshal and sendout
bool EcdhProto::CreateResponse(EcdhpRequestS* req, EcdhpResponseS* out) {
	//Setup the ecdhp struct
	this->SessionNo = UINT16(req->SessionNo);
	this->Initiator = false;
	PutPeerPublicKey(req->Publickey, 97);

	UINT8 secretKey[KEY_SIZE];
	this->ComputeSecret(secretKey);
	//hal_printf("Key: ", secretKey)

	UINT8 msgNonce[128+8];
	memcpy(msgNonce, req->Msg, 128);
	memcpy(msgNonce+128, (void*)req->Nonce, 8);

	//UINT8 hmac[32];
	if(Crypto_GetHMAC(msgNonce, 128+8, secretKey, out->HmacSha256, 32)!=0){
		return false;
	}

	//create response struct
	out->SessionNo = this->SessionNo;
	out->Nonce=this->myNonce;
	out->EccSize= this->EccSize;
	memcpy(&out->Publickey[1], this->MyPubKey, 96);
	out->Publickey[0]=0x4;
	out->PrepareTx();

	return true;
}

///////////////////// Finalize or Terminate


///Create the Finalizeation message.
bool EcdhProto::CreateFinalize(EcdhpResponseS* res, EcdhpFinalizeS* out){

	if (this->SessionNo != uint16_t(res->SessionNo)) {
		hal_printf("Sessions numbers dont match, (mine)%d != %d(incoming) \n", this->SessionNo, res->SessionNo);
		return false;
	}
	this->Initiator = false;

	PutPeerPublicKey(res->Publickey, 97);
	//hal_printf("Peer public key:", dhp.PeerPubKey)
	uint8_t secretKey[48];
	bool _result;
	this->ComputeSecret(secretKey);

	//hal_printf("Key: ", secretKey)
	UINT8 msgNonce[128+8];
	memcpy(msgNonce,this->myMsg, 128);
	memcpy(msgNonce+128, (void*)this->myNonce, 8);

	//UINT8 hmac[32];
	if(Crypto_GetHMAC(msgNonce, 128+8, secretKey, out->HmacSha256, 32)!=0){
		return false;
	}
	_result = true;
	if(!IsEqualHex(out->HmacSha256, res->HmacSha256,32)){
		//something wrong.
		hal_printf("My hmac is: "); PrintHex(out->HmacSha256,32);
		hal_printf("Hmac received: ");PrintHex(res->HmacSha256, 32);
		_result = false;
	}

	hal_printf("My hmac is: "); PrintHex(out->HmacSha256,32);
	hal_printf("Hmac received: ");PrintHex(res->HmacSha256, 32);

	out->Result=_result;
	out->SessionNo= this->SessionNo;

	out->PrepareTx();
	return _result;
}


bool EcdhProto::ComputeSecret1(UINT8 *peerPub, UINT8 *secretKey){
	if(Crypto_ECDH_ComputeSecret(this->EccSize, this->priKey, peerPub, secretKey)==0){
		hal_printf("ComputeSecret:: Passed.");
		return true;
	}
	return false;
}

bool EcdhProto::ComputeSecret(UINT8 *secretKey){
	if(Crypto_ECDH_ComputeSecret(this->EccSize, this->priKey, this->PeerPubKey, secretKey)==0){
		hal_printf("ComputeSecret:: Passed.");
		return true;
	}
	return false;
}

bool EcdhProto::PutPeerPublicKey(UINT8 *publicKey, UINT16 size) {
	if(size!=97){return false;}
	memcpy(this->PeerPubKey, &publicKey[1], 96);
	return true;
}






/////////////////////////////Main state machine for the protocol

//while any side, cloud or device, can initiate a new session, for the purpose of the protocol,
//the one who initiates is a client and uses the g_ClientEcdhp and the one who receives the request
//uses g_ServerEcdhp object.
void EcdhpStateMachine(UINT8 *msg, UINT8 size)
{
	void *outB;
	MsgTypeE outT;
	switch ((MsgTypeE)msg[0]) {
		case M_ECDH_REQ:
		{
			hal_printf("Got a Ecdhp request, checking session manager...");
			EcdhpRequestS *reqS = (EcdhpRequestS *)msg;
			EcdhpResponseS resS;
			NewEcdhProto(reqS->EccSize, &g_ServerEcdhp);
			g_ServerEcdhp.CreateResponse(reqS, &resS);
			resS.PrepareTx();
			outT=M_ECDH_RES;
			outB = (void*)&resS;
			break;
		}
		case M_ECDH_RES:
		{
			hal_printf("Got a Ecdgp response , checking session manager...\n");
			EcdhpResponseS *resS = (EcdhpResponseS *)msg;
			EcdhpFinalizeS finS;
			outT=M_ECDH_FIN;
			if (GSM.IsPresent(resS->SessionNo) == -1) {
				hal_printf("Ecdhp:: Statemachine: Something wrong, got a ecdh response, but never sent a request for session: \n", resS->SessionNo);
			} else {
				g_ClientEcdhp.CreateFinalize(resS, &finS);
			}
			outT=M_ECDH_FIN;
			outB = (void*)&finS;
			break;
		}
		case M_ECDH_FIN:
		{
			hal_printf("Got a Finalize , checking session manager...\n");
			EcdhpFinalizeS *finS = (EcdhpFinalizeS *)(&msg[1]);
			if (GSM.IsPresent(finS->SessionNo) == -1) {
				hal_printf("Something wrong, got a ecdh response, but never sent a request for session: \n", finS->SessionNo);
			} else {
				if (finS->Result) {
					hal_printf("Received a successful finalize message for session: \n", finS->SessionNo);
					hal_printf("Session: ", finS->SessionNo, " is complete. Removing from session manager\n");
					GSM.Delete(finS->SessionNo);
					return;
				}
			}
			break;
		}
		default:
				hal_printf("Unknown struct is returned, something is terribly wrong");
	}

	SendToSecurityServer(outB, outT);
}




/*
There is not much the marshaller is doing, except calling prepare, hence added this to the statemachine directly
and commented this out
bool Marshall(void *msg,  MsgTypeE outType, UINT8 *outMsg) {
	switch(outType) {
		case M_ECDH_REQ:
			hal_printf("Got a ECDH_REQ msg\n");
			EcdhpRequestS *retS = (EcdhpRequestS*)msg;
			retS->PrepareTx();
			break;
		case M_ECDH_RES:
			hal_printf("Got a ECDH_RESPONSE msg\n");
			EcdhpResponseS *retS = (EcdhpResponseS*)msg;
			retS->PrepareTx();
			break;
		case M_ECDH_FIN:
			hal_printf("Got a ECDH_Finalize msg\n");
			EcdhpFinalizeS *retS = (EcdhpFinalizeS*)msg;
			retS->PrepareTx();
			break;
		default:
			return false;
	}
	outMsg=(UINT8*)msg;
	hal_printf("Unmarsharing Done\n");
	return true;
}
*/

/*
bool UnMarshall(UINT8 *msg, UINT16 size, MsgTypeE *outType, void *outS) {
	switch (MsgTypeE(msg[0])) {
		case M_ECDH_REQ:
			hal_printf("Got a ECDH_REQ msg");
			EcdhpRequestS *retS = (EcdhpRequestS*)outS;
			break;
		case M_ECDH_RES:
			hal_printf("Got a ECDH_RESPONSE msg");
			EcdhpResponeS *retS = (EcdhpResponseS*)outS;
			break;
		case M_ECDH_FIN:
			hal_printf("Got a ECDH_Finalize msg");
			EcdhpFinalizeS *retS = (EcdhpFinalizeS*)outS;
			break;
		default:
			return false;
	}
	hal_printf("Unmarsharing Done\n");
	return true;
}
*/
