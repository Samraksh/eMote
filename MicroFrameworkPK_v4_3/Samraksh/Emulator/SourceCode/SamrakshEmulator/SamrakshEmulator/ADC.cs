using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

using Microsoft.SPOT.Emulator;
using SamrakshEmulator;

namespace Samraksh.SPOT.Emulator
{
    enum AdcMode {Independent, Dependent};
    enum AdcDataAlign {Right, Left};
    enum AdcExternalTrigConv {None,Rising,Falling};
    public sealed class ADC_Configuration {
        AdcMode mode;
        bool ScanConvMode;
        bool ContinousConvMode;
        short NumberOfChannels = 3;
        AdcDataAlign DataAlignment;
        AdcExternalTrigConv ExternalTrigConv;
    }
    public partial class ADC
    {
        static ADC_Configuration _config;
        static short _samplingTime; //in MicroSeconds
        static bool[] ChannelEnabled= new bool[3];
        static Logger Log = new Logger();

        public ADC(ADC_Configuration Configuration)
        {
            _config = Configuration;
            
        }
        static void ADC_Configuration(short SamplingTime)
        {
            _samplingTime = SamplingTime;

            /* ADC1 regular channel10 configuration */
            //ADC_RegularChannelConfig(ADC1, EMOTE_ADC_CHANNEL[0], 1, sampTime);
            //ADC_RegularChannelConfig(ADC2, EMOTE_ADC_CHANNEL[1], 1, sampTime);
            //ADC_RegularChannelConfig(ADC3, EMOTE_ADC_CHANNEL[2], 1, sampTime);

            /* Enable ADC1 DMA */
            //ADC_DMACmd(ADC1, ENABLE);

            /* Enable ADC1 */
            ChannelEnabled[0] = true;
            ChannelEnabled[1] = true;
            ChannelEnabled[2] = true;
            
        }

        
        
       /************Public interfaces************/ 
        //[MethodImplAttribute(] 
        static public int Init(int adc_num_cycles)
        {
            ADC_Configuration((short)adc_num_cycles);
            Log.Debug(Module.CORE, "ADC::Init");
            return 1;
        }

        static public int getData(ushort[] currSample, uint startChannel, uint numChannels)
        {
            Log.Debug(Module.CORE, "ADC::getData");

            if (startChannel > 2 || (startChannel + numChannels > 3))
                return 0;

            currSample[0] = (ushort)4096;
            currSample[0] = (ushort)8192;
            return 1;
        }
    }
}
