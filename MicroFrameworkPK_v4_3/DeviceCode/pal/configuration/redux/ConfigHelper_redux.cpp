////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <Samraksh/ID_decl.h>

//--//

BOOL HAL_CONFIG_BLOCK::IsGoodBlock() const
{
    return TRUE;
}

BOOL HAL_CONFIG_BLOCK::IsGoodData() const
{
    return TRUE;
}

BOOL HAL_CONFIG_BLOCK::IsGood() const
{
    return TRUE;
}

const HAL_CONFIG_BLOCK* HAL_CONFIG_BLOCK::Next() const
{
    return NULL;
}

const void* HAL_CONFIG_BLOCK::Data() const
{
    return NULL;
}

//--//

BOOL HAL_CONFIG_BLOCK::Prepare( const char* Name, void* Data, UINT32 Size )
{
    return TRUE;
}

//--//

const HAL_CONFIG_BLOCK* HAL_CONFIG_BLOCK::Find( const char* Name, BOOL fSkipCurrent, BOOL fAppend ) const
{
    return NULL;
}

//--//

BOOL HAL_CONFIG_BLOCK::GetConfigSectorAddress(HAL_CONFIG_BLOCK_STORAGE_DATA& blData)
{
    return FALSE;
}

BOOL HAL_CONFIG_BLOCK::CompactBlock(HAL_CONFIG_BLOCK_STORAGE_DATA& blData, const ConfigurationSector* cfgStatic, const HAL_CONFIG_BLOCK* cfgEnd)
{
    return FALSE;
}


BOOL HAL_CONFIG_BLOCK::UpdateBlock( const HAL_CONFIG_BLOCK_STORAGE_DATA &blData, const void* pAddress, const HAL_CONFIG_BLOCK *Header, void* Data, size_t Length, const void* LastConfigAddress, BOOL isChipRO )
{
    return FALSE;
}

BOOL HAL_CONFIG_BLOCK::UpdateBlockWithName( const char* Name, void* Data, size_t Length, BOOL isChipRO )
{
    return FALSE;
}

BOOL HAL_CONFIG_BLOCK::ApplyConfig( const char* Name, void* Address, size_t Length )
{
    return FALSE;
}
BOOL HAL_CONFIG_BLOCK::ApplyConfig( const char* Name, void* Address, size_t Length, void** newAlloc )
{
    return FALSE;
}

unsigned int /* ie, BOOL */ GetHalSystemInfo(HalSystemInfo& systemInfo) // placate some compilers that handle BOOL unconventionally
{
#if defined(HAL_REDUCESIZE)
    // No config update.
    return FALSE;
#else

    // MfReleaseInfo:
    systemInfo.m_releaseInfo.version.usMajor       = VERSION_MAJOR;
    systemInfo.m_releaseInfo.version.usMinor       = VERSION_MINOR;
    systemInfo.m_releaseInfo.version.usBuild       = VERSION_BUILD;
    systemInfo.m_releaseInfo.version.usRevision    = VERSION_REVISION;
    const size_t len = sizeof(systemInfo.m_releaseInfo.infoString);

    hal_strncpy_s ((char*)&systemInfo.m_releaseInfo.infoString[0], len, OEMSYSTEMINFOSTRING, len-1 );

    // OEM_MODEL_SKU:
    memcpy((void*)&(systemInfo.m_OemModelInfo), (void*)&(g_ConfigurationSector.OEM_Model_SKU), sizeof(OEM_MODEL_SKU));

    // OEM_SERIAL_NUMBERS:
    //TODO: set config in the bootloader and copy the config value here. only retrieve SerialNumbers from HAL if g_ConfigurationSector.OemSerialNumbers was not set by the bootloader.
    ID_GetSerialNumbers(systemInfo.m_OemSerialNumbers);
    //memcpy((void*)&(systemInfo.m_OemSerialNumbers), (void*)&(g_ConfigurationSector.OemSerialNumbers), sizeof(OEM_SERIAL_NUMBERS));

    return TRUE;
#endif
}
