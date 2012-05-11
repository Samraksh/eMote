using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh
{
    public class rDAC
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public void Init();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int setValue(UInt16 value);
    }
}
