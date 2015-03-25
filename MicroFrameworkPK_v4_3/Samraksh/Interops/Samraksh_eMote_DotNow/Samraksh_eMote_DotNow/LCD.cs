using System.Runtime.CompilerServices;

namespace Samraksh.eMote.DotNow
{
    // ReSharper disable InconsistentNaming

    /// <summary>
    /// LCD blink rate
    /// </summary>
    public enum Blink
    {
        /// <summary>Don't blink</summary>
        OFF,
        /// <summary>Slow blink</summary>
        SLOW,
        /// <summary>Medium blink</summary>
        MEDIUM,
        /// <summary>Fast blink</summary>
        FAST
    };

    /// <summary>
    /// LCD chars
    /// </summary>
    public enum LCD
    {
        /// <summary>Blank</summary>
        CHAR_NULL,
        /// <summary>A</summary>
        CHAR_A,
        /// <summary>B</summary>
        CHAR_B,
        /// <summary>C</summary>
        CHAR_C,
        /// <summary>D</summary>
        CHAR_D,
        /// <summary>E</summary>
        CHAR_E,
        /// <summary>F</summary>
        CHAR_F,
        /// <summary>G</summary>
        CHAR_G,
        /// <summary>H</summary>
        CHAR_H,
        /// <summary>I</summary>
        CHAR_I,
        /// <summary>J</summary>
        CHAR_J,
        /// <summary>K</summary>
        CHAR_K,
        /// <summary>L</summary>
        CHAR_L,
        /// <summary>M</summary>
        CHAR_M,
        /// <summary>N</summary>
        CHAR_N,
        /// <summary>O</summary>
        CHAR_O,
        /// <summary>P</summary>
        CHAR_P,
        /// <summary>Q</summary>
        CHAR_Q,
        /// <summary>R</summary>
        CHAR_R,
        /// <summary>S</summary>
        CHAR_S,
        /// <summary>T</summary>
        CHAR_T,
        /// <summary>U</summary>
        CHAR_U,
        /// <summary>V</summary>
        CHAR_V,
        /// <summary>W</summary>
        CHAR_W,
        /// <summary>X</summary>
        CHAR_X,
        /// <summary>Y</summary>
        CHAR_Y,
        /// <summary>Z</summary>
        CHAR_Z,
        /// <summary>a</summary>
        CHAR_a,
        /// <summary>b</summary>
        CHAR_b,
        /// <summary>c</summary>
        CHAR_c,
        /// <summary>d</summary>
        CHAR_d,
        /// <summary>e</summary>
        CHAR_e,
        /// <summary>f</summary>
        CHAR_f,
        /// <summary>g</summary>
        CHAR_g,
        /// <summary>h</summary>
        CHAR_h,
        /// <summary>i</summary>
        CHAR_i,
        /// <summary>j</summary>
        CHAR_j,
        /// <summary>k</summary>
        CHAR_k,
        /// <summary>l</summary>
        CHAR_l,
        /// <summary>m</summary>
        CHAR_m,
        /// <summary>n</summary>
        CHAR_n,
        /// <summary>o</summary>
        CHAR_o,
        /// <summary>p</summary>
        CHAR_p,
        /// <summary>q</summary>
        CHAR_q,
        /// <summary>r</summary>
        CHAR_r,
        /// <summary>s</summary>
        CHAR_s,
        /// <summary>t</summary>
        CHAR_t,
        /// <summary>u</summary>
        CHAR_u,
        /// <summary>v</summary>
        CHAR_v,
        /// <summary>w</summary>
        CHAR_w,
        /// <summary>x</summary>
        CHAR_x,
        /// <summary>y</summary>
        CHAR_y,
        /// <summary>z</summary>
        CHAR_z,
        /// <summary>0</summary>
        CHAR_0,
        /// <summary>1</summary>
        CHAR_1,
        /// <summary>2</summary>
        CHAR_2,
        /// <summary>3</summary>
        CHAR_3,
        /// <summary>4</summary>
        CHAR_4,
        /// <summary>5</summary>
        CHAR_5,
        /// <summary>6</summary>
        CHAR_6,
        /// <summary>7</summary>
        CHAR_7,
        /// <summary>8</summary>
        CHAR_8,
        /// <summary>9</summary>
        CHAR_9,
        /// <summary>Underscore</summary>
        CHAR_UNDERSCORE,
        /// <summary>Hyphen</summary>
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
        /// <param name="column">Column to write</param>
        /// <param name="data">Character to write</param>
        /// <returns>True iff success</returns>
        /// <remarks>The columns are numbered as follows: </remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool WriteN(int column, LCD data);

        /// <summary>
        /// Write raw data to the LCD
        /// </summary>
        /// <param name="data4">Column 4 data</param>
        /// <param name="data3">Column 3 data</param>
        /// <param name="data2">Column 2 data</param>
        /// <param name="data1">Column 1 data</param>
        /// <returns>True iff success</returns>
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

        // ReSharper restore InconsistentNaming
    }
}
