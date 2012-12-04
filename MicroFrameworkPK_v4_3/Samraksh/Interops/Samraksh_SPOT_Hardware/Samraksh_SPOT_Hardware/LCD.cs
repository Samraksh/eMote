using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.Hardware.EmoteDotNow
{
    public enum SymbolType
    {
        DISPLAY_CHAR_A,
        DISPLAY_CHAR_B,
        DISPLAY_CHAR_C,
        DISPLAY_CHAR_E,
        DISPLAY_CHAR_F,
        DISPLAY_CHAR_G,
        DISPLAY_CHAR_H,
        DISPLAY_CHAR_I,
        DISPLAY_CHAR_J,
        DISPLAY_CHAR_L,
        DISPLAY_CHAR_N,
        DISPLAY_CHAR_O,
        DISPLAY_CHAR_P,
        DISPLAY_CHAR_S,
        DISPLAY_CHAR_U,
        DISPLAY_CHAR_Y,
        DISPLAY_CHAR_a,
        DISPLAY_CHAR_b,
        DISPLAY_CHAR_c,
        DISPLAY_CHAR_d,
        DISPLAY_CHAR_e,
        DISPLAY_CHAR_g,
        DISPLAY_CHAR_h,
        DISPLAY_CHAR_i,
        DISPLAY_CHAR_n,
        DISPLAY_CHAR_o,
        DISPLAY_CHAR_q,
        DISPLAY_CHAR_r,
        DISPLAY_CHAR_t,
        DISPLAY_CHAR_u,
        DISPLAY_CHAR_0,
        DISPLAY_CHAR_1,
        DISPLAY_CHAR_2,
        DISPLAY_CHAR_3,
        DISPLAY_CHAR_4,
        DISPLAY_CHAR_5,
        DISPLAY_CHAR_6,
        DISPLAY_CHAR_7,
        DISPLAY_CHAR_8,
        DISPLAY_CHAR_9,
        DISPLAY_CHAR_NULL,
        DISPLAY_DECIMAL_POINT,
    };

   

    public class LCD
    {
        public LCD()
        {
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Initialize();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern bool Write(int column4, int column3, int column2, int column1);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Clear();


        public bool WriteColumnN(int column, SymbolType symbol)
        {
            SymbolType nullsymbol = SymbolType.DISPLAY_CHAR_NULL;

            return Write((int)nullsymbol, (int)nullsymbol, (int)nullsymbol, (int)symbol);
        }

       

        public bool Write(SymbolType column1, SymbolType column2, SymbolType column3, SymbolType column4)
        {
            return  Write((int) column1, (int) column2, (int) column3, (int) column4);
        }

        public bool WriteRawBytes(byte b1, byte b2, byte b3, byte b4)
        {
            return Write((int) b1, (int) b2, (int) b3, (int) b4);
        }


    }
}
