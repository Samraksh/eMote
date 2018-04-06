#include "BinInt.h"


bool VerifyIntegrity(CK_BYTE_PTR  pData, CK_BYTE_PTR pSig, SigType st, CK_KEY_TYPE kt,CK_BYTE_PTR key ){

}


bool ComputeHMAC(CK_BYTE_PTR  pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_MECHANISM_TYPE mtype, CK_KEY_TYPE kt, CK_BYTE_PTR key){
	CK_RV ret;
	CK_SESSION_HANDLE hSession;
	CK_MECHANISM_PTR  pMechanism;
	CK_ULONG pulDigestLen;

	if(mtype != CKM_SHA256_HMAC) {return false;}
	else {pulDigestLen=256;}

	ret= C_DigestInit (hSession,  pMechanism);
	if(ret != CKR_OK) return false;
	ret=C_Digest (hSession,  pData,ulDataLen,  pDigest,  &pulDigestLen);
	if(ret != CKR_OK) return false;
	return true;
}
