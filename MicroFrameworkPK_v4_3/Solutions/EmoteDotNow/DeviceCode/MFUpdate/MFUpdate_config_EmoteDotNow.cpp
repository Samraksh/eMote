#include <tinyhal.h>
#include <mfupdate_decl.h>
#include <Drivers\MFUpdate\MicroBooterUpdate.h>
#include "PortableArrayKitUpdateProvider.h"

//IUpdatePackage interface object describes the set of update facilities used
// by the MFUpdate feature. When an update is created it is passed a provider
// name which must correspond to one of the IUpdatePackages for the solution.
// From that point on it uses the facilities designated by the update package.
static const IUpdatePackage s_UpdatePackages[] =
{
    {
        "NetMF",
        &g_MicroBooterUpdateProvider,
        &g_CrcUpdateValidationProvider,
        &g_BlockStorageUpdateProvider,
        NULL,/*backup provider*/
    },
    {
        "PAK",
        &g_PortableArrayKitUpdateProvider,
        &g_CrcUpdateValidationProvider,
        &g_BlockStorageUpdateProvider,
        NULL,/*backup provider*/
    }
};

const IUpdatePackage* g_UpdatePackages     = s_UpdatePackages;
const INT32           g_UpdatePackageCount = ARRAYSIZE(s_UpdatePackages);


