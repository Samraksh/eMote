using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh
{
    public class USBPipe
    {
        static public int Write(byte[] buffer)
        {
            return Write(buffer, 0, buffer.Length);
        }

        static public int Read(byte[] buffer)
        {
            return Read(buffer, 0, buffer.Length);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int Write(byte[] buffer, int offset, int numberOfBytes);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int Read(byte[] buffer, int offset, int count);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public bool Flush();

    }
}
