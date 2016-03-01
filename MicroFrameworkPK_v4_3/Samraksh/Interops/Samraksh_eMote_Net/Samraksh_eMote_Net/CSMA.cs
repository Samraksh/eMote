using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Samraksh.eMote.Net.Mac;

namespace Samraksh.eMote.Net.Mac
{
    /// <summary>CSMA objects</summary>
    public class CSMA : MACBase
    {
        private static CSMA instance;
        private static object syncObject = new Object();

        private CSMA()
            : base(MACType.CSMA)
        {
        }

        /// <summary>
        /// Returns the instance of the CSMA object
        /// </summary>
        /// <remarks>This is a singleton pattern. There can only be one CSMA Mac object.</remarks>
        public static CSMA Instance
        {
            get
            {
                if (instance == null)
                {
                    lock (syncObject)
                    {
                        if (instance == null)
                            instance = new CSMA();
                    }
                }

                return instance;
            }
        }

    }
}
