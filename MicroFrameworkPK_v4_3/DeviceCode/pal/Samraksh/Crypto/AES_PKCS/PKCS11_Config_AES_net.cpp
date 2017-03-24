#include <PAL\PKCS11\CryptokiPAL.h>

extern CK_SLOT_INFO  g_SF2_HW_SlotInfo;
extern CryptokiToken g_SF2_HW_Token;


CryptokiSlot  g_CryptokiSlots[] = 
{
    {
        &g_SF2_HW_SlotInfo,
        &g_SF2_HW_Token,
    },
};

const UINT32 g_CryptokiSlotCount = ARRAYSIZE(g_CryptokiSlots);


