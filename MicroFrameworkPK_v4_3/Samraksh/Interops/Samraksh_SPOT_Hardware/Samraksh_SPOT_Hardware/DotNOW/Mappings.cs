/*******************************************************
 * Pertains only to .NOW 
*******************************************************/
#if dotNOW
using Microsoft.SPOT.Hardware;

namespace Samraksh.SPOT.Hardware {
        /// <summary>
        /// Screw terminal pin mapping
        /// </summary>
        public static class Pins {
            public const Cpu.Pin GPIO_J11_PIN3 = (Cpu.Pin)0;
            public const Cpu.Pin GPIO_J11_PIN4 = (Cpu.Pin)1;
            public const Cpu.Pin GPIO_J11_PIN5 = (Cpu.Pin)2;
            public const Cpu.Pin GPIO_J11_PIN6 = (Cpu.Pin)3;
            public const Cpu.Pin GPIO_J11_PIN7 = (Cpu.Pin)4;
            public const Cpu.Pin GPIO_J11_PIN8 = (Cpu.Pin)8;
            public const Cpu.Pin GPIO_J11_PIN9 = (Cpu.Pin)22;
            public const Cpu.Pin GPIO_J11_PIN10 = (Cpu.Pin)23;
            public const Cpu.Pin GPIO_J12_PIN1 = (Cpu.Pin)24;
            public const Cpu.Pin GPIO_J12_PIN2 = (Cpu.Pin)25;
            public const Cpu.Pin GPIO_J12_PIN3 = (Cpu.Pin)29;
            public const Cpu.Pin GPIO_J12_PIN4 = (Cpu.Pin)30;
            public const Cpu.Pin GPIO_J12_PIN5 = (Cpu.Pin)31;
        }

        /// <summary>
        /// Emote ADC channel mapping
        /// </summary>
        public static class ADCChannel {
            public const Cpu.AnalogChannel ADC_Channel1 = (Cpu.AnalogChannel)0;
            public const Cpu.AnalogChannel ADC_Channel2 = (Cpu.AnalogChannel)1;
        }

    }
#endif
