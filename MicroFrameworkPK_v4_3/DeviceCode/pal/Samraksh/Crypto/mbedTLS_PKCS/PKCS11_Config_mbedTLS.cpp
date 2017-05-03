#include <PAL\PKCS11\CryptokiPAL.h>

extern CK_SLOT_INFO  g_mbedTLS_SlotInfo;
extern CryptokiToken g_mbedTLS_Token;


CryptokiSlot  g_CryptokiSlots[] = 
{
    {
        &g_mbedTLS_SlotInfo,
        &g_mbedTLS_Token,
    },
};

const UINT32 g_CryptokiSlotCount = ARRAYSIZE(g_CryptokiSlots);


