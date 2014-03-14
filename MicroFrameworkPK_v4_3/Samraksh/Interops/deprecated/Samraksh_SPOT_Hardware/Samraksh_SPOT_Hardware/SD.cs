using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.SPOT.Hardware.EmoteDotNow
{
    public class SD
    {
        public delegate void SDCallBackType(DeviceStatus status);

        public static SDInternal sdInternalObject;

        public static SDCallBackType sdCallbackFunction;

        public SD(SDCallBackType sdCallback)
        {
            sdInternalObject = new SDInternal("SDCallback", 1233, 0);

            sdCallbackFunction = sdCallback;

            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            sdInternalObject.OnInterrupt += eventHandler;

            
        }

        public static UInt32 writeAddressPtr = 0x0;
        public static UInt32 readAddressPtr = 0x0;

        static public void InternalCallback(uint data1, uint data2, DateTime time)
        {
            if (data2 == (uint)DeviceStatus.Success)
                sdCallbackFunction(DeviceStatus.Success);
            else
                sdCallbackFunction(DeviceStatus.Fail);
        }

        public static bool Initialize()
        {
            if (SDInternal.InternalInitialize() == DeviceStatus.Success)
                return true;
            else
                return false;
        }

    

        public static bool Read(byte[] dataArray, UInt16 offset, UInt16 length)
        {
            DeviceStatus result = SDInternal.InternalRead(dataArray, offset, length, readAddressPtr);

            readAddressPtr += (UInt32)(length * 2);

            if (result == DeviceStatus.Success)
                return true;
            else
                return false;
        }

        public static bool Write(byte[] dataArray, UInt16 offset, UInt16 length)
        {
            DeviceStatus result = SDInternal.InternalWrite(dataArray, offset, length, writeAddressPtr);

            writeAddressPtr += (UInt32) (length * 2);

            if (result == DeviceStatus.Success)
                return true;
            else
                return false;

        }



    }

    public class SDInternal : NativeEventDispatcher
    {
         public SDInternal(string strDrvName, ulong drvData, int callbackCount)
            : base(strDrvName, drvData)
        {

        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern DeviceStatus InternalInitialize();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern DeviceStatus InternalWrite(byte[] dataArray, UInt16 offset, UInt16 length, UInt32 writeAddressPtr);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern DeviceStatus InternalRead(byte[] dataArray, UInt16 offset, UInt16 length, UInt32 readAddressPtr);
    }
}
