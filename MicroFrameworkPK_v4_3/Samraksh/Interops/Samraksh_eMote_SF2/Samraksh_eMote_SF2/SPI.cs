using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote.SF2
{
        /// <summary>
        /// SPI class  
        /// </summary>
        public class SPI
        {
            /// <summary>
            /// 
            /// </summary>
            public SPI()
            {
                
            }

            //////////////////////////public properties and methods/////////////////////

            /// <summary>
            /// 
            /// </summary>
            ~SPI()
            {
            }

            ///////////////////////////////////Internal methods/////////////////////////

            /// <summary>
            /// Data is sent across the SPI.
            /// </summary>
            /// <returns>Returns true if a detection was found, false if no detection was found.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public bool SendData(Int32 SPI_Master, byte[] data, Int32 numBytes);    
        
            /// <summary>
            /// Data is received across the SPI.
            /// </summary>
            /// <returns>Returns true if a detection was found, false if no detection was found.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public bool ReceiveData(Int32 SPI_Master, byte[] data, Int32 numBytes);  
        }    
}
