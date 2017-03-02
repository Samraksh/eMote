#include <PAL\PKCS11\CryptokiPAL.h>

extern CK_SLOT_INFO  g_AES_Hardware_SlotInfo;
extern CryptokiToken g_AES_Hardware_Token;


CryptokiSlot  g_CryptokiSlots[] = 
{
    {
        &g_AES_Hardware_SlotInfo,
        &g_AES_Hardware_Token,
    },
};

const UINT32 g_CryptokiSlotCount = ARRAYSIZE(g_CryptokiSlots);


