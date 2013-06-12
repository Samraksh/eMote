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

        private static UInt32 maxSize = 12 * 1024 * 1024;

        static bool fullFlag = false;

        /// <summary>
        /// Initialize the nor flash
        /// </summary>
        /// <returns></returns>
        public static bool Initialize()
        {
            return InternalInitialize();
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
