using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh_Emote_Direct
{
    public class Static
    {
        public int Init() {
            return 1;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt32 SetJump(UInt32 entry);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void RunNative();
       
    }
}
