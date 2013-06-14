using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.Hardware.EmoteDotNow
{

    public enum FilePosition
    {
        Begin,
        End,
    }

    public class NOR
    {
        private static UInt32 writeAddressPtr = 0x0;
        private static UInt32 readAddressPtr = 0x0;

        public static UInt16[] startOfRecordDelimiter = new UInt16[2];
        public static UInt16[] endOfRecordDelimiter = new UInt16[2];

        private static UInt32 maxSize = 12 * 1024 * 1024;

        static bool fullFlag = false;

        /// <summary>
        /// Initialize the nor flash
        /// </summary>
        /// <returns></returns>
        public static bool Initialize()
        {
            startOfRecordDelimiter[0] = 0xff;
            startOfRecordDelimiter[1] |= (0xf0 << 8);

            endOfRecordDelimiter[0] = 0xfe;
            endOfRecordDelimiter[1] |= (0xef << 8);

            return InternalInitialize();
        }

        public static bool Initialize(UInt32 maxSizeConfig)
        {
            startOfRecordDelimiter[0] = 0xff;
            startOfRecordDelimiter[1] |= (0xf0 << 8);

            endOfRecordDelimiter[0] = 0xfe;
            endOfRecordDelimiter[1] |= (0xef << 8);

            maxSize = maxSizeConfig;

            return InternalInitialize();
        }

        public static bool StartNewRecord()
        {
            bool result = false;
            long CurrentTimeStamp = DateTime.Now.Ticks;
            UInt16[] time = new UInt16[4];

            if (writeAddressPtr > 0.6 * maxSize)
            {
                fullFlag = true;
            }

            // Start of record
            result = InternalWrite(startOfRecordDelimiter, writeAddressPtr, 2);

            time[0] = (UInt16) (CurrentTimeStamp & 0xff);
            time[1] = (UInt16) ((CurrentTimeStamp & 0xff00) >> 16);
            time[2] = (UInt16)((CurrentTimeStamp & 0xff0000) >> 32);
            time[3] = (UInt16)((CurrentTimeStamp & 0xff000000) >> 48);

            writeAddressPtr += 4;

            // Time stamp the start of record
            result = InternalWrite(time, writeAddressPtr, 4);

            writeAddressPtr += 8;

            return result;
        }

        public static bool EndRecord()
        {
            bool result = false;

            result = InternalWrite(startOfRecordDelimiter, writeAddressPtr, 2);

            writeAddressPtr += 4;

            if (writeAddressPtr > 0.6 * maxSize)
            {
                fullFlag = true;
            }

            return result;
        }

        public static bool IsFull()
        {
            if (fullFlag == false)
                return false;
            else
                return true;
        }

        public static bool IsEmpty()
        {
            return (writeAddressPtr == 0x0);
        }

        public static void Seek(FilePosition pos)
        {
            if (pos == FilePosition.Begin)
            {
                writeAddressPtr = 0x0;
                readAddressPtr = 0x0;
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool InternalInitialize();

        public static bool Write(UInt16[] data, UInt16 length)
        {
            if (writeAddressPtr > 0.6 * maxSize)
            {
                fullFlag = true;
            }

            bool result = InternalWrite(data, writeAddressPtr, length);
            
            writeAddressPtr += (UInt32)(length * 2);

            return result;
            
        }

        public static bool Write(UInt16[] data, UInt16 length, UInt16 offset)
        {

            if (writeAddressPtr > 0.6 * maxSize)
            {
                fullFlag = true;
            }

            bool result = InternalWrite(data, writeAddressPtr, offset, length);

            writeAddressPtr += (UInt32)(length * 2);

            return result;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool InternalWrite(UInt16[] data, UInt32 addressPtr, UInt16 length);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool InternalWrite(UInt16[] data, UInt32 addressPtr, UInt16 offset, UInt16 length);

        public static bool Read(UInt16[] data, UInt16 length)
        {
            bool result = InternalRead(data, readAddressPtr, length);

            readAddressPtr += (UInt16)(length * 2);

            return result;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool InternalRead(UInt16[] data, UInt32 readAddressPtr, UInt16 length);
        
    }
}
