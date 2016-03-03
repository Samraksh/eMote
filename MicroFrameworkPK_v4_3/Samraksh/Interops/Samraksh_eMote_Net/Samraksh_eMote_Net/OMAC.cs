using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Samraksh.eMote.Net.Mac;

namespace Samraksh.eMote.Net.Mac
{
    /// <summary>OMAC objects</summary>
    public class OMAC : MACBase
    {
        /// <summary>
        /// 
        /// </summary>
        public OMAC(MACConfiguration MacConfig)
            : base(MacConfig, MACType.OMAC)
        {
        }

    }
}
