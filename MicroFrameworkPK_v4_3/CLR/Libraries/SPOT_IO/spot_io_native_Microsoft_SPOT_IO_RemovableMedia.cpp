////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "spot_io.h"

HRESULT Library_spot_io_native_Microsoft_SPOT_IO_RemovableMedia::MountRemovableVolumes___STATIC__VOID( CLR_RT_StackFrame& stack )
{
    NATIVE_PROFILE_CLR_IO();
    TINYCLR_HEADER();

    FS_MountRemovableVolumes();

    TINYCLR_NOCLEANUP_NOLABEL();
}
