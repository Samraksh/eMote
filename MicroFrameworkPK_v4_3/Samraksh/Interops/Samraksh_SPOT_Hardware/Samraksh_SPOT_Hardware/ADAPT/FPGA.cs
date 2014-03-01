/*******************************************************
 * Pertains only to ADAPT
*******************************************************/
#if ADAPT
using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.Hardware
{
    /// <summary>
    /// Adapt FPGA Interface class
    /// </summary>
    public class FPGA
    {
        public FPGA()
        {
            Initialize();
        }

        /// <summary>
        /// Initializes the FPGA hardware
        /// </summary>
        /// <returns>The result of FPGA initialization: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Initialize();
    }
}
#endif