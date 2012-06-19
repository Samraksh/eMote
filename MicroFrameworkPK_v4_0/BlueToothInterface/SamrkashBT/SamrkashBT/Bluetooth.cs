using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace SamrkashBT
{
    public class Bluetooth
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public void Init(uint master, uint remote);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public bool Send(byte[] data);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public bool Recieve(byte[] data);

    }
}
