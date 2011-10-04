using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh
{
    class FileSystem
    {
        public enum Error {SUCCESS = 0, FAIL = 1} ;

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public Error Init();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public Error Write(byte[] buffer, UInt32 address, UInt16 blockSize);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public Error Read(byte[] buffer, UInt32 address, UInt16 blockSize);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public Error Erase(UInt32 startAddress, UInt32 endAddress);

    }
}
