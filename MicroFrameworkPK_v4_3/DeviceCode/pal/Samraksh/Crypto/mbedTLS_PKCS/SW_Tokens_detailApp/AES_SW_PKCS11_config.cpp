#include "../../mbedTLS_PKCS/SW_Tokens_detailApp/AES_SW_PKCS11.h"

static ICryptokiEncryption s_Encryption =
{
    AES_SW_PKCS11_Encryption::EncryptInit,
    AES_SW_PKCS11_Encryption::Encrypt,
    AES_SW_PKCS11_Encryption::EncryptUpdate,
    AES_SW_PKCS11_Encryption::EncryptFinal,
    AES_SW_PKCS11_Encryption::DecryptInit,
    AES_SW_PKCS11_Encryption::Decrypt,
    AES_SW_PKCS11_Encryption::DecryptUpdate,
    AES_SW_PKCS11_Encryption::DecryptFinal,
};

static ICryptokiDigest s_Digest = 
{
    AES_SW_PKCS11_Digest::DigestInit,
    AES_SW_PKCS11_Digest::Digest,
    AES_SW_PKCS11_Digest::Update,
    AES_SW_PKCS11_Digest::DigestKey,
    AES_SW_PKCS11_Digest::Final,
};

static ICryptokiSignature s_Sign = 
{
    AES_SW_PKCS11_Signature::SignInit,
    AES_SW_PKCS11_Signature::Sign,
    AES_SW_PKCS11_Signature::SignUpdate,
    AES_SW_PKCS11_Signature::SignFinal,
    
    AES_SW_PKCS11_Signature::VerifyInit,
    AES_SW_PKCS11_Signature::Verify,
    AES_SW_PKCS11_Signature::VerifyUpdate,
    AES_SW_PKCS11_Signature::VerifyFinal,
};

static ICryptokiKey s_Key =
{
    AES_SW_PKCS11_Keys::GenerateKey,
    AES_SW_PKCS11_Keys::GenerateKeyPair,
    AES_SW_PKCS11_Keys::WrapKey,
    AES_SW_PKCS11_Keys::UnwrapKey,
    AES_SW_PKCS11_Keys::DeriveKey,
    AES_SW_PKCS11_Keys::LoadSecretKey,
    AES_SW_PKCS11_Keys::LoadRsaKey,
    AES_SW_PKCS11_Keys::LoadDsaKey,
    AES_SW_PKCS11_Keys::LoadEcKey,
    AES_SW_PKCS11_Keys::LoadKeyBlob,
};

static ICryptokiObject s_Object = 
{
    AES_SW_PKCS11_Objects::CreateObject,
    AES_SW_PKCS11_Objects::CopyObject,
    AES_SW_PKCS11_Objects::DestroyObject,
    AES_SW_PKCS11_Objects::GetObjectSize,
    AES_SW_PKCS11_Objects::GetAttributeValue,
    AES_SW_PKCS11_Objects::SetAttributeValue,
    
    AES_SW_PKCS11_Objects::FindObjectsInit,
    AES_SW_PKCS11_Objects::FindObjects,
    AES_SW_PKCS11_Objects::FindObjectsFinal,
};

static ICryptokiRandom s_Random = 
{
    AES_SW_PKCS11_Random::SeedRandom,
    AES_SW_PKCS11_Random::GenerateRandom,
};

static ICryptokiSession s_Session = 
{
    AES_SW_PKCS11_Session::OpenSession,
    AES_SW_PKCS11_Session::CloseSession,
    AES_SW_PKCS11_Session::InitPin,
    AES_SW_PKCS11_Session::SetPin,
    AES_SW_PKCS11_Session::Login,
    AES_SW_PKCS11_Session::Logout,
};

static ICryptokiToken s_Token =
{
    AES_SW_PKCS11_Token::Initialize,
    AES_SW_PKCS11_Token::Uninitialize,
    AES_SW_PKCS11_Token::InitializeToken,
    AES_SW_PKCS11_Token::GetDeviceError,
};

CK_SLOT_INFO g_AES_SW_PKCS11_SlotInfo =
{
    "AES_SW_PKCS11_Crypto",
    "<ProviderNameHere>", 
    CKF_TOKEN_PRESENT, 
    { 0, 0 }, 
    { 1, 0 }
};

static CryptokiMechanism s_Mechanisms[] = 
{
    // Add your supported mechanisms here
    
    { CKM_SHA_1, { 160,  160, CKF_DIGEST } },
};

CryptokiToken g_AES_SW_PKCS11_Token =
{
    { // TOKEN INFO
        "<TokenLable>",
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

