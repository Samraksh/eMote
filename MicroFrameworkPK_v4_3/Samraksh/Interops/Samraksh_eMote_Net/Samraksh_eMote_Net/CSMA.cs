using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Samraksh.eMote.Net.Mac;

namespace Samraksh.eMote.Net.Mac
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
