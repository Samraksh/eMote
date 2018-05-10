/*
 * RoT_Services.cpp
 *
 *  Created on: Apr 25, 2018
 *      Author: MukundanSridharan
 */

#include "RoT_Services.h"
#include "Commands.h"

Loader_Engine g_eng;
//BYTE kernelHMAC[32];

#define eNVMAddress 0x60000000
#define RoT_Offset 0x10000
#define App_Offset 0x65800

void PrintHex(UINT8* data, int size){
	for (int j=0;j<size; j++){
		debug_printf("0x%.2X , ",data[j]);
	}
	debug_printf("\n");
}

//This looks up key and hmac from the config section of the flash, recomputes hash and validates it
bool AttestOS(OSModule mod, UINT32 modLength,  UINT8* pSig, UINT32 sigLength, UINT8* pkey, UINT32 keyLength){
	if(mod==RoT || mod==TB)
		return  AttestBinary((BYTE*)eNVMAddress+RoT_Offset, modLength, pSig, sigLength, pkey, keyLength );
	else return FALSE;
}

/*
bool FindAddress(UINT32 Module){

}
*/

bool AttestBinary(PBYTE  pData, UINT32 dataLength, PBYTE pSig, UINT32 sigLength, PBYTE key, UINT32 keyLength ){
	//Compute Signature with Key
	BYTE mSig[sigLength];
	memset(mSig, 0, sigLength);
	debug_printf("Clearing signature buffer: ");
	PrintHex(mSig, sigLength);
	CRYPTO_RESULT m_res = Crypto_GetHMAC(pData,  dataLength, key, mSig, sigLength);
	//m_res = ::Crypto_StartRSAOperationWithKey( RSA_VERIFYSIGNATURE, key, (BYTE*)m_dataAddress, m_dataLength, (BYTE*)m_sig, m_sigLength, &m_handle );

	//Verify signature
	if(m_res != CRYPTO_SUCCESS) return false;

	//read the expected hash from the config section
	//hash = (BYTE*)g_PrimaryConfigManager.GetDeploymentHash( keyIndex );

	debug_printf("\n Expecting signature: ");
	PrintHex(pSig, sigLength);
	debug_printf(" Computed signature: ");
	PrintHex(mSig, sigLength);

	if(memcmp(mSig, pSig,sigLength)==0){
		return TRUE;
	}
	else {
		debug_printf("Validating failed.. \n");

		debug_printf("Data is: ");
		PrintHex(pData, 256);


		return FALSE;
	}

}

bool ComputeHMAC(PBYTE  pData, UINT32 ulDataLen, PBYTE pDigest, SigType mtype, KeyType kt, PBYTE key, UINT32 keyLen){
	return FALSE;
}

bool SecureOS_Boot(OSModule mod, UINT32 modLength, UINT8* pSig, UINT32 sigLength, UINT8* pKey, UINT32 keyLength){
	if (sigLength!=keyLength || sigLength!=32) return FALSE;

	if(AttestOS(mod, modLength,pSig,sigLength,pKey,keyLength)){
		debug_printf("Root of Trust Validation Success!!");
		int i=10;
		while(i>0){
			debug_printf("Will boot into Rot in %d\n", i);  i--;
			//WaitForEvent()
			Events_WaitForEvents(0,1000);
		}

		BootRoT();

	}
}

void BootRoT(){
	g_eng.EnumerateAndLaunch();
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




