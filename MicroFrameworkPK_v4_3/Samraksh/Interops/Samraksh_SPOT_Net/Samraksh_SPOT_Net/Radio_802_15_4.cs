using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

//using Samraksh.SPOT.Net.Radio;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.Net.Radio
{
    /*class Radio_15_4_CB : NativeEventDispatcher
    {
        public Radio_15_4_CB(string strDrvName, ulong drvData, int callbackCount)
            : base(strDrvName, drvData)
        {

        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int GetStatus();
    }*/

    /// <summary>
    /// 802.15.4 radio configuration
    /// </summary>
    public class Radio_802_15_4 : NativeEventDispatcher, IRadio
    {
        const byte RadioMessageSize = 128;
        static ReceiveCallBack MyReceiveCallback;
        static byte[] ReceiveMessage = new byte[RadioMessageSize];

        /// <summary>
        /// Constructor for 802.15.4 radio
        /// </summary>
        public Radio_802_15_4()
            : base("RadioCallback_802_15_4", 1234)
        {           
        }
        
        /// <summary>
        /// Initialize native radio and interop drivers.
        /// </summary>
        /// <param name="config">MAC configuration.</param>
        /// <param name="callback">Callback method for received messages.</param>
        /// <returns>The status after the method call: Success, Fail, Ready, Busy</returns>
        public DeviceStatus Initialize(RadioConfiguration config, ReceiveCallBack callback)
        {
            MyReceiveCallback = callback;
            NativeEventHandler eventHandler = new NativeEventHandler(ReceiveFunction);
            OnInterrupt += eventHandler;
            return InternalInitialize(config, ReceiveMessage);
        }

        private static void ReceiveFunction(uint data1, uint data2, DateTime time)
        {
            MyReceiveCallback(ReceiveMessage, (UInt16) data1);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern DeviceStatus InternalInitialize(RadioConfiguration config, byte[] receiveMessage);    // Changed to private by Bill Leal 2/6/2013 per Mukundan Sridharan.
        
        /// <summary>
        /// Set MAC configuration for 802.15.4 radio.
        /// </summary>
        /// <param name="config">MAC configuration.</param>
        /// <returns>The status after the method call: Success, Fail, Ready, Busy</returns>
        /// <remarks>Used to change the MAC configuration after initialization.</remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus Configure(RadioConfiguration config);

        /// <summary>
        /// Uninitialize native MAC, radio and interop drivers.
        /// </summary>
        /// <returns>The status after the method call: Success, Fail, Ready, Busy</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus UnInitialize();

        /// <summary>
        /// Get the ID of this 802.15.4 radio instance.
        /// </summary>
        /// <returns>The ID of the instance.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetID();

        /// <summary>
        /// Get the address of the device.
        /// </summary>
        /// <returns>Address of the device.</returns>
        /// <remarks>This is the address by which the device is known to the rest of the world.</remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 GetAddress();

        /// <summary>
        /// Set the address of the device.
        /// </summary>
        /// <param name="Address">The address of the device.</param>
        /// <returns>True iff address successfully set.</returns>
        /// <remarks>This is the address by which the device is known to the rest of the world. 
        ///     A return value of false can occur if another layer locks the address and prevents changes.
        /// </remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetAddress(UInt16 Address);

        /// <summary>
        /// Turn on the radio.
        /// </summary>
        /// <returns>The status after the method call: Success, Fail, Ready, Busy</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus TurnOn();

        /// <summary>
        /// Go to sleep.
        /// </summary>
        /// <param name="level">The sleep level.</param>
        /// <returns>The status after the method call: Success, Fail, Ready, Busy</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus Sleep(byte level);

        /// <summary>
        /// Load the message into the transmit buffer of the radio.
        /// </summary>
        /// <param name="message">The message.</param>
        /// <param name="size">The size of the message.</param>
        /// <returns>The result of the method: E_RadioInit, E_RadioSync, E_RadioConfig, E_MacInit, E_MacConfig, E_MacSendError, E_MacBufferFull, S_Success</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus PreLoad(byte[] message, UInt16 size); 

        /// <summary>
        /// Sent a message that's already in the transmit buffer.
        /// </summary>
        /// <returns>The result of the method: E_RadioInit, E_RadioSync, E_RadioConfig, E_MacInit, E_MacConfig, E_MacSendError, E_MacBufferFull, S_Success</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus SendStrobe();	//Send preloaded message
        
        /// <summary>
        /// Load and send a message.
        /// </summary>
        /// <param name="message">The message.</param>
        /// <param name="size">The size of the message.</param>
        /// <returns>The result of the method: E_RadioInit, E_RadioSync, E_RadioConfig, E_MacInit, E_MacConfig, E_MacSendError, E_MacBufferFull, S_Success</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus Send(byte[] message, UInt16 size);
        
        /// <summary>
        /// Load and send a message, with radio layer time stamping. 
        /// </summary>
        /// <param name="message">The message.</param>
        /// <param name="size">The size of the message.</param>
        /// <param name="eventTime">The time stamp.</param>
        /// <returns>The result of the method: E_RadioInit, E_RadioSync, E_RadioConfig, E_MacInit, E_MacConfig, E_MacSendError, E_MacBufferFull, S_Success</returns>
        /// <remarks>The offset for the timestamp in the packet is specified by TimeStampOffset  member of the RadioConfiguration structure passed as parameter during radio module initialization.</remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus SendTimeStamped(byte[] message, UInt16 size, UInt32 eventTime);
        
        /// <summary>
        /// Assess channel activity.
        /// </summary>
        /// <returns>True iff channel is free.</returns>
        /// <remarks>Default is 140 microseconds.</remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ClearChannelAssesment();
        
        /// <summary>
        /// Assess channel activity.
        /// </summary>
        /// <param name="numberOfMicroSecond">Number of microsecondsto check.</param>
        /// <returns>True iff channel is free.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ClearChannelAssesment(UInt16 numberOfMicroSecond);
    }
}
