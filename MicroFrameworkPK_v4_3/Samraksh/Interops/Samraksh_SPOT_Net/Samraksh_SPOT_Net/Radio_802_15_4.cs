using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

//using Samraksh.SPOT.Net.Radio;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.Net.Radio
{

    /// <summary>
    /// Enum defines the different power levels supported by the RF231 radio
    /// </summary>
    public enum TxPowerValue
    {
        Power_3dBm,
        Power_2Point8dBm,
        Power_2Point3dBm,
        Power_1Point8dBm,
        Power_1Point3dBm,
        Power_0Point7dBm,
        Power_0Point0dBm,
        Power_Minus1dBm,
        Power_Minus2dBm,
        Power_Minus3dBm,
        Power_Minus4dBm,
        Power_Minus5dBm,
        Power_Minus7dBm,
        Power_Minus9dBm,
        Power_Minus12dBm,
        Power_Minus17dBm,


    }

    /// <summary>
    /// Enum defines the different channels the RF231 radio can use
    /// </summary>
    public enum Channels
    {
        /// <summary>
        /// Channel 11 Frequency 2405 MHz
        /// </summary>
        Channel_11,
        /// <summary>
        /// Channel 12 Frequency 2410 MHz
        /// </summary>
        Channel_12,
        /// <summary>
        /// Channel 13 Frequency 2415 MHz
        /// </summary>
        Channel_13,
        /// <summary>
        /// Channel 14 Frequency 2420 MHz
        /// </summary>
        Channel_14,
        /// <summary>
        /// Channel 15 Frequency 2425 MHz
        /// </summary>
        Channel_15,
        /// <summary>
        /// Channel 16 Frequency 2430 MHz
        /// </summary>
        Channel_16,
        /// <summary>
        /// Channel 17 Frequency 2435 MHz
        /// </summary>
        Channel_17,
        /// <summary>
        /// Channel 18 Frequency 2440 MHz
        /// </summary>
        Channel_18,
        /// <summary>
        /// Channel 19 Frequency 2445 MHz
        /// </summary>
        Channel_19,
        /// <summary>
        /// Channel 20 Frequency 2450 MHz
        /// </summary>
        Channel_20,
        /// <summary>
        /// Channel 21 Frequency 2455 MHz
        /// </summary>
        Channel_21,
        /// <summary>
        /// Channel 22 Frequency 2460 MHz
        /// </summary>
        Channel_22,
        /// <summary>
        /// Channel 23 Frequency 2465 MHz
        /// </summary>
        Channel_23,
        /// <summary>
        /// Channel 24 Frequency 2470 MHz
        /// </summary>
        Channel_24,
        /// <summary>
        /// Channel 25 Frequency 2475 MHz
        /// </summary>
        Channel_25,
        /// <summary>
        /// Channel 26  Frequency 2480 MHz
        /// </summary>
        Channel_26,
    }



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
        // Size of the radio message
        const byte RadioMessageSize = 128;

        // Size of the radio configuration byte array for marshalling purposes
        // Note we are marshalling because NETMF does not support passing custom types to native code
        const byte RadioConfigSize = 2;

        static ReceiveCallBack MyReceiveCallback;
        static byte RSSI, LinkQuality;
        static UInt16 Src;
        static bool Unicast;
        static byte[] ReceiveMessage = new byte[RadioMessageSize];

        // Create a buffer that you can use when you want to marshal
        static byte[] marshalBuffer = new byte[RadioConfigSize];

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
            marshalBuffer[0] = (byte) config.Channel;
            marshalBuffer[1] = (byte) config.TxPower;
            return InternalInitialize(marshalBuffer, ReceiveMessage);
        }

        private static void ReceiveFunction(uint data1, uint data2, DateTime time)
        {
            Src = (UInt16) ((data1 >> 16) & 0x0000FFFF);
            RSSI = (byte)(data2 & 0x000000FF);
            LinkQuality = (byte)((data2 >> 8) & 0x000000FF);
            Unicast = ((data2 >> 16) & 0x000000FF) > 0 ? true : false ;
            MyReceiveCallback(ReceiveMessage, (UInt16) data1, Src, Unicast, RSSI, LinkQuality);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern DeviceStatus InternalInitialize(byte[] config, byte[] receiveMessage);    // Changed to private by Bill Leal 2/6/2013 per Mukundan Sridharan.
        
        /// <summary>
        /// Set MAC configuration for 802.15.4 radio.
        /// </summary>
        /// <param name="config">MAC configuration.</param>
        /// <returns>The status after the method call: Success, Fail, Ready, Busy</returns>
        /// <remarks>Used to change the MAC configuration after initialization.</remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus Configure(RadioConfiguration config);

        /// <summary>
        /// Set the transmit power of the 802.15.4 radio.
        /// </summary>
        /// <param name="TxPower"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetTxPower(int TxPower);

        /// <summary>
        /// Set the channel of the 802.15.4 radio.
        /// </summary>
        /// <param name="Channel"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetChannel(int Channel);

        /// <summary>
        /// Return the output power of the radio 802.15.4
        /// </summary>
        /// <returns>TxPowerValue</returns>
        public TxPowerValue GetTxPowerValue()
        {
            return (TxPowerValue)GetTxPower();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern int GetTxPower();


        /// <summary>
        /// Return the current active channel of the radio 
        /// </summary>
        /// <returns>Channels</returns>
        public Channels GetActiveChannel()
        {
            return (Channels)GetChannel();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern int GetChannel();

        /// <summary>
        /// Reconfigure the radio object with new channel and power
        /// </summary>
        /// <param name="config"></param>
        /// <returns>DeviceStatus</returns>
        public DeviceStatus ReConfigure(RadioConfiguration config)
        {
            marshalBuffer[0] = (byte) config.TxPower;
            marshalBuffer[1] = (byte) config.Channel;

            return ReConfigure(marshalBuffer);

        }

        //Make the interop call to set the new configuration 
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus ReConfigure(byte[] data); 

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
