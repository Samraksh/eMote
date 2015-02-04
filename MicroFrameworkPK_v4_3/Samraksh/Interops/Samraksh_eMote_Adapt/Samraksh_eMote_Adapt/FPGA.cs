using System.Runtime.CompilerServices;

namespace Samraksh.eMote.Adapt
{
    /// <summary>Adapt FPGA class</summary>
    public class FPGA
    {
        /// <summary>FPGA constructor</summary>
        public FPGA()
        {
            Initialize();
        }

        /// <summary>Initialize FPGA hardware</summary>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Initialize();
    }
}