#include "SF2_HW_pkcs11.h"

static ICryptokiEncryption s_Encryption =
{
    SF2_HW_PKCS11_Encryption::EncryptInit,
    SF2_HW_PKCS11_Encryption::Encrypt,
    SF2_HW_PKCS11_Encryption::EncryptUpdate,
    SF2_HW_PKCS11_Encryption::EncryptFinal,
    SF2_HW_PKCS11_Encryption::DecryptInit,
    SF2_HW_PKCS11_Encryption::Decrypt,
    SF2_HW_PKCS11_Encryption::DecryptUpdate,
    SF2_HW_PKCS11_Encryption::DecryptFinal,
};

static ICryptokiDigest s_Digest = 
{
    SF2_HW_PKCS11_Digest::DigestInit,
    SF2_HW_PKCS11_Digest::Digest,
    SF2_HW_PKCS11_Digest::Update,
    SF2_HW_PKCS11_Digest::DigestKey,
    SF2_HW_PKCS11_Digest::Final,
};

static ICryptokiSignature s_Sign = 
{
    SF2_HW_PKCS11_Signature::SignInit,
    SF2_HW_PKCS11_Signature::Sign,
    SF2_HW_PKCS11_Signature::SignUpdate,
    SF2_HW_PKCS11_Signature::SignFinal,
    
    SF2_HW_PKCS11_Signature::VerifyInit,
    SF2_HW_PKCS11_Signature::Verify,
    SF2_HW_PKCS11_Signature::VerifyUpdate,
    SF2_HW_PKCS11_Signature::VerifyFinal,
};

static ICryptokiKey s_Key =
{
    SF2_HW_PKCS11_Keys::GenerateKey,
    SF2_HW_PKCS11_Keys::GenerateKeyPair,
    SF2_HW_PKCS11_Keys::WrapKey,
    SF2_HW_PKCS11_Keys::UnwrapKey,
    SF2_HW_PKCS11_Keys::DeriveKey,
    SF2_HW_PKCS11_Keys::LoadSecretKey,
    SF2_HW_PKCS11_Keys::LoadRsaKey,
    SF2_HW_PKCS11_Keys::LoadDsaKey,
    SF2_HW_PKCS11_Keys::LoadEcKey,
    SF2_HW_PKCS11_Keys::LoadKeyBlob,
};

static ICryptokiObject s_Object = 
{
    SF2_HW_PKCS11_Objects::CreateObject,
    SF2_HW_PKCS11_Objects::CopyObject,
    SF2_HW_PKCS11_Objects::DestroyObject,
    SF2_HW_PKCS11_Objects::GetObjectSize,
    SF2_HW_PKCS11_Objects::GetAttributeValue,
    SF2_HW_PKCS11_Objects::SetAttributeValue,
    
    SF2_HW_PKCS11_Objects::FindObjectsInit,
    SF2_HW_PKCS11_Objects::FindObjects,
    SF2_HW_PKCS11_Objects::FindObjectsFinal,
};

static ICryptokiRandom s_Random = 
{
    SF2_HW_PKCS11_Random::SeedRandom,
    SF2_HW_PKCS11_Random::GenerateRandom,
};

static ICryptokiSession s_Session = 
{
    SF2_HW_PKCS11_Session::OpenSession,
    SF2_HW_PKCS11_Session::CloseSession,
    SF2_HW_PKCS11_Session::InitPin,
    SF2_HW_PKCS11_Session::SetPin,
    SF2_HW_PKCS11_Session::Login,
    SF2_HW_PKCS11_Session::Logout,
};

static ICryptokiToken s_Token =
{
    SF2_HW_PKCS11_Token::Initialize,
    SF2_HW_PKCS11_Token::Uninitialize,
    SF2_HW_PKCS11_Token::InitializeToken,
    SF2_HW_PKCS11_Token::GetDeviceError,
};

CK_SLOT_INFO g_SF2_HW_PKCS11_SlotInfo =
{
    "SF2_HW_PKCS11_Crypto",
    "SmartFusion2 FPGA",
    CKF_TOKEN_PRESENT, 
    { 0, 0 }, 
    { 1, 0 }
};

static CryptokiMechanism s_Mechanisms[] = 
{
    // Add your supported mechanisms here
    
    //{ CKM_SHA_1, { 160,  160, CKF_DIGEST } },
	{ CKM_EC_KEY_PAIR_GEN      , { 256,  521, CKF_GENERATE_KEY_PAIR                            } },
	{ CKM_ECDSA                , { 256,  521,                             CKF_SIGN | CKF_VERIFY} },
	{ CKM_ECDH1_DERIVE         , { 256,  521, CKF_DERIVE                                       } },
    { CKM_GENERIC_SECRET_KEY_GEN,{   1, 3072, CKF_GENERATE                                     } },
	{ CKM_SHA256_HMAC          , { 256,  256, CKF_DIGEST                | CKF_SIGN | CKF_VERIFY} },
	{ CKM_SHA256               , { 256,  256, CKF_DIGEST                                       } },
	{ CKM_AES_CBC              , { 128,  256, CKF_ENCRYPT | CKF_DECRYPT                        } },
	{ CKM_AES_CBC_PAD          , { 128,  256, CKF_ENCRYPT | CKF_DECRYPT                        } },
	{ CKM_AES_ECB              , { 128,  256, CKF_ENCRYPT | CKF_DECRYPT                        } },
	{ CKM_AES_ECB_PAD          , { 128,  256, CKF_ENCRYPT | CKF_DECRYPT                        } },
	{ CKM_AES_KEY_GEN          , { 128,  256, CKF_GENERATE                                     } },

};

CryptokiToken g_SF2_HW_PKCS11_Token =
{
    { // TOKEN INFO
        "SF2",
        "<TokenManufacturer>",
        "<model>",
        "<serialNumber>",
        CKF_RNG | CKF_PROTECTED_AUTHENTICATION_PATH | CKF_TOKEN_INITIALIZED,
        10, // max session count
        0, // session count
        10, // max rw session count
        0, // rw session count
        0, // max pin len
        0, // min pin len
        1024, // Total public mem
        1024, // free public mem
        0, // total private mem
        0, // free private mem

        { 1, 0 }, // hardware version
        { 1, 0 }, // firmware version
        "201001010101000", // utc time
    },

    ROPublic,  // token wide session state
    {   // key wrapping mechanism
        CKM_AES_CBC,
        NULL,
        0
    },
    L"PinWrappingKey",
    4*1024,
    ARRAYSIZE(s_Mechanisms),
    s_Mechanisms,

    &s_Token,
    &s_Encryption,
    &s_Digest,
    &s_Sign,
    &s_Key,
    &s_Object,
    &s_Random,    
    &s_Session,
    NULL,  // persistent storage 
};

