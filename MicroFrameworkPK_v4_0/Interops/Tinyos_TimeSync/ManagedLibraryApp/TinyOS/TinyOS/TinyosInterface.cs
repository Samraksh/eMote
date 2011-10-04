using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Runtime.CompilerServices;

namespace TinyOS
{
    public class TinyosInterface
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void execute(byte[] input);
    }
}
