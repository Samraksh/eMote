using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote.DotLaura
{        
        public class LCD
        {
            private bool LCD_Initialized = false;

            public LCD()
            {
                Initialize();
            }

            [MethodImpl(MethodImplOptions.InternalCall)]
            public extern bool Initialize();

            [MethodImpl(MethodImplOptions.InternalCall)]
            public extern void WriteString(byte[] writeData, int size, int start_page, int start_column);

            [MethodImpl(MethodImplOptions.InternalCall)]
            public extern void Clear(int start_page, int start_column);
        }
}
