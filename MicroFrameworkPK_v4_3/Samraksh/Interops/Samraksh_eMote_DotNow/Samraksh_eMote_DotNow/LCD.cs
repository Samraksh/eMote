using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote.DotNow
{    
    /// <summary>
    /// LCD blink rate
    /// </summary>
    public enum Blink
    {
        OFF,
        SLOW,
        MEDIUM,
        FAST
    };

    /// <summary>
    /// LCD char enum
    /// </summary>
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
        CHAR_9,
		CHAR_UNDERSCORE,
		CHAR_HYPEN
    };

    /// <summary>
    /// Emote LCD Interface class
    /// </summary>
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

        /// <summary>
        /// Initializes the lcd hardware
        /// </summary>
        /// <returns>The result of lcd initialization: Success, Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Initialize();

        /// <summary>
        /// Uninitializes the lcd hardware
        /// </summary>
        /// <returns>The result of lcd uninitialize: Success, Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Uninitialize();

        /// <summary>
        /// Write to all columns of the lcd interface 
        /// </summary>
        /// <param name="data4">Columns 4</param>
        /// <param name="data3">Columns 3</param>
        /// <param name="data2">Columns 2</param>
        /// <param name="data1">Columns 1</param>
        /// <returns>The result of the write operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Write(LCD data4, LCD data3, LCD data2, LCD data1);

        /// <summary>
        /// Set the decimal point
        /// </summary>
        /// <param name="dp1">Columns 1</param>
        /// <param name="dp2">Columns 2</param>
        /// <param name="dp3">Columns 3</param>
        /// <param name="dp4">Columns 4</param>
        /// <returns>The result ofthe write operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetDP(bool dp1, bool dp2, bool dp3, bool dp4);

        /// <summary>
        /// Writes to the specified column on the lcd 
        /// </summary>
        /// <param name="column">Specifies the column on the lcd hardware</param>
        /// <param name="data">Sepcifies the characted to be printed</param>
        /// <returns>The result of the write operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool WriteN(int column, LCD data);

        /// <summary>
        /// Write raw data to the lcd instead of characters
        /// </summary>
        /// <param name="data4">Column 4</param>
        /// <param name="data3">Column 3</param>
        /// <param name="data2">Column 2</param>
        /// <param name="data1">Column 1</param>
        /// <returns>Return the result of the write operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool WriteRawBytes(int data4, int data3, int data2, int data1);

        /// <summary>
        /// Set the lcd to blink
        /// </summary>
        /// <param name="blinkType">Defines the rate of the blink</param>
        /// <returns>Returns the result of the blink operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Blink(Blink blinkType);

        /// <summary>
        /// Clear the lcd 
        /// </summary>
        /// <returns>Returns the result of the clear operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Clear();
    }
}
