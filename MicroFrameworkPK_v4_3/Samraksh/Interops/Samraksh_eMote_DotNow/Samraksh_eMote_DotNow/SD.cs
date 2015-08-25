using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.eMote.DotNow
{
    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Micro SD device
    /// </summary>
    public class SD
    {
        /// <summary>
        /// Callback delegate
        /// </summary>
        /// <param name="status">Operation status</param>
        public delegate void SDCallBackType(DeviceStatus status);

        public static SDInternal sdInternalObject;

        public static SDCallBackType sdCallbackFunction;

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="sdCallback">Callback method</param>
        public SD(SDCallBackType sdCallback)
        {
            sdInternalObject = new SDInternal("SDCallback", 1233, 0);

            sdCallbackFunction = sdCallback;

            var eventHandler = new NativeEventHandler(InternalCallback);
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

        /// <summary>
        /// Initialize the SD
        /// </summary>
        /// <returns>True iff success</returns>
        public static bool Initialize()
        {
            if (SDInternal.InternalInitialize() == DeviceStatus.Success)
                return true;
            else
                return false;
        }

    

        /// <summary>
        /// Read from SD
        /// </summary>
        /// <param name="dataArray">Array to receive data</param>
        /// <param name="offset">Offset into array</param>
        /// <param name="length">Amount to read</param>
        /// <returns>True iff success</returns>
        public static bool Read(byte[] dataArray, UInt16 offset, UInt16 length)
        {
            var result = SDInternal.InternalRead(dataArray, offset, length, readAddressPtr);

            readAddressPtr += (UInt32)(length * 2);

            if (result == DeviceStatus.Success)
                return true;
            else
                return false;
        }

        /// <summary>
        /// Write to SD
        /// </summary>
        /// <param name="dataArray">Array to write</param>
        /// <param name="offset">Offset into array</param>
        /// <param name="length">Amount to write</param>
        /// <returns>True iff success</returns>
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
    // ReSharper restore InconsistentNaming

    //--//

}
