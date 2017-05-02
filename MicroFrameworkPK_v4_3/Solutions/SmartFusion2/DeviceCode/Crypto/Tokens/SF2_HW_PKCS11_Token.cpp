//#include <objects/objects.h>
//#include <EVP/evp.h>
//#include <Crypto/crypto.h>
#include "SF2_HW_PKCS11.h"

CK_RV SF2_HW_PKCS11_Token::Initialize()
{
    //SF2_HW_add_all_algorithms();

    return CKR_OK;
}
CK_RV SF2_HW_PKCS11_Token::Uninitialize()
{
   // CRYPTO_cleanup_all_ex_data();
    //EVP_cleanup();
    
    return CKR_OK;
}

CK_RV SF2_HW_PKCS11_Token::InitializeToken(CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen, CK_UTF8CHAR_PTR pLabel, CK_ULONG ulLabelLen)
{
    SF2_HW_PKCS11_Objects::IntitializeObjects();
    
    return CKR_OK;
}

CK_RV SF2_HW_PKCS11_Token::GetDeviceError(CK_ULONG_PTR pErrorCode)
{
    *pErrorCode = 0;
    return CKR_OK;
}
    

