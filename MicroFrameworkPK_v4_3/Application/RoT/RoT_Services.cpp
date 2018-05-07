/*
 * RoT_Services.cpp
 *
 *  Created on: Apr 25, 2018
 *      Author: MukundanSridharan
 */

#include "RoT_Services.h"
#include "Commands.h"

Loader_Engine g_eng;
BYTE kernelHMAC[32];

//This looks up key and hmac from the config section of the flash, recomputes hash and validates it
bool AttestOS(UINT8* signature, UINT32 length, UINT32 keyIndex){
	return  g_eng.m_signedDataState.VerifySignature(signature,length, keyIndex );
}

bool AttestBinary(PBYTE  pData, PBYTE pSig, SigType st, KeyType kt, PBYTE key ){
	return FALSE;
}

bool ComputeHMAC(PBYTE  pData, UINT32 ulDataLen, PBYTE pDigest, SigType mtype, KeyType kt, PBYTE key, UINT32 keyLen){
	return FALSE;
}

bool SecureOS_Boot(){
	if(AttestOS(kernelHMAC,32, 0)){
		g_eng.EnumerateAndLaunch();
	}
	return FALSE;
}

/////////////////////// Support methods for reading and writing//////////////

////////////////////////////////////////////////////////////////////////////////
// The SectorOverlapsBootstrapRegion method enables you to deny access for writing
// certain sectors.  Returning true does not guarrantee that Tinybooter will be
// able to write to the sector.  It performes other checks (including signature
// validation) to determine if the write operation is valid.
////////////////////////////////////////////////////////////////////////////////
bool CheckFlashSectorPermission( BlockStorageDevice *pDevice, ByteAddress address )
{
    bool fAllowWrite = false;
    UINT32 BlockType, iRegion, iRange;


    if(pDevice->FindRegionFromAddress(address, iRegion, iRange))
    {
        const BlockRange& range = pDevice->GetDeviceInfo()->Regions[iRegion].BlockRanges[iRange];

        if (range.IsBootstrap())
        {
#if defined(TARGETLOCATION_RAM) // do not allow overwriting the bootstrap sector unless Tinybooter is RAM build.
            fAllowWrite = true;
#else
            hal_printf( "Trying to write to bootstrap region\r\n" );
            fAllowWrite = false;
#endif
        }
        else
        {
            fAllowWrite = true;
        }
    }

    return fAllowWrite;
}

////////////////////////////////////////////////////////////////////////////////
// TinyBooter_GetOemInfo
//
// Return in version and oeminfostring the compile-time values of some OEM-specific
// data, which you provide in the ReleaseInfo.settings file to apply to all projects
// built in this Porting Kit, or in the settings file specific to this solution.
//
// The properties to set are
//  MajorVersion, MinorVersion, BuildNumber, RevisionNumber, and OEMSystemInfoString.
// If OEMSystemInfoString is not provided, it will be created by concatenating your
// settings for MFCompanyName and MFCopyright, also defined in ReleaseInfo.settings
// or your solution's own settings file.
//
// It is typically not necessary actually to modify this function. If you do, note
// that the return value indicates to the caller whether the releaseInfo structure
// has been fully initialized and is valid. It is safe to return false if
// there is no useful build information you wish to report.
////////////////////////////////////////////////////////////////////////////////
BOOL TinyBooter_GetReleaseInfo(MfReleaseInfo& releaseInfo)
{
    MfReleaseInfo::Init(
                    releaseInfo,
                    VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION,
                    OEMSYSTEMINFOSTRING, hal_strlen_s(OEMSYSTEMINFOSTRING)
                    );
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//  Tinybooter_ProgramWordCheck
//
//  Returns the value of the first binary word of an application in FLASH
//  Tinybooter will search for this value at the beginning of each flash sector
//  at boot time and will execute the first instance.
////////////////////////////////////////////////////////////////////////////////
UINT32 Tinybooter_ProgramWordCheck()
{
    return 0x2000C000;  // for backward compatibility, hard-code 0x2000C000 as the magic word.  Should match first word of ARM_Vectors in VectorsTrampolines.s.
}




