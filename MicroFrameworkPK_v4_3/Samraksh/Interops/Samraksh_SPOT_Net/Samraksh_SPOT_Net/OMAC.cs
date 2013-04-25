using System;
using Microsoft.SPOT;

namespace Samraksh.SPOT.Net
{
    public class OMAC : MACBase
    {
        private static OMAC instance;
        private static object syncObject = new Object();

        private OMAC() : base(MacID.OMAC)
        {
        }

        // There can only be one csma mac object in the system
        /// <summary>
        /// Returns the instance of the CSMA object
        /// </summary>
        public static OMAC Instance
        {
            get
            {
                if (instance == null)
                {
                    lock (syncObject)
                    {
                        if (instance == null)
                            instance = new OMAC();
                    }
                }

                return instance;
            }
        }
    }
}
