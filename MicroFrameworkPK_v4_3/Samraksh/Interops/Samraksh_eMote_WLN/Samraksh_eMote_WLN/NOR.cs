using System;
using System.Runtime.CompilerServices;
using System.Text;

namespace Samraksh.eMote.WLN
{
    // ReSharper disable InconsistentNaming
    /// <summary>
    /// File positions
    /// </summary>
    public enum FilePosition
    {
        /// <summary>Beginning position</summary>
        Begin,
        /// <summary>Ending position</summary>
        End,
    }

    /// <summary>
    /// NOR (on-board) flash 
    /// </summary>
    public class NOR
    {
        private static UInt32 writeAddressPtr = 0x0;
        private static UInt32 readAddressPtr = 0x0;

        public static UInt16[] startOfRecordDelimiter = new UInt16[3];
        public static UInt16[] endOfRecordDelimiter = new UInt16[2];

        public static byte[] startOfRecordInBytes = Encoding.UTF8.GetBytes("Start ");
        public static byte[] endOfRecordInBytes = Encoding.UTF8.GetBytes("End ");

        private static UInt32 maxSize = 12 * 1024 * 1024;

        static bool fullFlag = false;

        /// <summary>
        /// Initialize NOR flash
        /// </summary>
        /// <returns>True if success</returns>
        public static bool Initialize()
        {
            startOfRecordDelimiter[0] = (UInt16) (startOfRecordInBytes[0] << 8);
            startOfRecordDelimiter[0] |= (startOfRecordInBytes[1]);
            startOfRecordDelimiter[1] = (UInt16) (startOfRecordInBytes[2] << 8);
            startOfRecordDelimiter[1] |= (startOfRecordInBytes[3]);
            startOfRecordDelimiter[2] = (UInt16) (startOfRecordInBytes[4] << 8);
            startOfRecordDelimiter[2] |= (startOfRecordInBytes[5]);

            endOfRecordDelimiter[0] = (UInt16) (endOfRecordInBytes[0] << 8);
            endOfRecordDelimiter[0] |= (UInt16)(endOfRecordInBytes[1]);
            endOfRecordDelimiter[1] = (UInt16)(endOfRecordInBytes[2] << 8);
            endOfRecordDelimiter[1] |= (UInt16)(endOfRecordInBytes[3]);

            return InternalInitialize();
        }

        /// <summary>
        /// Initialize NOR flash to a specified size
        /// </summary>
        /// <param name="maxSizeConfig">Maximum size</param>
        /// <returns>True if success</returns>
        public static bool Initialize(UInt32 maxSizeConfig)
        {
            startOfRecordDelimiter[0] = (UInt16)(startOfRecordInBytes[0] << 8);
            startOfRecordDelimiter[0] |= (startOfRecordInBytes[1]);
            startOfRecordDelimiter[1] = (UInt16)(startOfRecordInBytes[2] << 8);
            startOfRecordDelimiter[1] |= (startOfRecordInBytes[3]);
            startOfRecordDelimiter[2] = (UInt16)(startOfRecordInBytes[4] << 8);
            startOfRecordDelimiter[2] |= (startOfRecordInBytes[5]);

            endOfRecordDelimiter[0] = (UInt16)(endOfRecordInBytes[0] << 8);
            endOfRecordDelimiter[0] |= (UInt16)(endOfRecordInBytes[1]);
            endOfRecordDelimiter[1] = (UInt16)(endOfRecordInBytes[2] << 8);
            endOfRecordDelimiter[1] |= (UInt16)(endOfRecordInBytes[3]);

            maxSize = maxSizeConfig;

            return InternalInitialize();
        }

        /// <summary>
        /// Start a new record in NOR
        /// </summary>
        /// <returns>True if success</returns>
        public static bool StartNewRecord()
        {
            var result = DeviceStatus.Fail;
            var CurrentTimeStamp = DateTime.Now.Ticks;
            var time = new UInt16[4];

            if (writeAddressPtr > 0.6 * maxSize)
            {
                fullFlag = true;
            }

            // Start of record
            result = InternalWrite(startOfRecordDelimiter, writeAddressPtr, 3);

            time[0] = (UInt16) (CurrentTimeStamp & 0xffff);
            time[1] = (UInt16) ((CurrentTimeStamp & 0xffff0000) >> 16);
            time[2] = (UInt16)((CurrentTimeStamp & 0xffff00000000) >> 32);
            time[3] = (UInt16)((CurrentTimeStamp) >> 48);

            writeAddressPtr += 6;

            // Time stamp the start of record
            result = InternalWrite(time, writeAddressPtr, 4);

            writeAddressPtr += 8;

            if (result == DeviceStatus.Success)
                return true;
            else
                return false;
        }

        /// <summary>
        /// Write end of record to NOR
        /// </summary>
        /// <returns>True if success</returns>
        public static bool EndRecord()
        {
            var result = DeviceStatus.Fail;

            result = InternalWrite(endOfRecordDelimiter, writeAddressPtr, 2);

            writeAddressPtr += 4;

            if (writeAddressPtr > 0.6 * maxSize)
            {
                fullFlag = true;
            }

            if (result == DeviceStatus.Success)
                return true;
            else
                return false;
        }

        /// <summary>
        /// Check if NOR is full
        /// </summary>
        /// <returns>True if full</returns>
        public static bool IsFull()
        {
            if (fullFlag == false)
                return false;
            else
                return true;
        }

        /// <summary>
        /// Check if NOR is empty
        /// </summary>
        /// <returns>True if empty</returns>
        public static bool IsEmpty()
        {
            return (writeAddressPtr == 0x0);
        }

        /// <summary>
        /// Seek to specified position in NOR
        /// </summary>
        /// <param name="pos">Position to seek</param>
        public static void Seek(FilePosition pos)
        {
            if (pos == FilePosition.Begin)
            {
                writeAddressPtr = 0x0;
                readAddressPtr = 0x0;
                fullFlag = false;
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool InternalInitialize();

        /// <summary>
        /// Write end of file to NOR
        /// </summary>
        /// <returns>True if success</returns>
        public static bool eof()
        {
            if (readAddressPtr >= writeAddressPtr)
                return true;
            else
                return false;
        }

        /// <summary>
        /// Write data to NOR
        /// </summary>
        /// <param name="data">Array of data to write</param>
        /// <param name="length">Amount of data to write</param>
        /// <returns>True if success</returns>
        public static bool Write(UInt16[] data, UInt16 length)
        {
            if (writeAddressPtr > 0.6 * maxSize)
            {
                fullFlag = true;
            }

            var result = InternalWrite(data, writeAddressPtr, length);
            
            writeAddressPtr += (UInt32)(length * 2);

            //Debug.Print("Current Fill Level : " + writeAddressPtr.ToString() + " \n");

            if (result == DeviceStatus.Success)
                return true;
            else
                return false;
            
        }

        /// <summary>
        /// Write data to NOR
        /// </summary>
        /// <param name="data">Array of data to write</param>
        /// <param name="length">Amount of data to write</param>
        /// <param name="offset">Offset into array</param>
        /// <returns>True if success</returns>
        public static bool Write(UInt16[] data, UInt16 length, UInt16 offset)
        {

            if (writeAddressPtr > 0.6 * maxSize)
            {
                fullFlag = true;
            }

            var result = InternalWrite(data, writeAddressPtr, offset, length);

            writeAddressPtr += (UInt32)(length * 2);

            if (result == DeviceStatus.Success)
                return true;
            else
                return false;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static DeviceStatus InternalWrite(UInt16[] data, UInt32 addressPtr, UInt16 length);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static DeviceStatus InternalWrite(UInt16[] data, UInt32 addressPtr, UInt16 offset, UInt16 length);

        /// <summary>
        /// Read data from NOR
        /// </summary>
        /// <param name="data">Array to receive data</param>
        /// <param name="length">Amount of data to read</param>
        /// <returns>Status of operation</returns>
        public static DeviceStatus Read(UInt16[] data, UInt16 length)
        {
            if ((readAddressPtr + (length * 2) > writeAddressPtr) && (readAddressPtr < writeAddressPtr))
            {
                length = (UInt16) ((writeAddressPtr - readAddressPtr) / 2);
            }

            var result = InternalRead(data, readAddressPtr, length);

            readAddressPtr += (UInt16)(length * 2);

            //Debug.Print("Current Read level : " + readAddressPtr.ToString() + " \n");

            return result;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static DeviceStatus InternalRead(UInt16[] data, UInt32 readAddressPtr, UInt16 length);
        // ReSharper restore InconsistentNaming

    }
}
