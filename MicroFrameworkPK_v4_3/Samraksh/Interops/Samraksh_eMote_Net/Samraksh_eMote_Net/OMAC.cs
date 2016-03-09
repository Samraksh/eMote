using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Samraksh.eMote.Net.MAC;

namespace Samraksh.eMote.Net.MAC
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
