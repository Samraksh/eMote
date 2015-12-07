using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT;

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
        private I2CDevice.Configuration config;
        private I2CDevice i2c;
        private I2CDevice.I2CTransaction[] i2cTx;

        private byte[] LCD_NUM = new byte[] {0x00, 0xee, 0xef, 0x8d, 0xed, 0x8f, 0x8e, 0xcd, 0x6e, 0x60, 0x65, 0x6e, 0x0d, 0xc6, 0xec, 0xed, 0xae, 0xea, 0x8c, 0xcb, 0xe0, 0x6d, 0x29, 0xc5, 0x6e, 0x6a, 0xa7, 0xe7, 0x4f, 0x07, 0x67, 0xaf, 0x8e, 0xeb, 0x4e, 0x40, 0x41, 0x2f, 0x60, 0x8f, 0x46, 0x47, 0xae, 0xea, 0x06, 0xcb, 0x0f, 0x45, 0x45, 0xe3, 0x6e, 0x6b, 0xa7, 0xed, 0x60, 0xa7, 0xe3, 0x6a, 0xcb, 0xcf, 0xe0, 0xef, 0xeb, 0x02, 0x08};

        private const Cpu.Pin GPIO_PIN_PB12 = (Cpu.Pin)28;
        private static Microsoft.SPOT.Hardware.OutputPort enableLCD = new Microsoft.SPOT.Hardware.OutputPort(GPIO_PIN_PB12, false);

        private int currentColumn1;
        private int currentColumn2;
        private int currentColumn3;
        private int currentColumn4;

        private bool DP1;
        private bool DP2;
        private bool DP3;
        private bool DP4;

        private bool LCD_Initialized = false;

        public EmoteLCD()
        {
            Initialize();
        }

        /// <summary>
        /// Initializes the lcd hardware
        /// </summary>
        /// <returns>The result of lcd initialization: Success, Fail</returns>
        public bool Initialize()
        {
            if (!LCD_Initialized)
            {
                config = new I2CDevice.Configuration(0x38, 100);
                i2c = new I2CDevice(config);
                i2cTx = new I2CDevice.I2CTransaction[1];

                currentColumn1 = 0;
                currentColumn2 = 0;
                currentColumn3 = 0;
                currentColumn4 = 0;
                DP1 = false;
                DP2 = false;
                DP3 = false;
                DP4 = false;

                enableLCD.Write(true);
                Write(LCD.CHAR_NULL, LCD.CHAR_NULL, LCD.CHAR_NULL, LCD.CHAR_NULL);

                LCD_Initialized = true;
            }
            return LCD_Initialized;
        }

        /// <summary>
        /// Uninitializes the lcd hardware
        /// </summary>
        /// <returns>The result of lcd uninitialize: Success, Fail</returns>        
        public bool Uninitialize()
        {            
            if (LCD_Initialized)
            {
                // This will cause the I2C lines to be un-reserved but MF never actually calls I2C_Uninitialize. 
                i2c.Dispose();
                enableLCD.Write(false);
                LCD_Initialized = false;
            }

            return LCD_Initialized;
        }

        /// <summary>
        /// Write to all columns of the lcd interface 
        /// </summary>
        /// <param name="data4">Columns 4</param>
        /// <param name="data3">Columns 3</param>
        /// <param name="data2">Columns 2</param>
        /// <param name="data1">Columns 1</param>
        /// <returns>The result of the write operation</returns>
        public bool Write(LCD data4, LCD data3, LCD data2, LCD data1)
        {
            byte byte1, byte2, byte3, byte4;

            if (LCD_Initialized == false) { return false; }

            currentColumn1 = (int)data1;
            currentColumn2 = (int)data2;
            currentColumn3 = (int)data3;
            currentColumn4 = (int)data4;

            if (DP1 == true)
            {
                byte1 = (byte)(LCD_NUM[currentColumn1] | 0x10);
            }
            else
            {
                byte1 = (byte)(LCD_NUM[currentColumn1]);
            }
            if (DP2 == true)
            {
                byte2 = (byte)(LCD_NUM[currentColumn2] | 0x10);
            }
            else
            {
                byte2 = (byte)(LCD_NUM[currentColumn2]);
            }
            if (DP3 == true)
            {
                byte3 = (byte)(LCD_NUM[currentColumn3] | 0x10);
            }
            else
            {
                byte3 = (byte)(LCD_NUM[currentColumn3]);
            }
            if (DP4 == true)
            {
                byte4 = (byte)(LCD_NUM[currentColumn4] | 0x10);
            }
            else
            {
                byte4 = (byte)(LCD_NUM[currentColumn4]);
            }

            byte[] write = new byte[] { 0x80, 0x48, byte4, byte3, byte2, byte1 };
            i2cTx[0] = I2CDevice.CreateWriteTransaction(write);
            lock (i2c)
            {
                i2c.Config = config;                

                if (i2c.Execute(i2cTx, 100) == 0)
                    return false;
                else
                    return true;
            }
        }

        /// <summary>
        /// Set the decimal point
        /// </summary>
        /// <param name="dp1">Columns 1</param>
        /// <param name="dp2">Columns 2</param>
        /// <param name="dp3">Columns 3</param>
        /// <param name="dp4">Columns 4</param>
        /// <returns>The result ofthe write operation</returns>
        public bool SetDP(bool dp1, bool dp2, bool dp3, bool dp4)
        {
            if (LCD_Initialized == false) { return false; }

            DP1 = dp1;
            DP2 = dp2;
            DP3 = dp3;
            DP4 = dp4;

            Write((LCD)currentColumn4, (LCD)currentColumn3, (LCD)currentColumn2, (LCD)currentColumn1);

            return true;
        }

        /// <summary>
        /// Writes to the specified column on the lcd 
        /// </summary>
        /// <param name="column">Column to write</param>
        /// <param name="data">Character to write</param>
        /// <returns>True iff success</returns>
        /// <remarks>The columns are numbered as follows: </remarks>
        public bool WriteN(int column, LCD data)
        {
            byte byte1, byte2, byte3, byte4;
            if (LCD_Initialized == false) { return false; }
            
            switch (column)
            {
                case 1:
                    currentColumn1 = (int)data;
                    break;
                case 2:
                    currentColumn2 = (int)data;
                    break;
                case 3:
                    currentColumn3 = (int)data;
                    break;
                case 4:
                    currentColumn4 = (int)data;
                    break;
                default:
                    currentColumn4 = (int)data;
                    break;
            }
            if (DP1 == true)
            {
                byte1 = (byte)(LCD_NUM[currentColumn1] | 0x10);
            }
            else
            {
                byte1 = (byte)(LCD_NUM[currentColumn1]);
            }
            if (DP2 == true)
            {
                byte2 = (byte)(LCD_NUM[currentColumn2] | 0x10);
            }
            else
            {
                byte2 = (byte)(LCD_NUM[currentColumn2]);
            }
            if (DP3 == true)
            {
                byte3 = (byte)(LCD_NUM[currentColumn3] | 0x10);
            }
            else
            {
                byte3 = (byte)(LCD_NUM[currentColumn3]);
            }
            if (DP4 == true)
            {
                byte4 = (byte)(LCD_NUM[currentColumn4] | 0x10);
            }
            else
            {
                byte4 = (byte)(LCD_NUM[currentColumn4]);
            }

            byte[] write = new byte[] { 0x80, 0x48, byte4, byte3, byte2, byte1 };
            i2cTx[0] = I2CDevice.CreateWriteTransaction(write);
            lock (i2c)
            {
                i2c.Config = config;

                if (i2c.Execute(i2cTx, 100) == 0)
                    return false;
                else
                    return true;
            }

            return true;
        }

        /// <summary>
        /// Write raw data to the LCD
        /// </summary>
        /// <param name="data4">Column 4 data</param>
        /// <param name="data3">Column 3 data</param>
        /// <param name="data2">Column 2 data</param>
        /// <param name="data1">Column 1 data</param>
        /// <returns>True iff success</returns>
        public bool WriteRawBytes(int data4, int data3, int data2, int data1)
        {
            if (LCD_Initialized == false) { return false; }

            currentColumn1 = data1 & 0xff;
            currentColumn2 = data2 & 0xff;
            currentColumn3 = data3 & 0xff;
            currentColumn4 = data4 & 0xff;

            byte[] write = new byte[] { 0x80, 0x48, (byte)currentColumn4, (byte)currentColumn3, (byte)currentColumn2, (byte)currentColumn1 };
            i2cTx[0] = I2CDevice.CreateWriteTransaction(write);
            lock (i2c)
            {
                i2c.Config = config;

                if (i2c.Execute(i2cTx, 100) == 0)
                    return false;
                else
                    return true;
            }            

            return true;
        }

        /// <summary>
        /// Set the lcd to blink
        /// </summary>
        /// <param name="blinkType">Defines the rate of the blink</param>
        /// <returns>Returns the result of the blink operation</returns>
        public bool Blink(Blink blinkType)
        {
            if (LCD_Initialized == false) { return false; }

            byte[] write;

            switch (blinkType)
            {
                case (Samraksh.eMote.DotNow.Blink.OFF):
                    write = new byte[] { 0x80, 0xF0 };
                    break;
                case (Samraksh.eMote.DotNow.Blink.SLOW):
                    write = new byte[] { 0x80, 0xF3 };
                    break;
                case (Samraksh.eMote.DotNow.Blink.MEDIUM):
                    write = new byte[] { 0x80, 0xF2 };
                    break;
                case (Samraksh.eMote.DotNow.Blink.FAST):
                    write = new byte[] { 0x80, 0xF1 };
                    break;
                default:
                    write = new byte[] { 0x80, 0xF0 };
                    break;
            }
                        
            i2cTx[0] = I2CDevice.CreateWriteTransaction(write);
            lock (i2c)
            {
                i2c.Config = config;

                if (i2c.Execute(i2cTx, 100) == 0)
                    return false;
                else
                    return true;
            }
        }

        /// <summary>
        /// Clear the lcd 
        /// </summary>
        /// <returns>Returns the result of the clear operation</returns>
        public bool Clear()
        {
            Write(LCD.CHAR_NULL, LCD.CHAR_NULL, LCD.CHAR_NULL, LCD.CHAR_NULL);

            return true;
        }

        // ReSharper restore InconsistentNaming
    }
}
