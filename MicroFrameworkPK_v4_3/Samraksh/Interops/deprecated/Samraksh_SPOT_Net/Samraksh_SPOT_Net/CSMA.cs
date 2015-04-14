using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Samraksh.SPOT.Net.Mac;

namespace Samraksh.SPOT.Net.Mac
{
    public class CSMA : MACBase
    {
        private static CSMA instance;
        private static object syncObject = new Object();

        private CSMA() : base(MacID.CSMA)
        {
        }

        // There can only be one csma mac object in the system
        /// <summary>
        /// Returns the instance of the CSMA object
        /// </summary>
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