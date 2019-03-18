using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;

namespace Samraksh.eMote.SensorBoard
{
    public class Buzzer
    {
        static Buzzer() { }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public bool On();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public bool Off();
    }
}
