using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh
{
    public class ADC
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int Init(int adc_num_cycles);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int getData(ushort[] currSample, uint startChannel, uint numChannels);
 
    }
}
