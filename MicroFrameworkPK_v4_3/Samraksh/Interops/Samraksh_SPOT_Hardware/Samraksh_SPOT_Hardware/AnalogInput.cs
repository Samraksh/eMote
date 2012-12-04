using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh_SPOT_Hardware.EmoteDotNow
{
    public class AnalogInput
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern double Read();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern int ReadRaw();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void Dispose();
    }
}
