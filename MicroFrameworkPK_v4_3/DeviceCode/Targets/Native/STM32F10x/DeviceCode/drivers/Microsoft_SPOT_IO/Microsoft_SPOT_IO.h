//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#ifndef _MICROSOFT_SPOT_IO_H_
#define _MICROSOFT_SPOT_IO_H_

#include <TinyCLR_Interop.h>
struct Library_Microsoft_SPOT_IO_System_IO_FileSystemManager
{
    static const int FIELD_STATIC__m_openFiles = 0;
    static const int FIELD_STATIC__m_lockedDirs = 1;
    static const int FIELD_STATIC__CurrentDirectory = 2;
    static const int FIELD_STATIC__m_currentDirectoryRecord = 3;


    //--//

};

struct Library_Microsoft_SPOT_IO_System_IO_FileSystemManager__FileRecord
{
    static const int FIELD__FullName = 65535;
    static const int FIELD__NativeFileStream = 65535;
    static const int FIELD__Share = 65535;


    //--//

};

struct Library_Microsoft_SPOT_IO_Microsoft_SPOT_IO_MediaEventArgs
{
    static const int FIELD__Time = 65535;
    static const int FIELD__Volume = 65535;


    //--//

};

struct Library_Microsoft_SPOT_IO_Microsoft_SPOT_IO_NativeFileInfo
{
    static const int FIELD__Attributes = 65535;
    static const int FIELD__CreationTime = 65535;
    static const int FIELD__LastAccessTime = 65535;
    static const int FIELD__LastWriteTime = 65535;
    static const int FIELD__Size = 65535;
    static const int FIELD__FileName = 65535;


    //--//

};

struct Library_Microsoft_SPOT_IO_Microsoft_SPOT_IO_NativeFileStream
{
    static const int FIELD__m_fs = 65535;

    TINYCLR_NATIVE_DECLARE(_ctor___VOID__STRING__I4);
    TINYCLR_NATIVE_DECLARE(Read___I4__SZARRAY_U1__I4__I4__I4);
    TINYCLR_NATIVE_DECLARE(Write___I4__SZARRAY_U1__I4__I4__I4);
    TINYCLR_NATIVE_DECLARE(Seek___I8__I8__U4);
    TINYCLR_NATIVE_DECLARE(Flush___VOID);
    TINYCLR_NATIVE_DECLARE(GetLength___I8);
    TINYCLR_NATIVE_DECLARE(SetLength___VOID__I8);
    TINYCLR_NATIVE_DECLARE(GetStreamProperties___VOID__BYREF_BOOLEAN__BYREF_BOOLEAN__BYREF_BOOLEAN);
    TINYCLR_NATIVE_DECLARE(Close___VOID);

    //--//

};

struct Library_Microsoft_SPOT_IO_Microsoft_SPOT_IO_NativeFindFile
{
    static const int FIELD__m_ff = 65535;

    TINYCLR_NATIVE_DECLARE(_ctor___VOID__STRING__STRING);
    TINYCLR_NATIVE_DECLARE(GetNext___MicrosoftSPOTIONativeFileInfo);
    TINYCLR_NATIVE_DECLARE(Close___VOID);
    TINYCLR_NATIVE_DECLARE(GetFileInfo___STATIC__MicrosoftSPOTIONativeFileInfo__STRING);

    //--//

};

struct Library_Microsoft_SPOT_IO_Microsoft_SPOT_IO_NativeIO
{
    TINYCLR_NATIVE_DECLARE(Format___STATIC__VOID__STRING__STRING__STRING__U4);
    TINYCLR_NATIVE_DECLARE(Delete___STATIC__VOID__STRING);
    TINYCLR_NATIVE_DECLARE(Move___STATIC__BOOLEAN__STRING__STRING);
    TINYCLR_NATIVE_DECLARE(CreateDirectory___STATIC__VOID__STRING);
    TINYCLR_NATIVE_DECLARE(GetAttributes___STATIC__U4__STRING);
    TINYCLR_NATIVE_DECLARE(SetAttributes___STATIC__VOID__STRING__U4);

    //--//

};

struct Library_Microsoft_SPOT_IO_Microsoft_SPOT_IO_RemovableMedia
{
    static const int FIELD_STATIC__Insert = 4;
    static const int FIELD_STATIC__Eject = 5;
    static const int FIELD_STATIC___volumes = 6;
    static const int FIELD_STATIC___events = 7;

    TINYCLR_NATIVE_DECLARE(MountRemovableVolumes___STATIC__VOID);

    //--//

};

struct Library_Microsoft_SPOT_IO_Microsoft_SPOT_IO_StorageEvent
{
    static const int FIELD__EventType = 65535;
    static const int FIELD__Handle = 65535;
    static const int FIELD__Time = 65535;


    //--//

};

struct Library_Microsoft_SPOT_IO_Microsoft_SPOT_IO_VolumeInfo
{
    static const int FIELD__Name = 65535;
    static const int FIELD__VolumeLabel = 65535;
    static const int FIELD__VolumeID = 65535;
    static const int FIELD__FileSystem = 65535;
    static const int FIELD__FileSystemFlags = 65535;
    static const int FIELD__DeviceFlags = 65535;
    static const int FIELD__SerialNumber = 65535;
    static const int FIELD__TotalFreeSpace = 65535;
    static const int FIELD__TotalSize = 65535;
    static const int FIELD__VolumePtr = 65535;

    TINYCLR_NATIVE_DECLARE(_ctor___VOID__STRING);
    TINYCLR_NATIVE_DECLARE(_ctor___VOID__U4);
    TINYCLR_NATIVE_DECLARE(Refresh___VOID);
    TINYCLR_NATIVE_DECLARE(FlushAll___VOID);
    TINYCLR_NATIVE_DECLARE(GetVolumes___STATIC__SZARRAY_MicrosoftSPOTIOVolumeInfo);
    TINYCLR_NATIVE_DECLARE(GetFileSystems___STATIC__SZARRAY_STRING);

    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Microsoft_SPOT_IO;

#endif  //_MICROSOFT_SPOT_IO_H_
