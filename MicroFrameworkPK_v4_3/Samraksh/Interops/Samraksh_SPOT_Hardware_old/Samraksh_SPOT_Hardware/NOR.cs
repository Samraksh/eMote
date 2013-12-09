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
            startOfRecordDelimiter[0] = 0xfff0;
            startOfRecordDelimiter[1] = 0xf0ff;

            endOfRecordDelimiter[0] = 0xfeef;
            endOfRecordDelimiter[1] = 0xeffe;

            return InternalInitialize();
        }

        public static bool Initialize(UInt32 maxSizeConfig)
        {
            startOfRecordDelimiter[0] = 0xfff0;
            startOfRecordDelimiter[1] = 0xf0ff;

            endOfRecordDelimiter[0] = 0xfeef;
            endOfRecordDelimiter[1] = 0xeffe;

            maxSize = maxSizeConfig;

            return InternalInitialize();
        }

        public static bool StartNewRecord()
        {
            DeviceStatus result = DeviceStatus.Fail;
            long CurrentTimeStamp = DateTime.Now.Ticks;
            UInt16[] time = new UInt16[4];

            if (writeAddressPtr > 0.6 * maxSize)
            {
                fullFlag = true;
            }

            // Start of record
            result = InternalWrite(startOfRecordDelimiter, writeAddressPtr, 2);

            time[0] = (UInt16) (CurrentTimeStamp & 0xffff);
            time[1] = (UInt16) ((CurrentTimeStamp & 0xffff0000) >> 16);
            time[2] = (UInt16)((CurrentTimeStamp & 0xffff00000000) >> 32);
            time[3] = (UInt16)((CurrentTimeStamp) >> 48);

            writeAddressPtr += 4;

            // Time stamp the start of record
            result = InternalWrite(time, writeAddressPtr, 4);

            writeAddressPtr += 8;

            if (result == DeviceStatus.Success)
                return true;
            else
                return false;
        }

        public static bool EndRecord()
        {
            DeviceStatus result = DeviceStatus.Fail;

            result = InternalWrite(startOfRecordDelimiter, writeAddressPtr, 2);

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
                fullFlag = false;
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool InternalInitialize();

        public static bool eof()
        {
            if (readAddressPtr >= writeAddressPtr)
                return true;
            else
                return false;
        }

        public static bool Write(UInt16[] data, UInt16 length)
        {
            if (writeAddressPtr > 0.6 * maxSize)
            {
                fullFlag = true;
            }

            DeviceStatus result = InternalWrite(data, writeAddressPtr, length);
            
            writeAddressPtr += (UInt32)(length * 2);

            Debug.Print("Current Fill Level : " + writeAddressPtr.ToString() + " \n");

            if (result == DeviceStatus.Success)
                return true;
            else
                return false;
            
        }

        public static bool Write(UInt16[] data, UInt16 length, UInt16 offset)
        {

            if (writeAddressPtr > 0.6 * maxSize)
            {
                fullFlag = true;
            }

            DeviceStatus result = InternalWrite(data, writeAddressPtr, offset, length);

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

        public static DeviceStatus Read(UInt16[] data, UInt16 length)
        {
            if (readAddressPtr + (length * 2) > writeAddressPtr)
            {
                length = (UInt16) ((writeAddressPtr - readAddressPtr) / 2);
            }

            DeviceStatus result = InternalRead(data, readAddressPtr, length);

            readAddressPtr += (UInt16)(length * 2);

            Debug.Print("Current Read level : " + readAddressPtr.ToString() + " \n");

            return result;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static DeviceStatus InternalRead(UInt16[] data, UInt32 readAddressPtr, UInt16 length);
        
    }
}
