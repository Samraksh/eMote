using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using System.Text;

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

        public static UInt16[] startOfRecordDelimiter = new UInt16[3];
        public static UInt16[] endOfRecordDelimiter = new UInt16[2];

        public static byte[] startOfRecordInBytes = Encoding.UTF8.GetBytes("Start ");
        public static byte[] endOfRecordInBytes = Encoding.UTF8.GetBytes("End ");

        private static UInt32 maxSize = 12 * 1024 * 1024;

        static bool fullFlag = false;

        /// <summary>
        /// Initialize the nor flash
        /// </summary>
        /// <returns></returns>
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

        public static bool EndRecord()
        {
            DeviceStatus result = DeviceStatus.Fail;

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

            //Debug.Print("Current Fill Level : " + writeAddressPtr.ToString() + " \n");

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
            if ((readAddressPtr + (length * 2) > writeAddressPtr) && (readAddressPtr < writeAddressPtr))
            {
                length = (UInt16) ((writeAddressPtr - readAddressPtr) / 2);
            }

            DeviceStatus result = InternalRead(data, readAddressPtr, length);

            readAddressPtr += (UInt16)(length * 2);

            //Debug.Print("Current Read level : " + readAddressPtr.ToString() + " \n");

            return result;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static DeviceStatus InternalRead(UInt16[] data, UInt32 readAddressPtr, UInt16 length);
        
    }
}
