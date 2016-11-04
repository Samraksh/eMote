//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "SamrakshEmote.h"
#include "SamrakshEmote_Samraksh_eMote_Platform.h"

using namespace Samraksh::eMote;

enum PLATFORM_NAME
{
    /// <summary>Unknown</summary>
    UNKNOWN,
    /// <summary>.NOW</summary>
    DOTNOW,
    /// <summary>WLN</summary>
    WLN,
	/// <summary>SMARTFUSION2</summary>
	SMARTFUSION2
};


INT32 Platform::GetPlatformTypeInternal( HRESULT &hr )
{
#if defined(PLATFORM_ARM_EmoteDotNow)
	return DOTNOW;
#elif defined(PLATFORM_ARM_WLN)		
	return WLN;
#elif defined(PLATFORM_ARM_SmartFusion2)
	return SMARTFUSION2;
#else
    return UNKNOWN;
#endif
}

