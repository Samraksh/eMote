using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.Hardware.Adapt
{
    /// <summary>
    /// Adapt ADC Interface class
    /// </summary>
    public class ADC
    {
        public ADC()
        {
        }

        /// <summary>
        /// Initializes the ADC (and FPGA if needed) hardware
        /// </summary>
        /// <returns>The result of ADC initialization: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_ADC_Init();

        /// <summary>
        /// ADC configuration register
        /// </summary>
        /// <param name="incc"></param>
        /// <param name="refr"></param>
        /// <param name="bw"></param>
        /// <returns>The result of ADC configuration: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_ADC_Conf(Byte incc, Byte refr, Byte bw);

        /// <summary>
        /// ADC command register
        /// </summary>
        /// <param name="chan"></param>
        /// <param name="cmd"></param>
        /// <returns>The result of ADC command: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_ADC_Command(Byte chan, Byte cmd);

        /// <summary>
        /// ADC status register
        /// </summary>
        /// <returns>The ADC status</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Byte ADAPT_ADC_Status();

        /// <summary>
        /// Sets sample rate in increments of 13.33us (256 clocks @ 19.2 MHz).
        /// </summary>
        /// <param name="sample_rate_ticks"></param>
        /// <returns>The result of ADC set sample rate: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_ADC_Sample_Rate(Byte sample_rate_ticks);

        /// <summary>
        /// Returns the last ADC immediate mode sample stored
        /// </summary>
        /// <returns>The ADC status</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 ADAPT_ADC_Get_Sample();

        /// <summary>
        /// Bit vector for enabling ADC inputs 0-7. Auto mode only.
        /// </summary>
        /// <param name="chan"></param>
        /// <returns>The result of ADC bit vector: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_ADC_Set_Chan(Byte chan);

        /// <summary>
        /// raw access mode. write_mode = 0. 'offset' is the offset of adc register from module base.
        /// </summary>
        /// <param name="offset"></param>
        /// <param name="rw"></param>
        /// <param name="data"></param>
        /// <returns>The result of ADC raw access: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_ADC_Raw(Byte offset, Byte rw, Byte data);

        /// <summary>
        /// Get continuous mode samples (multiple).
        /// </summary>
        /// <param name="buffer">array to place samples (in event of multiple channels, user must sort out).</param>
        /// <param name="toRead">Maximum number of samples to place into buffer (e.g. buffer size)</param>
        /// <returns>Number of samples available (including what was returned)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 ADAPT_ADC_Cont_Get(UInt16[] buffer, Byte toRead);

        /// <summary>
        /// Grab one sample on one channel and blocks until returned.
        /// </summary>
        /// <param name="chan"></param>
        /// <returns>The result of ADC now: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_ADC_Now(Byte chan);

        /// <summary>
        /// Setup and start continuous mode.
        /// </summary>
        /// <param name="chan"></param>
        /// <param name="sample_rate_ticks"></param>
        /// <returns>The result of ADC continuous start: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_ADC_Cont_Start(Byte chan, Byte sample_rate_ticks);

        /// <summary>
        /// Stop continuous mode.
        /// </summary>
        /// <returns>The result of ADC stop continuous mode: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_ADC_Cont_Stop();
    }
}