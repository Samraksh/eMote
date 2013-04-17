using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net.Mac;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.SPOT.Net
{
    // Making this private as the user should really never be seeing this 
    private class MACBase : NativeEventDispatcher, IMac
    {
        /// <summary>
        /// Specifies the marshalling buffer size, used by the config to pass data to native code 
        /// </summary>
        const byte MarshalBufferSize = 5;


        /// <summary>
        /// Specifies the neighbour size
        /// </summary>
        const byte NeighborSize = 22; //Look at IMac.cs to figure out the size of the Neighbor structure.

        Message message;

        byte[] ByteNeighbor = new byte[NeighborSize];

        byte[] MarshalBuffer = new byte[MarshalBufferSize];

        /// <summary>
        /// Callback for the MAC object 
        /// </summary>
        ReceiveCallBack MyReceiveCallback;

        /// <summary>
        /// Constructor to initialize callbacks 
        /// </summary>
        /// <param name="drvName">Name of the callback</param>
        /// <param name="drvData">Driver data</param>
        public MACBase(string drvName ,ulong drvData) : base(drvName, drvData) 
        {
        }


        private void ReceiveFunction(uint data1, uint data2, DateTime time)
        {
            /*
            Src = (UInt16)((data1 >> 16) & 0x0000FFFF);
            RSSI = (byte)(data2 & 0x000000FF);
            LinkQuality = (byte)((data2 >> 8) & 0x000000FF);
            Unicast = ((data2 >> 16) & 0x000000FF) > 0 ? true : false;
             */
            //MyReceiveCallback(ReceiveMessage, (UInt16)data1, Src, Unicast, RSSI, LinkQuality);
        }

        /// <summary>
        /// Initialize MAC
        /// </summary>
        /// <param name="config">MAC configuration.</param>
        /// <param name="callback">Callback method for received messages.</param>
        /// <returns>The radio status after initialization: Success, Fail, Ready, Busy</returns>
        /// <remarks>By default, uses Radio #1 on the device. For the eMote.NOW 1.0, that is the 802.15.4 radio.</remarks>
        public DeviceStatus Initialize(MacConfiguration config, ReceiveCallBack callback)
        {
            MyReceiveCallback = callback;
            NativeEventHandler eventHandler = new NativeEventHandler(ReceiveFunction);
            OnInterrupt += eventHandler;
            return InternalInitialize(MarshalBuffer);
        }

        /// <summary>
        /// Set the CCA
        /// </summary>
        /// <param name="CCA"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetCCA(bool CCA);

        /// <summary>
        /// Set Number of retries of the MAC
        /// </summary>
        /// <param name="NumberOfRetries"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetNumberOfRetries(byte NumberOfRetries);

        /// <summary>
        /// Set CCA Sense Time
        /// </summary>
        /// <param name="CCASenseTime"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetCCASenseTime(byte CCASenseTime);

        /// <summary>
        /// Set Buffer Size
        /// </summary>
        /// <param name="BufferSize"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetBufferSize(byte BufferSize);

        /// <summary>
        /// Set Radio ID
        /// </summary>
        /// <param name="RadioID"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetRadioID(byte RadioID);

        /// <summary>
        /// Get CCA
        /// </summary>
        /// <returns>bool</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool GetCCA();

        /// <summary>
        /// Get number of retries
        /// </summary>
        /// <returns>byte</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetNumberOfRetries();

        /// <summary>
        /// Get CCA Sense Time
        /// </summary>
        /// <returns>byte</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetCCASenseTime();

        /// <summary>
        /// Get Radio ID
        /// </summary>
        /// <returns>byte</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetRadioID();


        /// <summary>
        /// Initialize native MAC, radio and interop drivers.
        /// </summary>
        /// <param name="config">MAC configuration.</param>
        /// <param name="receiveMessage">Byte array for received messages.</param>
        /// <returns>The driver status after initialization: Success, Fail, Ready, Busy</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus InternalInitialize(byte[] marshalBuffer);  // Changed to private by Bill Leal 2/6/2013 per Mukundan Sridharan.

    }
}
