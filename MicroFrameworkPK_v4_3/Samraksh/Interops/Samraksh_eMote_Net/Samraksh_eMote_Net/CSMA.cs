using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Samraksh.eMote.Net.MAC;


namespace Samraksh.eMote.Net.MAC
{
    /// <summary>CSMA objects</summary>
    public class CSMA : MACBase
    {
        /// <summary>
        /// 
        /// </summary>
        public CSMA(MACConfiguration MacConfig)
            : base(MacConfig, MACType.CSMA)
        {
        }

    }
}
