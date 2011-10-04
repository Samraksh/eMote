using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace SimpleInterop
{
    public class Library
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void init();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void execute();

    }
}
