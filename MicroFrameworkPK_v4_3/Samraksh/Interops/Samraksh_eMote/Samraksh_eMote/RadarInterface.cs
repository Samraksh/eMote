using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote
{
    class RadarInterface
    {
        /// <summary>
            /// 
            /// </summary>
            public RadarInterface() {                
            }
            
            //////////////////////////public properties and methods/////////////////////

            /// <summary>
            /// 
            /// </summary>
            ~RadarInterface()
            {
                TurnOff();
            }

            ///////////////////////////////////Internal methods/////////////////////////

            /// <summary>
            /// Turns on the Radar
            /// </summary>
            /// <returns>The result of turning on the radar: Success, Fail</returns>
            [MethodImpl(MethodImplOptions.InternalCall)]
            private extern bool TurnOn();

            /// <summary>
            /// Turns off the Radar
            /// </summary>
            /// <returns>The result of turning off the radar: Success, Fail</returns>
            [MethodImpl(MethodImplOptions.InternalCall)]
            private extern bool TurnOff();

            /// <summary>
            /// Blinds the Radar
            /// </summary>
            /// <returns>The result of blinding the radar: Success, Fail</returns>
            [MethodImpl(MethodImplOptions.InternalCall)]
            private extern bool Blind();
    }
}
