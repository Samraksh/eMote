/*
 * PortableArrayKitUpdateProvider.h
 *
 *  Created on: Apr 9, 2013
 *      Author: researcher
 */

#ifndef PORTABLEARRAYKITUPDATEPROVIDER_H_
#define PORTABLEARRAYKITUPDATEPROVIDER_H_

#include <TinyHal.h>
#include <MFUpdate_decl.h>
//#include "..\..\..\..\DeviceCode\Drivers\MFUpdate\Storage\BlockStorageUpdate.h"
//#include "Storage\FS\FSUpdateStorage.h"
//#include "Validation\SSL\SslUpdateValidation.h"
//#include "..\..\..\..\DeviceCode\Drivers\MFUpdate\Validation\CRC\CrcUpdateValidation.h"
//#include "Validation\X509\X509UpdateValidation.h"

struct PortableArrayKitUpdateProvider
{
    static BOOL InitializeUpdate( MFUpdate* pUpdate );
    static BOOL GetProperty     ( MFUpdate* pUpdate, LPCSTR szPropName , UINT8* pPropValue, INT32* pPropValueSize                       );
    static BOOL SetProperty     ( MFUpdate* pUpdate, LPCSTR szPropName , UINT8* pPropValue, INT32  pPropValueSize                       );
    static BOOL InstallUpdate   ( MFUpdate* pUpdate, UINT8* pValidation, INT32  validationLen                                           );
};

extern IUpdateProvider g_PortableArrayKitUpdateProvider;




#endif /* PORTABLEARRAYKITUPDATEPROVIDER_H_ */
