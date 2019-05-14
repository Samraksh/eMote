#include "Keystore.h"

//extern BOOL g_isCryptokiInitialized;


uint8_t  KeyStore_KeysEnrolled(uint8_t *noOfKeys){
	MSS_SYS_puf_get_number_of_keys(noOfKeys);
}


uint8_t KeyStore_KeysRemaining(){
	uint8_t noOfKeys=0;
	return totalKeys-KeyStore_KeysEnrolled(&noOfKeys);
}

//The last parameter is very important, its is were the key will be read back to
//consequently, its needs to be a global/static pointer in Ram that is allocated by the compiler
//If you pass something from the stack or a dynamically allocated value it will not
//work properly across reboots.
uint8_t KeyStore_EnrollKey(UserType ut, CK_BYTE_PTR  pKeyValue, CK_ULONG ulDataLen, CK_BYTE_PTR  permanentKeyPtr){
	uint8_t MSS_SYS_puf_enroll_key
	(
	    uint8_t key_number,
	    uint16_t key_size,
	    uint8_t* p_key_value,
	    uint8_t* p_key_location
	)
}


/*
bool VerifyIntegrity(CK_BYTE_PTR  pData, CK_BYTE_PTR pSig, SigType st, CK_KEY_TYPE kt,CK_BYTE_PTR key ){

}

bool InitCryptoki(){
	if (!g_isCryptokiInitialized) {
		CK_VOID_PTR pInitArgs;
		if(C_Initialize(pInitArgs)!=CKR_OK) return FALSE;
		Cryptoki_InitializeSession();

	}
	return TRUE;
}

CK_OBJECT_HANDLE CreateKey(CK_SESSION_HANDLE hSession, CK_KEY_TYPE kt, CK_BYTE_PTR key, CK_ULONG keyLen){

	CK_ATTRIBUTE attrib[4];

	attrib[0].type=CKA_CLASS;
	//uint coc=SwapEndian(CKO_SECRET_KEY);
	uint coc=CKO_SECRET_KEY;
	attrib[0].pValue=&coc;
	attrib[0].ulValueLen= sizeof(CK_OBJECT_CLASS);

	attrib[1].type=CKA_KEY_TYPE;
	attrib[1].pValue=&kt;
	attrib[1].ulValueLen= sizeof(CKA_KEY_TYPE);

	attrib[2].type=CKA_VALUE;
	attrib[2].pValue=key;
	attrib[2].ulValueLen= keyLen;

	CK_ATTRIBUTE_PTR pAttrib = (CK_ATTRIBUTE_PTR)attrib;

	CK_OBJECT_HANDLE hKey;
	CK_OBJECT_HANDLE_PTR phKey=&hKey;

	//create cyptoki key object
	CK_RV ret=C_CreateObject	(hSession,  pAttrib,  4,  phKey);
	if (ret==CKR_OK)
		return hKey;
	else return 0;
}

bool ComputeHMAC(CK_BYTE_PTR  pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_MECHANISM_TYPE mtype, CK_KEY_TYPE kt, CK_BYTE_PTR key, CK_ULONG keyLen){

	if(!InitCryptoki()) {hal_printf("ComputeHMAC: Cryptoki Init failed ");return false;}

	CK_RV ret;
	CK_SESSION_HANDLE hSession;
	//uint mechParam= 0x00030000;
	//CK_OBJECT_HANDLE mechParam;
	CK_MECHANISM mech;
	CK_ULONG pulDigestLen;

	if(mtype != CKM_SHA256_HMAC) {return false;}
	else {pulDigestLen=256;}

	//open a sessions
	ret=C_OpenSession(CK_SLOT_ID_INVALID+1, CKF_SERIAL_SESSION, NULL, NULL, &hSession);
	if(ret != CKR_OK) {hal_printf("ComputeHMAC: Opening session Init failed ");return false;}

	CK_OBJECT_HANDLE mechParam=CreateKey(hSession, kt, key, keyLen);
	mech.mechanism = mtype;
	mech.pParameter=(CK_VOID_PTR)&mechParam ;
	mech.ulParameterLen=sizeof(uint);
	CK_MECHANISM_PTR  pMechanism= &mech;


	ret= C_DigestInit (hSession,  pMechanism);
	if(ret != CKR_OK) {hal_printf("ComputeHMAC: digest Init failed ");return false;}
	ret=C_DigestUpdate (hSession,  pData,ulDataLen)  ;
	if(ret != CKR_OK) {hal_printf("ComputeHMAC: Update failed ");return false;}
	ret=C_DigestFinal(hSession, pDigest,  &pulDigestLen);
	if(ret != CKR_OK) {hal_printf("ComputeHMAC: Final failed ");return false;}
	return true;
}
*/
