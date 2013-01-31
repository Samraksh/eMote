using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.Hardware.EmoteDotNow
{    
    public enum Blink
    {
        OFF,
        SLOW,
        MEDIUM,
        FAST
    };

    public enum LCD
    {
        CHAR_NULL,
        CHAR_A,
        CHAR_B,
        CHAR_C,
        CHAR_D,
        CHAR_E,
        CHAR_F,
        CHAR_G,
        CHAR_H,
        CHAR_I,
        CHAR_J,
        CHAR_K,
        CHAR_L,
        CHAR_M,
        CHAR_N,
        CHAR_O,
        CHAR_P,
        CHAR_Q,
        CHAR_R,
        CHAR_S,
        CHAR_T,
        CHAR_U,
        CHAR_V,
        CHAR_W,
        CHAR_X,
        CHAR_Y,
        CHAR_Z,
        CHAR_a,
        CHAR_b,
        CHAR_c,
        CHAR_d,
        CHAR_e,
        CHAR_f,
        CHAR_g,
        CHAR_h,
        CHAR_i,
        CHAR_j,
        CHAR_k,
        CHAR_l,
        CHAR_m,
        CHAR_n,
        CHAR_o,
        CHAR_p,
        CHAR_q,
        CHAR_r,
        CHAR_s,
        CHAR_t,
        CHAR_u,
        CHAR_v,
        CHAR_w,
        CHAR_x,
        CHAR_y,
        CHAR_z,
        CHAR_0,
        CHAR_1,
        CHAR_2,
        CHAR_3,
        CHAR_4,
        CHAR_5,
        CHAR_6,
        CHAR_7,
        CHAR_8,
        CHAR_9
    };

    public class EmoteLCD
    {
        int currentColumn1;
        int currentColumn2;
        int currentColumn3;
        int currentColumn4;

        bool DP1;
        bool DP2;
        bool DP3;
        bool DP4;

        public EmoteLCD()
        {
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Initialize();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Uninitialize();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Write(LCD data4, LCD data3, LCD data2, LCD data1);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetDP(bool dp1, bool dp2, bool dp3, bool dp4);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool WriteN(int column, LCD data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool WriteRawBytes(int data4, int data3, int data2, int data1);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Blink(Blink blinkType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Clear();
    }
}
