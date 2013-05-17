using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using System.Threading;
using Samraksh.SPOT.Net.Mac;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware.EmoteDotNow;
using Samraksh.SPOT.RealTime;

namespace MakerFairController
{
    public static class EmotePins
    {
        public const Cpu.Pin GPIO_PIN_0 = (Cpu.Pin)24;	//PB8
        public const Cpu.Pin GPIO_PIN_1 = (Cpu.Pin)25;	//PB9
        public const Cpu.Pin GPIO_PIN_2 = (Cpu.Pin)29;	//PB13
        public const Cpu.Pin GPIO_PIN_3 = (Cpu.Pin)30;	//PB14
        public const Cpu.Pin GPIO_PIN_4 = (Cpu.Pin)31;	//PB15
        public const Cpu.Pin GPIO_PIN_5 = (Cpu.Pin)8;	//PA8
        public const Cpu.Pin GPIO_PIN_6 = (Cpu.Pin)22;	//PB6
        public const Cpu.Pin GPIO_PIN_7 = (Cpu.Pin)23;	//PB7
    }
}
