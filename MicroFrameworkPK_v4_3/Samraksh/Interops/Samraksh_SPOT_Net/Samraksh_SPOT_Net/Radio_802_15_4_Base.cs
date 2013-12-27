using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

//using Samraksh.SPOT.Net.Radio;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.Net.Radio
{

    public enum RadioName
    {
        RF231RADIO,
        RF231RADIOLR,
    }

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


    /// <summary>
    /// 802.15.4 radio configuration
    /// </summary>
    public class Radio_802_15_4_Base : NativeEventDispatcher, IRadio
    {
        // Size of the radio message
        const byte RadioMessageSize = 128;

        // Size of the radio configuration byte array for marshalling purposes
        // Note we are marshalling because NETMF does not support passing custom types to native code
        const byte RadioConfigSize = 3;

        byte[] dataBuffer = new byte[RadioMessageSize];

        Message message;

        private RadioName radioName;

        public void SetRadioName(RadioName radioName)
        {
            this.radioName = radioName;
        }

        public RadioName GetRadioName()
        {
            return this.radioName;
        }

        public static RadioConfiguration config = null;

        // Create a buffer that you can use when you want to marshal
        byte[] marshalBuffer = new byte[RadioConfigSize];

        // Maintains who the current user of the radio is CSharp/ MAC Objects
        public static RadioUser currUser = RadioUser.IDLE;

        /// <summary>
        /// Constructor for 802.15.4 radio
        /// </summary>
        public Radio_802_15_4_Base()
            : base("RadioCallback_802_15_4", 1234)
        {

            if (config == null || Callbacks.GetReceiveCallback() == null)
                throw new RadioNotConfiguredException();

            Initialize(config);
        }

        public Radio_802_15_4_Base(string drvname, ulong drvData)
            : base(drvname, drvData)
        {


            if (config == null)
                Debug.Print("The Configuration is null\n");

            
            if (config == null ||  Callbacks.GetReceiveCallback() == null)
                throw new RadioNotConfiguredException();

            ShallowInitialize(config);
        }

        /// <summary>
        /// This function explicitly releases the message packet and is free to be collected by the gc. If this is not called, the packet is released during the next GetNextPacket call
        /// </summary>
        /// <returns></returns>
        public void ReleaseMessage()
        {
            message = null;
        }

        /*
        private static Radio_802_15_4 instance;
        private static object syncObject = new Object();

        public static Radio_802_15_4 GetInstance()
        {
            if (instance == null)
            {
                lock (syncObject)
                {
                    if (instance == null)
                        instance = new Radio_802_15_4();
                }
            }

            return instance;
        }

        /// <summary>
        /// Get a shallow instance of the radio object, should only be used by the mac 
        /// </summary>
        /// <param name="user"></param>
        /// <returns>Returns a radio object</returns>
        public static Radio_802_15_4 GetShallowInstance(RadioUser user)
        {
            if (instance == null)
            {
                lock (syncObject)
                {
                    if (instance == null)
                    {
                        if (user == RadioUser.CSMAMAC)
                            instance = new Radio_802_15_4("CSMACallback", 4321);
                        else if (user == RadioUser.OMAC)
                            instance = new Radio_802_15_4("OMACCallback", 4322);
                        else if (user == RadioUser.CSharp)
                            instance = new Radio_802_15_4();
                    }
                }
            }

            return instance;
        }
         * */

        /// <summary>
        /// Get the next packet from the radio driver, the radio does not maintain a buffer, the onus is on the application to sample this data as quickly as possible on getting a recieve interrupt
        /// Otherwise the packet is overwritten in the radio layer, for buffer support use the mac interface 
        /// </summary>
        /// <returns>A data packet of message type to the caller</returns>
        public Message GetNextPacket()
        {
            if (GetNextPacket(dataBuffer) != DeviceStatus.Success)
                return null;

            message = new Message(dataBuffer);

            return message;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus GetNextPacket(byte[] nativeBuffer);


        private DeviceStatus ShallowInitialize(RadioConfiguration config)
        {
            NativeEventHandler eventHandler = new NativeEventHandler(Callbacks.ReceiveFunction);
            OnInterrupt += eventHandler;

            return DeviceStatus.Success;
        }
        
        /// <summary>
        /// Initialize native radio and interop drivers.
        /// </summary>
        /// <param name="config">MAC configuration.</param>
        /// <param name="callback">Callback method for received messages.</param>
        /// <returns>The status after the method call: Success, Fail, Ready, Busy</returns>
        private DeviceStatus Initialize(RadioConfiguration config)
        {
            NativeEventHandler eventHandler = new NativeEventHandler(Callbacks.ReceiveFunction);
            OnInterrupt += eventHandler;
            marshalBuffer[0] = (byte)config.GetChannel();
            marshalBuffer[1] = (byte) config.GetTxPower();
            marshalBuffer[2] = (byte)config.GetRadioName();
            return InternalInitialize(marshalBuffer);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus InternalInitialize(byte[] config);    // Changed to private by Bill Leal 2/6/2013 per Mukundan Sridharan.


        public static DeviceStatus Configure(RadioConfiguration config, ReceiveCallBack rcallback, NeighbourhoodChangeCallBack ncallback)
        {
            Radio_802_15_4_Base.config = new RadioConfiguration(config);
            Callbacks.SetReceiveCallback(rcallback);
            Callbacks.SetNeighbourChangeCallback(ncallback);

            return DeviceStatus.Success;

        }

        /// <summary>
        /// Set Radio configuration for 802.15.4 radio.
        /// </summary>
        /// <param name="config">Radio configuration.</param>
        /// <returns>The status after the method call: Success, Fail, Ready, Busy</returns>
        /// <remarks>Used to change the Radio configuration during and after initialization, using this function can change the callback if a different callback is used. Please use reconfigure to change power and channel</remarks>
        public static DeviceStatus Configure(RadioConfiguration config)
        {
            DeviceStatus result = DeviceStatus.Success;

            Radio_802_15_4_Base.config = new RadioConfiguration(config);

            return result;
        }

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
            marshalBuffer[0] = (byte) config.GetTxPower();
            marshalBuffer[1] = (byte) config.GetChannel();

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
